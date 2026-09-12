"""
Hydragon Engine - Volume stage authoring, PhysX trigger plumbing, and display toggle

Everything the Force and Kill volume systems need in common, so neither controller duplicates it.

Three responsibilities, in three clearly separated sections:

1. STAGE AUTHORING. A volume is drawn and simulated by two prims, each with exactly one job:

       volumes/<type>_bounds    BasisCurves   purpose = "default"     -> the VISUAL
       volumes/<type>_trigger   Cube/Sphere    visibility = "invisible" -> the PHYSICS

   Both are generated from the SAME shape parameters, via `volume_geometry`, so the drawn cage and
   the region PhysX reports on cannot drift apart. Nothing here is invented: the size always comes
   from measuring the authored wireframe (`volume_bounds`), which stays the source of truth.

   Two prims rather than one because the two jobs have contradictory requirements. A single prim
   would have to be a collider AND invisible AND drawn as wires - which cannot be expressed without
   a rendering hack. See TRIGGER_HIDING for the mechanisms and why.

2. TRIGGER STATE. PhysX resolves the overlap in its own broadphase, on its own threads, so no Python
   loop over volumes x rigid bodies is needed per step - the systems read the result back. Read from
   `PhysxTriggerStateAPI`'s relationship rather than from trigger reports, because a report
   identifies the actor with a FABRIC handle that cannot be resolved to a USD path from Python.

3. DISPLAY. The Shift+V / "Show By Type" toggle, moved here from the retired `omni.ui.scene`
   overlay. It authors `visibility` on the WIREFRAME only, in the session layer, so a presentation
   toggle can never disturb the physics prim.

All code and comments in English per AGENTS.md section 1.
"""

from typing import Any, Dict, Optional, Tuple

try:
    import carb
    import omni.usd
    from pxr import Usd, UsdGeom, Gf, UsdPhysics
    try:
        from pxr import PhysxSchema
    except ImportError:  # pragma: no cover - only true outside a PhysX-enabled USD build
        PhysxSchema = None
    try:
        from pxr import Vt
    except ImportError:  # pragma: no cover
        Vt = None
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    Usd = None
    UsdGeom = None
    Gf = None
    Vt = None
    UsdPhysics = None
    PhysxSchema = None

from . import volume_bounds, volume_geometry


#: Setting that drives the Shift+V / "Show By Type > Hydragon Volumes" toggle.
#:
#: Deliberately the SAME persistent path the retired overlay used, so an existing preference keeps
#: working. It gates the WIREFRAMES only; the trigger colliders are never touched by it.
SETTING_SHOW_VOLUMES = "/persistent/app/hydragon/viewport/showVolumes"

#: How the trigger collider is kept out of the viewport AND out of the pick buffer.
#:
#: "visibility" - `visibility = "invisible"`. A prim that is not rasterized writes nothing into the
#:                pick buffer, so it has NO clickable area at all - by construction, not by rule.
#:                That is the property an invisible-but-present collider lacks, and the whole
#:                reason the `primvars:isVolume` route was rejected: `isVolume` is a shading hint,
#:                so it hides the surface while still capturing clicks over it.
#:                It also means the collider has no visual role whatsoever, which is what makes it
#:                safe to hide: the wireframe is a SEPARATE prim.
#:                Assumption to verify in Kit: that omni.physx still cooks a collider on an
#:                invisible prim. Kit ships `/physics/mouseGrabIgnoreInvisible`, which implies
#:                invisible bodies do remain in the scene, but that is an inference, not a
#:                measurement.
#: "purpose"    - `purpose = "guide"`. Confirmed physics-safe: NVIDIA hides collision meshes this
#:                way in `omni.physx/scripts/deformableUtils.py`. Use it if "visibility" turns out
#:                to disable collider cooking. Known drawback: it does NOT hide a shaded Gprim in
#:                every viewport, because `guide` is a display-purpose filter the artist can have
#:                switched on.
TRIGGER_HIDING = "visibility"

#: Relative path of the trigger collider, per volume type. Mirrors
#: `volume_bounds.BOUNDS_PRIM_REL_PATH`, so the visual and the physics prim sit in the same frame.
TRIGGER_PRIM_REL_PATH = {
    "Force": "volumes/force_trigger",
    "Kill": "volumes/kill_trigger",
}

#: Display colour of the wireframe, per volume type.
WIREFRAME_COLORS = {
    "Force": (0.2, 0.7, 1.0),
    "Kill": (1.0, 0.2, 0.2),
}

#: Tube width of the wireframe, in stage units. Also the width of the clickable area, because the
#: native pick reads the rasterized pixels.
WIREFRAME_WIDTH = 2.5

#: What was last authored for a given prim path, so an unchanged volume is not re-written.
#:
#: This matters: authoring into the root layer marks the stage dirty, and a system that re-authors
#: identical values every time it runs would leave the document permanently "unsaved". The cache
#: is only trusted while the prim still exists and still has the expected type.
_authored: Dict[str, tuple] = {}

#: Last visibility written per wireframe path, so a resync does not re-author an unchanged state.
_visibility_applied: Dict[str, bool] = {}

#: Authoring failures already reported, keyed by prim path. One log line per path per session -
#: AGENTS.md section E. `forget()` clears an entry so a repaired volume can report again.
_failures_reported: set = set()


def _report_authoring_failure(path: str, error: Exception) -> None:
    """Reports an authoring failure once, loudly, with the traceback.

    ONCE, because callers retry on every resync: without the guard, one bad prim would print a
    traceback per sync, which is the log-spam failure mode AGENTS.md section E exists to prevent.

    LOUDLY, because a volume whose geometry could not be authored has no gameplay effect at all, and
    a quiet failure here would be indistinguishable from "there is simply nothing here".

    The traceback is included because the exceptions from this API are coding errors whose text is
    the only clue: `AddTranslateOp()` failing on an existing op, for instance.
    """
    if path in _failures_reported:
        return
    _failures_reported.add(path)
    _log_error(f"Could not author the volume representation at {path}: {error}")
    try:
        import traceback

        if carb:
            carb.log_error(traceback.format_exc())
    except Exception:
        pass


def _log_warn(message: str) -> None:
    if carb:
        carb.log_warn(f"[hydragon.editor.core] {message}")


def _log_error(message: str) -> None:
    if carb:
        carb.log_error(f"[hydragon.editor.core] {message}")


def forget(path: str) -> None:
    """Drops the caches for a volume. Call when it is unregistered or removed."""
    derived = [
        path,
        f"{path}/volumes/force_bounds",
        f"{path}/volumes/kill_bounds",
        f"{path}/volumes/force_trigger",
        f"{path}/volumes/kill_trigger",
    ]
    for key in derived:
        _authored.pop(key, None)
        _visibility_applied.pop(key, None)
        _failures_reported.discard(key)


def current_stage():
    """The open USD stage, or None.

    Centralised because reaching for the stage through `omni.usd` is only valid inside Kit, and doing
    it inline scattered unguarded references through the volume systems - where a bare `except`
    turned the resulting NameError into a silent no-op.
    """
    if not HAS_KIT:
        return None
    try:
        usd_context = omni.usd.get_context()
        return usd_context.get_stage() if usd_context else None
    except Exception:
        return None


def notice_prim_paths(notice) -> set:
    """The PRIM paths touched by a USD ObjectsChanged notice.

    A notice reports property paths too - `.../ForceVolume.apiSchemas` is the one that matters when a
    schema is stamped onto an existing prim - and those are not prim paths, so each is trimmed to its
    prim. USD separates only the prim from the property with a dot, so the first dot is the boundary.
    """
    paths = set()
    for getter_name in ("GetResyncedPaths", "GetChangedInfoOnlyPaths"):
        getter = getattr(notice, getter_name, None)
        if getter is None:
            continue
        try:
            for path in getter():
                paths.add(str(path).split(".", 1)[0])
        except Exception:
            pass
    return paths


def touches_any(paths, registered_paths) -> bool:
    """Whether any changed path IS one of `registered_paths`, or lies under one."""
    for known in registered_paths:
        for changed in paths:
            if changed == known or changed.startswith(known + "/"):
                return True
    return False


def paths_include_volume(paths, is_volume_fn, stage=None) -> bool:
    """Whether any changed prim is itself a volume - i.e. newly created or newly stamped.

    This half is what makes a volume created into a stage with NOTHING registered yet show up.
    `touches_any` alone cannot see it: the new prim is contained by no registered path, so the system
    stayed deaf to the very first volume placed in a scene. The user-visible result was that
    `Create > Hydragon > Force Volume` produced a bare prim - payload applied, but no
    `volumes/force_bounds` and nothing under it in the hierarchy - and the wireframe only appeared
    once the timeline was started, because PLAY is what forced the next sync.

    `stage` may be supplied so this is testable without a live Kit session.
    """
    stage = stage if stage is not None else current_stage()
    if not stage:
        return False
    for changed in paths:
        try:
            prim = stage.GetPrimAtPath(changed)
            if prim and prim.IsValid() and is_volume_fn(prim):
                return True
        except Exception:
            continue
    return False


# ---------------------------------------------------------------------------------------------
# 1. Stage authoring
# ---------------------------------------------------------------------------------------------


def wireframe_path(volume_prim, volume_type: str) -> str:
    """Stage path of the volume's wireframe, whether or not the prim exists yet."""
    relative = volume_bounds.BOUNDS_PRIM_REL_PATH.get(volume_type, "volumes/force_bounds")
    return f"{volume_prim.GetPath().pathString}/{relative}"


def trigger_path(volume_prim, volume_type: str) -> str:
    """Stage path of the volume's trigger collider, whether or not the prim exists yet."""
    relative = TRIGGER_PRIM_REL_PATH.get(volume_type, "volumes/force_trigger")
    return f"{volume_prim.GetPath().pathString}/{relative}"


def _reference_frame_prim(volume_prim, volume_type: str):
    """The prim whose frame BOTH generated representations are expressed in.

    The wireframe is authored as points in its own frame, and it lives under the `volumes` group.
    Measuring in that same group frame is what makes the generated geometry land exactly on the
    measured region. Falls back to the volume root when the group is absent.
    """
    group = volume_prim.GetPrimAtPath("volumes")
    if group and group.IsValid():
        return group
    return volume_prim


def ensure_wireframe(volume_prim, volume_type: str) -> Optional[Any]:
    """Creates or updates the volume's BasisCurves wireframe. Returns the prim, or None on failure.

    Regenerated from the shape token plus the currently measured size, so it stays correct when the
    `volume:Shape` dropdown changes - which is what the old, never-called
    `update_wireframe_guide()` was for.

    Two things it deliberately does NOT do:

    * touch the prim's xformOp. The shipped asset offsets `force_bounds` with a translate and
      scales it, and that authored placement is the artist's, not ours. Since the points are
      generated around the origin in the prim's OWN frame, and the measurement is taken in that
      same own frame, the geometry lands where it already was.
    * author a schematic 2-vertex segment. `volume_geometry` emits 3 collinear points per segment
      because `linear` + `nonperiodic` requires `curveVertexCounts[i] > 2`; the retired generator
      wrote `[2] * 12` for a Box, which is invalid USD that only renders because USD is lenient.
    """
    if not HAS_KIT or not volume_prim or not volume_prim.IsValid():
        return None

    shape = volume_bounds.read_shape(volume_prim, volume_type)
    bounds_prim = volume_bounds.find_bounds_prim(volume_prim, volume_type)

    # Measure in the WIREFRAME'S OWN FRAME, which is the frame its points live in.
    if bounds_prim is not None and bounds_prim.IsValid():
        half_extents = volume_bounds.measure_relative_half_extents(
            volume_prim, volume_type, reference_prim=bounds_prim
        )
    else:
        half_extents = None

    if half_extents is None:
        # Nothing measurable yet: a brand new volume, or a wireframe that was just deleted. Use the
        # module default rather than collapsing the volume to nothing.
        half_extents = (volume_geometry.DEFAULT_HALF_EXTENT,) * 3

    half_extents, radius, half_height = volume_geometry.recover_shape_parameters(shape, half_extents)
    points, counts, extent = volume_geometry.build_shape_geometry(
        shape, half_extents=half_extents, radius=radius, half_height=half_height
    )

    path = wireframe_path(volume_prim, volume_type)
    fingerprint = (volume_type, shape, tuple(round(float(v), 4) for v in half_extents))
    existing = volume_prim.GetPrimAtPath(
        volume_bounds.BOUNDS_PRIM_REL_PATH.get(volume_type, "volumes/force_bounds")
    )
    if (
        _authored.get(path) == fingerprint
        and existing is not None
        and existing.IsValid()
        and existing.GetTypeName() == "BasisCurves"
    ):
        return existing

    stage = volume_prim.GetStage()
    if stage is None:
        return None

    # Authored into the ROOT layer on purpose: this geometry is meant to TRAVEL WITH THE STAGE, so
    # that another application opening the .usda sees the same volumes. Writing through whatever
    # edit target happens to be current would put it somewhere that is never saved.
    try:
        with Usd.EditContext(stage, Usd.EditTarget(stage.GetRootLayer())):
            curves = UsdGeom.BasisCurves.Define(stage, path)
            curves.GetTypeAttr().Set(UsdGeom.Tokens.linear)
            curves.GetWrapAttr().Set(UsdGeom.Tokens.nonperiodic)
            curves.GetPointsAttr().Set(Vt.Vec3fArray([Gf.Vec3f(*p) for p in points]) if Vt else points)
            curves.GetCurveVertexCountsAttr().Set(Vt.IntArray(counts) if Vt else counts)
            # A correct extent, not the previous one: an authored `extent` OVERRIDES points + widths
            # in a BBoxCache, so a stale one lies silently and the whole measurement goes wrong.
            curves.GetExtentAttr().Set(Vt.Vec3fArray([Gf.Vec3f(*p) for p in extent]) if Vt else extent)

            widths = curves.GetWidthsAttr()
            widths.Set(Vt.FloatArray([WIREFRAME_WIDTH]) if Vt else [WIREFRAME_WIDTH])
            widths.SetMetadata("interpolation", "constant")

            color = WIREFRAME_COLORS.get(volume_type, (1.0, 1.0, 1.0))
            display_color = curves.GetDisplayColorAttr()
            display_color.Set(Vt.Vec3fArray([Gf.Vec3f(*color)]) if Vt else [Gf.Vec3f(*color)])
            display_color.SetMetadata("interpolation", "constant")

            # `default` is what makes the wireframe both RENDER (so the artist sees it) and be
            # pickable (so clicking the cage selects the volume). `guide` would be invisible in the
            # final render but is filtered by the viewport's display-purpose setting, so it is not
            # reliable.
            curves.GetPurposeAttr().Set("default")
    except Exception as error:
        # Reported once and kept LOCAL to this prim: a single un-authorable volume must not stop the
        # others from being registered, nor take down the whole system at startup.
        _report_authoring_failure(path, error)
        return None

    _failures_reported.discard(path)
    _authored[path] = fingerprint
    return curves.GetPrim()


def ensure_trigger(volume_prim, volume_type: str) -> Optional[Any]:
    """Creates or updates the volume's PhysX trigger collider. Returns the prim, or None on failure.

    A trigger needs a real Gprim: PhysX cannot cook a collision volume from BasisCurves. The Gprim
    is chosen per shape by `volume_geometry.trigger_collider_spec` so that it reproduces exactly
    the analytic containment test it replaces, and it is placed at the MEASURED CENTRE because the
    wireframe is not necessarily centred on the volume root.

    Carries `UsdPhysics.CollisionAPI` (a trigger is a collider that reports instead of blocking)
    plus `PhysxSchema.PhysxTriggerAPI` (the marker that makes PhysX report the overlap).
    """
    if not HAS_KIT or not volume_prim or not volume_prim.IsValid():
        return None

    if PhysxSchema is None or UsdPhysics is None:
        _log_error(
            f"Cannot create a trigger collider for {volume_prim.GetPath()}: "
            "UsdPhysics/PhysxSchema are unavailable in this USD build."
        )
        return None

    reference = _reference_frame_prim(volume_prim, volume_type)
    shape = volume_bounds.read_shape(volume_prim, volume_type)
    half_extents, radius, half_height = volume_bounds.measure_shape_parameters(
        volume_prim, volume_type, reference_prim=reference
    )
    bound = volume_bounds.measure_bound(volume_prim, volume_type, reference_prim=reference)
    centre = tuple(bound[0]) if bound is not None else (0.0, 0.0, 0.0)
    spec = volume_geometry.trigger_collider_spec(shape, half_extents, radius, half_height)

    path = trigger_path(volume_prim, volume_type)
    existing = volume_prim.GetPrimAtPath(
        TRIGGER_PRIM_REL_PATH.get(volume_type, "volumes/force_trigger")
    )
    fingerprint = (
        volume_type,
        spec.gprim_type,
        tuple(round(float(v), 4) for v in spec.scale),
        tuple(round(float(v), 4) for v in centre),
    )
    if (
        _authored.get(path) == fingerprint
        and existing is not None
        and existing.IsValid()
        and existing.GetTypeName() == spec.gprim_type
    ):
        return existing

    stage = volume_prim.GetStage()
    if stage is None:
        return None

    try:
        with Usd.EditContext(stage, Usd.EditTarget(stage.GetRootLayer())):
            # A shape change means a different Gprim TYPE, and a prim's type cannot be swapped in
            # place without leaving the previous type's attributes behind. Recreate it instead.
            if (
                existing is not None
                and existing.IsValid()
                and existing.GetTypeName() != spec.gprim_type
            ):
                stage.RemovePrim(path)

            gprim = _define_gprim(stage, path, spec)
            if gprim is None:
                _log_error(f"Unsupported trigger Gprim type '{spec.gprim_type}' for {path}.")
                return None

            prim = gprim.GetPrim()
            author_trigger_transform(prim, centre, spec.scale)

            collision = UsdPhysics.CollisionAPI.Apply(prim)
            collision.CreateCollisionEnabledAttr().Set(True)
            PhysxSchema.PhysxTriggerAPI.Apply(prim)
            # The STATE API is what lets the systems read back which colliders are inside, as real
            # USD paths. It is applied alongside the trigger API, exactly as NVIDIA's own
            # TriggerStateAPIDemo does.
            PhysxSchema.PhysxTriggerStateAPI.Apply(prim)
            _apply_trigger_hiding(prim)
    except Exception as error:
        # See `_report_authoring_failure`: reported once, kept local to this prim.
        _report_authoring_failure(path, error)
        return None

    _failures_reported.discard(path)
    _authored[path] = fingerprint
    return prim


def author_trigger_transform(prim, centre, scale) -> None:
    """Authors the (translate, scale) pair that places a trigger collider on its wireframe.

    IDEMPOTENT BY CONSTRUCTION, and that matters more than it looks.

    `UsdGeomXformable::AddTranslateOp()` / `AddScaleOp()` do NOT hand back an existing op. When the op
    is already present in `xformOpOrder` they raise:

        The xformOp 'xformOp:translate' already exists in xformOpOrder
        [[xformOp:translate, xformOp:scale]].

    A trigger collider PERSISTS in the stage layer, so a second authoring pass over it hit exactly
    that - whether from an extension reload or from any edit that changes the volume's size. The
    exception then skipped the remainder of `ensure_trigger`, so the collision and trigger API
    schemas were never applied and the prim was never hidden, and it escaped the caller's `startup()`
    and took the whole system down.

    Clearing the op order first makes the pass repeatable. The underlying attributes are REUSED by
    `Add*Op`, so nothing accumulates in the layer and the layer stays clean on repeated saves.
    """
    xformable = UsdGeom.Xformable(prim)
    xformable.ClearXformOpOrder()
    translate_op = xformable.AddTranslateOp()
    translate_op.Set(Gf.Vec3d(float(centre[0]), float(centre[1]), float(centre[2])))
    scale_op = xformable.AddScaleOp()
    scale_op.Set(Gf.Vec3f(float(scale[0]), float(scale[1]), float(scale[2])))
    xformable.SetXformOpOrder([translate_op, scale_op])


def _define_gprim(stage, path: str, spec: volume_geometry.TriggerSpec):
    """Defines the Gprim named by a TriggerSpec and authors its intrinsic (unit) size."""
    if spec.gprim_type == "Sphere":
        sphere = UsdGeom.Sphere.Define(stage, path)
        sphere.GetRadiusAttr().Set(float(spec.radius))
        return sphere

    if spec.gprim_type == "Cylinder":
        cylinder = UsdGeom.Cylinder.Define(stage, path)
        cylinder.GetRadiusAttr().Set(float(spec.radius))
        cylinder.GetHeightAttr().Set(float(spec.height))
        # The wireframe generator builds cylinders around Y, so the collider must match.
        cylinder.GetAxisAttr().Set(UsdGeom.Tokens.y)
        return cylinder

    if spec.gprim_type == "Cube":
        cube = UsdGeom.Cube.Define(stage, path)
        cube.GetSizeAttr().Set(float(spec.size))
        return cube

    return None


def _apply_trigger_hiding(prim) -> None:
    """Keeps the trigger collider out of the viewport and out of the pick buffer."""
    imageable = UsdGeom.Imageable(prim)
    if TRIGGER_HIDING == "purpose":
        imageable.GetPurposeAttr().Set("guide")
        imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.inherited)
    else:
        # NOT rasterized => writes nothing to the pick buffer => no clickable area at all, so a
        # click inside the volume reaches whatever is inside it. The wireframe, a different prim,
        # remains the clickable handle for selecting the volume.
        imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)


def ensure_volume(volume_prim, volume_type: str) -> Tuple[Optional[Any], Optional[Any]]:
    """Authors both representations for a volume. Returns (wireframe_prim, trigger_prim).

    A failure to author the trigger is not silently tolerated: without it the volume has no
    gameplay effect at all, so it is reported as an error at the call site.
    """
    wireframe = ensure_wireframe(volume_prim, volume_type)
    trigger = ensure_trigger(volume_prim, volume_type)
    if wireframe is not None:
        apply_wireframe_visibility(wireframe, is_volumes_visible())
    return wireframe, trigger


# ---------------------------------------------------------------------------------------------
# 2. Trigger state
# ---------------------------------------------------------------------------------------------


def triggered_colliders(trigger_prim) -> set:
    """USD paths of the colliders currently overlapping a trigger volume.

    Read from PhysX's own state relationship rather than from trigger REPORTS, and that distinction
    is the whole point.

    A report identifies the actor with an opaque integer id, and in this build those ids are
    **Fabric handles**, not the `SdfPath`-encoded integers that `PhysicsSchemaTools.intToSdfPath`
    decodes. NVIDIA's own graph node proves the encoding: `OgnPhysXOnTriggerCollider.cpp` resolves
    them with `omni::fabric::handleToSdfPath(tdata.otherColliderPrimId)`. Decoding them with the
    SdfPath helper produced a non-empty but meaningless path, so every physics step asked PhysX for
    the transformation of a prim that does not exist:

        [Error] [omni.physx.plugin] SimulationInterface function could did not locate any objects
        at the specified path.
        [Error] [omni.physx.plugin] Error executing getRigidBodyTransformation.

    `PhysxTriggerStateAPI`'s `physxTrigger:triggeredCollisions` relationship carries real
    `SdfPath`s, so there is nothing to decode. NVIDIA's own `TriggerStateAPIDemo` reads it exactly
    this way.

    A snapshot also beats an edge for a second reason: a body ALREADY inside when the simulation
    starts never produces an ENTER report, but it is in the first snapshot. Callers derive enter and
    leave themselves by differencing consecutive snapshots.

    Returns an empty set when the prim has no state API, or before PhysX has stepped.
    """
    if (
        not HAS_KIT
        or PhysxSchema is None
        or trigger_prim is None
        or not trigger_prim.IsValid()
        or not hasattr(PhysxSchema, "PhysxTriggerStateAPI")
    ):
        return set()
    try:
        state_api = PhysxSchema.PhysxTriggerStateAPI(trigger_prim)
        if not state_api:
            return set()
        relationship = state_api.GetTriggeredCollisionsRel()
        if not relationship or not relationship.IsValid():
            return set()
        return {str(target) for target in relationship.GetTargets()}
    except Exception:
        return set()


def paths_match(reported: str, known: str) -> bool:
    """True when two USD paths refer to the same prim, or one contains the other.

    Trigger reports name the COLLIDER that entered, while the entity registries hold the RIGID
    BODY. Those are usually the same prim but not always - a collider can be parented under the
    body - so a prefix match in either direction is required rather than equality.
    """
    if not reported or not known:
        return False
    return (
        reported == known
        or reported.startswith(known + "/")
        or known.startswith(reported + "/")
    )


#: Faction label for a body that belongs to no registered entity. Such a body is only acted on by
#: a volume whose `filterFaction` is "All" or "RigidBodies".
UNKNOWN_FACTION = "Unknown"


def resolve_body_entity(reported_path: str) -> Tuple[str, Optional[str], Optional[str]]:
    """Maps a reported collider or body path to (faction, entity key, rigid body path).

    The registries live in the Player and Foes controllers, which is the in-memory entity index the
    rest of the engine uses - so this is a dictionary lookup, never a stage traversal.

    The match is TOLERANT in both directions because the two sides name different prims: the trigger
    relationship names COLLIDERS, while the registries hold RIGID BODIES, and in the common case one
    is a child of the other.

    Returning the rigid body path is what removes the need to decode anything: callers use the path
    the registry already knows, rather than resolving an opaque actor id into a path.

    An entity is only recognised if the engine has registered it. That is the engine's own ECS rule,
    and it also means a volume filtered to "RigidBodies" acts on registered entities rather than on
    every collider in the scene, including the ground.
    """
    if not reported_path:
        return UNKNOWN_FACTION, None, None

    try:
        from .player_controller import HydragonPlayerControllerSystem

        player_sys = HydragonPlayerControllerSystem.get_instance()
        if player_sys and player_sys.is_active_and_simulating():
            player_rb = player_sys.get_player_rb_path()
            if player_rb and paths_match(reported_path, player_rb):
                return "Player", player_rb, player_rb
    except Exception:
        pass

    try:
        from .foes_controller import HydragonFoesControllerSystem

        foes_sys = HydragonFoesControllerSystem.get_instance()
        if foes_sys and foes_sys.is_active_and_simulating():
            for foe_path, brain in getattr(foes_sys, "_active_brains", {}).items():
                if not brain or not brain.is_alive:
                    continue
                foe_rb = getattr(brain, "_rb_path", None)
                if foe_rb and paths_match(reported_path, foe_rb):
                    return "Enemy", foe_path, foe_rb
    except Exception:
        pass

    return UNKNOWN_FACTION, None, None


def faction_is_accepted(filter_faction: str, faction: str) -> bool:
    """Whether a volume configured with `filter_faction` should act on a body of `faction`.

    "RigidBodies" means every body PhysX reported, i.e. any collider that is part of a rigid body,
    as opposed to static scenery (which reports a body id of zero and never reaches this function
    with a path).
    """
    if filter_faction in ("All", "RigidBodies"):
        return True
    return filter_faction == faction


def resolve_inside_body(collider_path: str, filter_faction: str) -> Optional[Dict[str, Any]]:
    """Maps one reported collider to the registered entity that owns it, or None to ignore it.

    The trigger relationship names COLLIDERS while the registries hold RIGID BODIES, and those differ
    by a level in the common case. `resolve_body_entity` matches tolerantly and returns the rigid
    body path the registry already knows, so nothing has to be decoded - which is exactly the step
    that was failing: the actor ids in trigger REPORTS are Fabric handles, and decoding them with
    `PhysicsSchemaTools.intToSdfPath` produced a path that does not exist.

    Unregistered bodies are ignored. A volume filtered to "All" or "RigidBodies" therefore acts on
    registered entities rather than on every collider in the scene, which is what keeps the ground
    plane and the walls out, and it matches the engine's rule that entities live in a registry.
    """
    faction, entity_key, rb_path = resolve_body_entity(collider_path)
    if rb_path is None:
        return None
    if not faction_is_accepted(filter_faction, faction):
        return None
    return {"rb_path": rb_path, "faction": faction, "entity_key": entity_key}


# ---------------------------------------------------------------------------------------------
# 3. Display
# ---------------------------------------------------------------------------------------------

def is_volumes_visible() -> bool:
    """Current state of the Shift+V toggle. Defaults to visible."""
    if not HAS_KIT:
        return True
    settings = carb.settings.get_settings()
    value = settings.get(SETTING_SHOW_VOLUMES)
    if value is None:
        settings.set_bool(SETTING_SHOW_VOLUMES, True)
        return True
    return bool(settings.get_as_bool(SETTING_SHOW_VOLUMES))


def apply_wireframe_visibility(wireframe_prim, visible: bool) -> None:
    """Shows or hides a wireframe, in the SESSION layer.

    Session layer because this is DISPLAY state: it must not be saved into the artist's document,
    and it must not be able to affect physics. Only the wireframe is ever touched - never the
    trigger collider, which is invisible for a different reason and would otherwise disappear from
    the viewport the moment the artist hid the volumes.

    Skipped when the state is already what was asked for. That is not just an optimisation: this
    function is called during a geometry resync, and resyncs are triggered by USD edits, so writing
    unconditionally would feed the notice handler that invoked it.
    """
    if not HAS_KIT or wireframe_prim is None or not wireframe_prim.IsValid():
        return
    path = str(wireframe_prim.GetPath())
    if _visibility_applied.get(path) is visible:
        return
    stage = wireframe_prim.GetStage()
    if stage is None:
        return
    try:
        with Usd.EditContext(stage, Usd.EditTarget(stage.GetSessionLayer())):
            visibility = UsdGeom.Imageable(wireframe_prim).GetVisibilityAttr()
            if visible:
                visibility.Clear()
            else:
                visibility.Set(UsdGeom.Tokens.invisible)
        _visibility_applied[path] = visible
    except Exception as error:
        _log_warn(f"Could not apply wireframe visibility to {path}: {error}")


class VolumeDisplayToggle:
    """Owns the Shift+V / "Show By Type > Hydragon Volumes" toggle.

    Takes over the one genuinely useful piece of the retired `omni.ui.scene` overlay. The geometry
    is plain USD in the stage now, so this is deliberately small: it flips a setting, applies it to
    the wireframes, and registers the action, hotkey and menu entry around it.
    """

    _ACTION_EXT = "hydragon.editor.core"
    _ACTION_NAME = "toggle_volume_visibility"
    _HOTKEY = "Shift+V"

    def __init__(self):
        self._is_active = False
        self._setting_sub = None
        self._action = None
        self._menu_item = None
        self._hotkey_registered = False

    def startup(self) -> None:
        if not HAS_KIT:
            return
        self._is_active = True
        self._subscribe_setting()
        self._register_action_and_hotkey()
        self._register_viewport_menu_item()

    def shutdown(self) -> None:
        self._is_active = False
        if self._setting_sub and carb:
            try:
                carb.settings.get_settings().unsubscribe_to_change_events(self._setting_sub)
            except Exception:
                pass
        self._setting_sub = None
        self._deregister_viewport_menu_item()
        self._deregister_action_and_hotkey()

    def toggle(self) -> None:
        """Flips the setting. The change event applies it, so the hotkey and the menu agree."""
        if not HAS_KIT:
            return
        settings = carb.settings.get_settings()
        settings.set_bool(SETTING_SHOW_VOLUMES, not bool(settings.get_as_bool(SETTING_SHOW_VOLUMES)))

    def apply_to_stage(self) -> None:
        """Applies the current state to every volume wireframe on the open stage.

        Runs on a toggle change only - never per frame - so a single traversal is harmless here.
        """
        if not HAS_KIT:
            return
        stage = current_stage()
        if not stage:
            return

        visible = is_volumes_visible()
        try:
            wanted = {f"/{relative}" for relative in volume_bounds.BOUNDS_PRIM_REL_PATH.values()}
            for prim in stage.Traverse():
                if not prim.IsValid() or prim.GetTypeName() != "BasisCurves":
                    continue
                if any(str(prim.GetPath()).endswith(suffix) for suffix in wanted):
                    apply_wireframe_visibility(prim, visible)
        except Exception as error:
            _log_warn(f"Could not apply the volume visibility toggle: {error}")

    def _subscribe_setting(self) -> None:
        settings = carb.settings.get_settings()
        if settings.get(SETTING_SHOW_VOLUMES) is None:
            settings.set_bool(SETTING_SHOW_VOLUMES, True)
        self._setting_sub = settings.subscribe_to_node_change_events(
            SETTING_SHOW_VOLUMES, self._on_visibility_changed
        )

    def _on_visibility_changed(self, item, event_type) -> None:
        if self._is_active:
            self.apply_to_stage()

    def _register_action_and_hotkey(self) -> None:
        try:
            import omni.kit.actions.core as kit_actions

            action_reg = kit_actions.get_action_registry()
            if action_reg and hasattr(action_reg, "register_action"):
                self._action = action_reg.register_action(
                    self._ACTION_EXT,
                    self._ACTION_NAME,
                    self.toggle,
                    display_name="Show/Hide Hydragon Volumes",
                    description="Toggles visibility of the Hydragon Force and Kill volume wireframes.",
                )
        except Exception as error:
            _log_warn(f"Could not register the volume visibility action: {error}")

        try:
            import omni.kit.hotkeys.core as kit_hotkeys

            hotkey_reg = kit_hotkeys.get_hotkey_registry()
            if hotkey_reg and hasattr(hotkey_reg, "register_hotkey"):
                hotkey_reg.register_hotkey(
                    self._ACTION_EXT, self._HOTKEY, self._ACTION_EXT, self._ACTION_NAME
                )
                self._hotkey_registered = True
        except Exception as error:
            _log_warn(f"Could not register the '{self._HOTKEY}' hotkey: {error}")

    def _deregister_action_and_hotkey(self) -> None:
        if self._hotkey_registered:
            try:
                import omni.kit.hotkeys.core as kit_hotkeys

                hotkey_reg = kit_hotkeys.get_hotkey_registry()
                if hotkey_reg and hasattr(hotkey_reg, "deregister_hotkey"):
                    hotkey_reg.deregister_hotkey(self._ACTION_EXT, self._HOTKEY)
            except Exception:
                pass
            self._hotkey_registered = False

        if self._action:
            try:
                import omni.kit.actions.core as kit_actions

                action_reg = kit_actions.get_action_registry()
                if action_reg and hasattr(action_reg, "deregister_action"):
                    action_reg.deregister_action(self._ACTION_EXT, self._ACTION_NAME)
            except Exception:
                pass
            self._action = None

    def _register_viewport_menu_item(self) -> None:
        try:
            import omni.kit.viewport.menubar.display as vp_display
            from omni.kit.viewport.menubar.core import CategoryStateItem

            display_ext = vp_display.get_instance()
            if display_ext and hasattr(display_ext, "register_custom_category_item"):
                self._menu_item = CategoryStateItem(
                    "Hydragon Volumes",
                    setting_path=SETTING_SHOW_VOLUMES,
                    hotkey_text=self._HOTKEY,
                )
                display_ext.register_custom_category_item("Show By Type", self._menu_item)
        except Exception as error:
            _log_warn(f"Could not register 'Hydragon Volumes' in the viewport menu: {error}")

    def _deregister_viewport_menu_item(self) -> None:
        if not self._menu_item:
            return
        try:
            import omni.kit.viewport.menubar.display as vp_display

            display_ext = vp_display.get_instance()
            if display_ext and hasattr(display_ext, "deregister_custom_category_item"):
                display_ext.deregister_custom_category_item("Show By Type", self._menu_item)
        except Exception:
            pass
        self._menu_item = None
