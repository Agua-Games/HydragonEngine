# =============================================================================
# water_body_controller.py
# A body of water as a physics medium: buoyancy, drag, and standing on it.
#
# The single most important decision here: **a water body authors NO collider.**
#
# Containment is analytic - a plane, plus a box when the mode is Bounded - so the
# water costs no PhysX cooking, no geometry, and above all it does not appear in
# any scene query.  That matters because a scene query cannot be filtered (see
# `docs/omniverse_architecture/collision_triggers_and_scene_queries.md`): a large
# water collider would be hit by the camera's anti-clipping probe, by the player
# ground check, and by the foe ground check, and the first of those collapses the
# camera to its minimum distance the moment the player is inside the cage.  That
# bug has already been paid for once with the force volumes; there is no reason to
# buy it again for a body of water, whose containment test is trivial.
#
# The second decision: buoyancy is applied as a VELOCITY correction, not a force.
# A rigid body's vertical acceleration under gravity and buoyancy is independent
# of its mass, and this engine already writes velocities to implement damping
# ("damping is applied by writing velocities rather than by PhysX damping
# attributes, so that it stops the instant the body leaves the volume").  Doing
# the same avoids needing a mass, a density in engine units, or a force-unit
# conversion, none of which this repository can verify.
# =============================================================================

from __future__ import annotations

from typing import Any, Dict, Optional, Tuple

try:
    import carb
    import omni.kit.app
    import omni.timeline
    import omni.usd
    from pxr import Gf, Sdf, Tf, Usd, UsdGeom
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    Gf = None
    Sdf = None
    Tf = None
    Usd = None
    UsdGeom = None

from .schemas import HydragonWaterBody

try:
    from . import volume_bounds
    HAS_VOLUME_BOUNDS = True
except Exception:  # noqa: BLE001
    volume_bounds = None
    HAS_VOLUME_BOUNDS = False

try:
    from .force_volume_controller import get_physx_interface, get_physx_simulation_interface
    HAS_PHYSX = True
except Exception:  # noqa: BLE001
    get_physx_interface = None
    get_physx_simulation_interface = None
    HAS_PHYSX = False

from .ocean_controller import HydragonOceanSystem, paths_include_schema

#: Gravity in the engine's units.  The stage is in centimetres (metersPerUnit
#: 0.01), so this is 9.81 m/s^2 expressed in cm/s^2.  Using 9.81 here would make
#: every body a hundred times too heavy under water.
GRAVITY_CM_PER_S2 = 981.0

#: A body is treated as dry once less than this fraction of it is submerged, so
#: that a fleck of the collider clipping the surface does not apply drag.
DRY_FRACTION = 0.02

#: Child prim whose measured bounds define a Bounded water body.
BOUND_PRIM_NAME = "water_volume_bound"

#: Child prim whose transform carries the still-water level.
SURFACE_PRIM_NAME = "water_volume_infinite"


def _log_info(message: str) -> None:
    if carb:
        carb.log_info(f"[hydragon.water] {message}")


def _log_warn(message: str) -> None:
    if carb:
        carb.log_warn(f"[hydragon.water] {message}")


def _log_error(message: str) -> None:
    if carb:
        carb.log_error(f"[hydragon.water] {message}")


def _world_transform(prim):
    """World matrix of a prim, or None outside Kit."""
    if not HAS_KIT or not prim:
        return None
    try:
        return UsdGeom.Xformable(prim).ComputeLocalToWorldTransform(
            Usd.TimeCode.Default()
        )
    except Exception:  # noqa: BLE001
        return None


class HydragonWaterBodyZone:
    """One body of water: where it is, how high it stands, and what it does to a body."""

    def __init__(self, prim):
        self._prim = prim
        self._path = str(prim.GetPath()) if prim else ""
        self._body = HydragonWaterBody(prim)
        self._world_y = 0.0
        self._min_x = self._max_x = 0.0
        self._min_z = self._max_z = 0.0
        self._has_bounds = False

    # -- identity -------------------------------------------------------------

    @property
    def prim(self):
        return self._prim

    @property
    def path(self) -> str:
        return self._path

    @property
    def is_enabled(self) -> bool:
        return self._body.is_enabled

    @property
    def mode(self) -> str:
        return self._body.mode

    @property
    def linear_drag(self) -> float:
        return self._body.linear_drag

    @property
    def angular_drag(self) -> float:
        return self._body.angular_drag

    @property
    def buoyancy_scale(self) -> float:
        return self._body.buoyancy_scale

    @property
    def grounded_tolerance(self) -> float:
        return self._body.grounded_tolerance

    @property
    def can_jump_off_surface(self) -> bool:
        return self._body.can_jump_off_surface

    def describe(self) -> Dict[str, Any]:
        return {
            "path": self._path,
            "enabled": self.is_enabled,
            "mode": self.mode,
            "waterline_y": self._world_y,
            "bounded": self._has_bounds,
            "density": self._body.density,
            "buoyancy_scale": self._body.buoyancy_scale,
        }

    # -- geometry -------------------------------------------------------------

    def refresh(self) -> bool:
        """Re-measure the waterline and the bounds from the prim hierarchy.

        The waterline is the world Y of `water_volume_infinite`, so moving that
        child moves the water.  Bounds come from `volume_bounds.measure_bound`,
        the same measurement the force and kill volumes use, rather than from a
        new attribute: the region is authored by scaling a prim, as everywhere
        else in this engine.
        """
        if not HAS_KIT or not self._prim:
            return False

        offset_cm = self._body.surface_offset * 100.0

        surface_prim = self._prim.GetChild(SURFACE_PRIM_NAME)
        matrix = _world_transform(surface_prim) if surface_prim else _world_transform(self._prim)
        if matrix is None:
            matrix = _world_transform(self._prim)
        if matrix is None:
            return False
        self._world_y = float(matrix.ExtractTranslation()[1]) + offset_cm

        self._has_bounds = False
        if self.mode == "Bounded" and HAS_VOLUME_BOUNDS:
            bound_prim = self._prim.GetChild(BOUND_PRIM_NAME)
            if bound_prim and bound_prim.IsValid():
                try:
                    measured = volume_bounds.measure_bound(bound_prim)
                except Exception as exc:  # noqa: BLE001
                    _log_warn(f"{self._path}: could not measure the water bounds: {exc}")
                    measured = None
                if measured is not None:
                    centre, _half_extents = measured
                    bound_matrix = _world_transform(bound_prim)
                    if bound_matrix is not None:
                        world_centre = bound_matrix.Transform(Gf.Vec3d(*centre))
                        half = Gf.Vec3d(*_half_extents)
                        # The box is axis aligned in the bound prim's frame, which
                        # is the frame the artist scales.  Deriving its extent in
                        # world space keeps the test cheap: a rotated water body
                        # would need the full inverse transform per query.
                        corners = []
                        for sx in (-1.0, 1.0):
                            for sy in (-1.0, 1.0):
                                for sz in (-1.0, 1.0):
                                    local = Gf.Vec3d(
                                        sx * half[0], sy * half[1], sz * half[2]
                                    )
                                    corners.append(bound_matrix.Transform(local))
                        self._min_x = min(c[0] for c in corners)
                        self._max_x = max(c[0] for c in corners)
                        self._min_z = min(c[2] for c in corners)
                        self._max_z = max(c[2] for c in corners)
                        self._has_bounds = True
                        del world_centre
        return True

    def contains_xz(self, world_x: float, world_z: float) -> bool:
        """Whether a world position is inside the water body horizontally."""
        if self.mode != "Bounded" or not self._has_bounds:
            return True
        return self._min_x <= world_x <= self._max_x and self._min_z <= world_z <= self._max_z

    # -- surface --------------------------------------------------------------

    def still_water_y(self) -> float:
        """The flat waterline in world units, before any waves."""
        return self._world_y

    def surface_y(self, world_x: float, world_z: float) -> float:
        """Water surface height at a world position, waves included.

        Waves come from whichever ocean patch covers the position; with no ocean
        the body is a still plane, which is a perfectly reasonable pond, river or
        swimming pool.
        """
        base = self._world_y
        system = HydragonOceanSystem.get_instance()
        if system is None:
            return base
        height = system.sample_height(world_x, world_z)
        if height is None:
            return base
        # The ocean reports metres; the stage is centimetres.
        return base + height * 100.0

    def submerged_fraction(self, world_pos, radius: float) -> float:
        """Fraction of a sphere at `world_pos` that is below the surface, 0 to 1.

        A sphere cap, so a body half in the water gets exactly one half and a body
        that has just touched gets almost nothing.  The previous generation of
        gameplay volumes used a binary inside/outside test, which makes a body
        snap between full gravity and full buoyancy at the waterline; the cap is
        what makes entering the water look like entering water.
        """
        if radius <= 0.0:
            return 0.0
        x, y, z = float(world_pos[0]), float(world_pos[1]), float(world_pos[2])
        if not self.contains_xz(x, z):
            return 0.0

        surface = self.surface_y(x, z)
        depth = surface - y
        if depth <= -radius:
            return 0.0
        if depth >= radius:
            return 1.0

        # Volume of the sphere below the plane, normalised by the whole sphere.
        # d is the distance from the centre down to the plane, in [-r, r].
        d = depth
        volume = (3.141592653589793 * (radius + d) ** 2 * (2.0 * radius - d)) / 3.0
        whole = (4.0 / 3.0) * 3.141592653589793 * radius ** 3
        return max(0.0, min(1.0, volume / whole))

    # -- forces ---------------------------------------------------------------

    def compute_forces(
        self,
        world_pos,
        linear_vel,
        angular_vel,
        delta_seconds: float,
        radius: float,
        buoyancy_scale: float,
    ) -> Tuple[Tuple[float, float, float], Tuple[float, float, float]]:
        """Return (linear velocity, angular velocity) for a submerged body.

        Gravity itself is left to PhysX.  What is added here is the buoyant
        acceleration, `g * fraction * buoyancyScale`, which is the fraction of
        gravity the displaced water pushes back with.  `buoyancyScale` is
        therefore the ratio of water density to body density: 1.0 is neutral, and
        above 1.0 the body rises.  Drag is the same idea applied to both
        velocities, scaled by how much of the body is actually wet.

        Note the asymmetry that makes this behave: drag is scaled by the
        submerged fraction, so a body skipping across the surface is barely
        slowed, while a body under the water is stopped.
        """
        if delta_seconds <= 0.0:
            return tuple(linear_vel), tuple(angular_vel)

        fraction = self.submerged_fraction(world_pos, radius)
        if fraction <= DRY_FRACTION:
            return tuple(linear_vel), tuple(angular_vel)

        lift = GRAVITY_CM_PER_S2 * fraction * buoyancy_scale * delta_seconds
        linear = [
            float(linear_vel[0]),
            float(linear_vel[1]) + lift,
            float(linear_vel[2]),
        ]

        # Exponential damping, so the result is independent of the step size.
        linear_drag = min(max(self.linear_drag, 0.0), 1.0) * fraction
        angular_drag = min(max(self.angular_drag, 0.0), 1.0) * fraction
        linear_factor = max(0.0, 1.0 - linear_drag)
        angular_factor = max(0.0, 1.0 - angular_drag)

        linear = [component * linear_factor for component in linear]
        angular = [float(angular_vel[index]) * angular_factor for index in range(3)]
        return (linear[0], linear[1], linear[2]), (angular[0], angular[1], angular[2])

    def is_resting_on_surface(self, world_pos, radius: float) -> bool:
        """Whether a body is close enough to the surface to count as standing on it.

        The tolerance is measured against the body's own radius as well as the
        schema value, because a large body penetrates further before it is
        actually floating.
        """
        x, y, z = float(world_pos[0]), float(world_pos[1]), float(world_pos[2])
        if not self.contains_xz(x, z):
            return False
        surface = self.surface_y(x, z)
        # The body centre sits one radius above the surface when it floats.
        tolerance = self.grounded_tolerance + radius
        return abs(y - (surface + radius)) <= tolerance


class HydragonWaterBodySystem:
    """Discovers water bodies once and applies them on every physics step."""

    _instance: Optional["HydragonWaterBodySystem"] = None

    def __init__(self):
        self._is_active = False
        self._is_simulating = False
        self._zones: Dict[str, HydragonWaterBodyZone] = {}
        self._sync_pending = False
        #: Paths named by the last stage notices, not yet judged.  See
        #: `_evaluate_notice_paths` for why the judgement cannot happen in the
        #: notice itself.
        self._notice_paths: set = set()
        self._last_dt = 1.0 / 60.0
        self._stage_id = 0
        self._timeline_sub = None
        self._physics_step_sub = None
        self._app_update_sub = None
        self._notice = None
        self._failures_reported = set()
        self._radius_cache: Dict[str, float] = {}

    # -- plumbing -------------------------------------------------------------

    @classmethod
    def get_instance(cls) -> Optional["HydragonWaterBodySystem"]:
        return cls._instance

    @property
    def zone_count(self) -> int:
        return len(self._zones)

    def startup(self) -> None:
        HydragonWaterBodySystem._instance = self
        self._is_active = True
        if not HAS_KIT:
            return

        try:
            timeline = omni.timeline.get_timeline_interface()
            self._timeline_sub = timeline.get_timeline_event_stream().create_subscription_to_pop(
                self._on_timeline_event, name="HydragonWaterTimeline"
            )
        except Exception as exc:  # noqa: BLE001
            _log_warn(f"could not subscribe to the timeline: {exc}")

        try:
            physx_iface = get_physx_interface() if HAS_PHYSX else None
            if physx_iface and hasattr(physx_iface, "subscribe_physics_step_events"):
                self._physics_step_sub = physx_iface.subscribe_physics_step_events(
                    self._on_physics_step
                )
        except Exception as exc:  # noqa: BLE001
            _log_warn(f"could not subscribe to physics steps: {exc}")

        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                self._on_app_update, name="HydragonWaterSync"
            )
        except Exception as exc:  # noqa: BLE001
            _log_warn(f"could not subscribe to the update stream: {exc}")

        self._register_objects_changed_notice()
        self._sync_zones_now()

    def shutdown(self) -> None:
        self._unregister_objects_changed_notice()
        self._zones.clear()
        self._timeline_sub = None
        self._physics_step_sub = None
        self._app_update_sub = None
        self._sync_pending = False
        self._notice_paths = set()
        self._is_active = False
        if HydragonWaterBodySystem._instance is self:
            HydragonWaterBodySystem._instance = None

    # -- discovery ------------------------------------------------------------

    def _current_stage(self):
        if not HAS_KIT:
            return None
        try:
            return omni.usd.get_context().get_stage()
        except Exception:  # noqa: BLE001
            return None

    def _register_objects_changed_notice(self) -> None:
        stage = self._current_stage()
        if not stage or not Tf:
            return
        try:
            self._notice = Tf.Notice.Register(
                Usd.Notice.ObjectsChanged, self._on_objects_changed, stage
            )
            self._stage_id = stage.GetStageId() if hasattr(stage, "GetStageId") else 0
        except Exception as exc:  # noqa: BLE001
            _log_warn(f"could not register the stage notice: {exc}")

    def _unregister_objects_changed_notice(self) -> None:
        if self._notice is not None:
            try:
                self._notice.Revoke()
            except Exception:  # noqa: BLE001
                pass
        self._notice = None

    def _evaluate_notice_paths(self, paths: set, stage=None) -> bool:
        """Whether a batch of changed paths means the registry must be rebuilt.

        NOT called from inside the notice handler: `AddPayload` authors the arc
        before the payload composes, so a freshly created water body is still a
        bare Xform at notice time and judging it there answers "not a water
        body".  See the ocean system's version of this for the full note.
        """
        if not paths:
            return False
        stage = stage or self._current_stage()
        if paths_include_schema(paths, HydragonWaterBody.is_applied, stage):
            return True
        for registered in self._zones:
            for path in paths:
                if path == registered or path.startswith(registered + "/"):
                    return True
        return False

    def _on_objects_changed(self, notice, sender) -> None:
        """Record the changed paths; judge them on the next frame.

        `sender` is REQUIRED.  `Tf.Notice.Register` calls the handler with
        `(notice, sender)`, so declaring only `notice` makes every notice raise
        TypeError and the handler never runs - the units would simply never be
        discovered, silently.  The force and kill volume systems declare the same
        two parameters for exactly this reason.
        """
        try:
            if not self._is_active:
                return
            paths = set()
            for path in list(notice.GetResyncedPaths()) + list(notice.GetChangedInfoOnlyPaths()):
                paths.add(str(path).split(".", 1)[0])
            if paths:
                self._notice_paths |= paths
        except Exception as exc:  # noqa: BLE001
            self._report_once("notice", f"stage notice handler failed: {exc}")

    def _sync_zones_now(self) -> None:
        stage = self._current_stage()
        if not stage:
            return

        seen = set()
        try:
            for prim in stage.Traverse():
                if not prim.IsActive():
                    continue
                if not HydragonWaterBody.is_applied(prim):
                    continue
                path = str(prim.GetPath())
                seen.add(path)
                zone = self._zones.get(path)
                if zone is None:
                    zone = HydragonWaterBodyZone(prim)
                    self._zones[path] = zone
                zone.refresh()
        except Exception as exc:  # noqa: BLE001
            _log_error(f"water body discovery failed: {exc}")

        for stale in [p for p in self._zones if p not in seen]:
            self._zones.pop(stale)

        if self._zones:
            _log_info(f"tracking {len(self._zones)} water body(ies)")

    def _report_once(self, key: str, message: str) -> None:
        if key in self._failures_reported:
            return
        self._failures_reported.add(key)
        _log_error(message)

    # -- callbacks ------------------------------------------------------------

    def _on_timeline_event(self, event) -> None:
        if not HAS_KIT:
            return
        try:
            event_type = event.type
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                self._is_simulating = True
                self._sync_pending = True
            elif event_type in (
                int(omni.timeline.TimelineEventType.STOP),
                int(omni.timeline.TimelineEventType.PAUSE),
            ):
                self._is_simulating = False
        except Exception as exc:  # noqa: BLE001
            _log_error(f"timeline handler failed: {exc}")

    def _on_app_update(self, event) -> None:
        try:
            if not self._is_active:
                return
            # Judged one frame after the notice, never inside it - see
            # `_evaluate_notice_paths`.
            if self._notice_paths:
                paths = self._notice_paths
                self._notice_paths = set()
                if self._evaluate_notice_paths(paths):
                    self._sync_pending = True
            if self._sync_pending:
                self._sync_pending = False
                self._sync_zones_now()
                for zone in self._zones.values():
                    zone.refresh()
        except Exception as exc:  # noqa: BLE001
            self._report_once("update", f"water body sync failed: {exc}")

    def _on_physics_step(self, delta_seconds: float) -> None:
        """Apply the water to every registered rigid body.

        The registries this needs already exist: the player and foe controllers
        hold `Dict[path, rigid body]` and `volume_triggers.resolve_body_entity`
        maps a reported path back to one of them.  Nothing here traverses the
        stage.
        """
        if not HAS_KIT or not self._is_active or not self._zones:
            return
        try:
            self._last_dt = float(delta_seconds) if delta_seconds else self._last_dt
            body_paths = self._tracked_body_paths()
            if not body_paths:
                return
            for zone in self._zones.values():
                if not zone.is_enabled:
                    continue
                for body_path in body_paths:
                    state = self._body_state(body_path)
                    if state is None or state["radius"] <= 0.0:
                        continue
                    linear, angular = zone.compute_forces(
                        state["world_pos"],
                        state["linear_vel"],
                        state["angular_vel"],
                        self._last_dt,
                        state["radius"],
                        zone.buoyancy_scale,
                    )
                    self._write_velocities(body_path, linear, angular)
        except Exception as exc:  # noqa: BLE001
            import traceback

            self._report_once(
                "physics", f"water body step failed, disabled:\n{traceback.format_exc()}"
            )
            self._is_active = False

    # -- bodies ---------------------------------------------------------------

    def _tracked_body_paths(self) -> list:
        """Rigid body paths the water should act on.

        Read from the systems that already own them, rather than by traversing the
        stage.  The player controller publishes its rigid body path, which is the
        same value the force volumes resolve their reports against.
        """
        paths = []
        try:
            from .player_controller import HydragonPlayerControllerSystem

            system = HydragonPlayerControllerSystem.get_instance()
            if system is not None:
                path = system.get_player_rb_path()
                if path:
                    paths.append(path)
        except Exception:  # noqa: BLE001 - the water is not essential to the player
            pass
        # Foes are deliberately NOT included yet.  Adding them means asking the
        # foes controller for its registry; until then, only the player is
        # affected, and that is stated rather than implied.
        return paths

    def _body_state(self, body_path: str) -> Optional[Dict[str, Any]]:
        """Position, radius and velocities of a rigid body, read from PhysX.

        `get_rigidbody_transformation` returns the body's own transform, which is
        its centre of mass - the point AGENTS.md section D requires work to be
        applied at, and the point a sphere-cap submersion test needs.
        """
        physx_iface = get_physx_interface() if HAS_PHYSX else None
        if not physx_iface or not body_path:
            return None

        try:
            transform = physx_iface.get_rigidbody_transformation(body_path)
            position = None
            if isinstance(transform, dict):
                position = transform.get("position")
            elif transform is not None and hasattr(transform, "position"):
                position = transform.position
            if position is None:
                return None

            linear = (0.0, 0.0, 0.0)
            angular = (0.0, 0.0, 0.0)
            for getter in ("get_rigidbody_linear_velocity", "get_linear_velocity"):
                if hasattr(physx_iface, getter):
                    value = getattr(physx_iface, getter)(body_path)
                    if value is not None:
                        linear = (float(value[0]), float(value[1]), float(value[2]))
                        break
            for getter in ("get_rigidbody_angular_velocity", "get_angular_velocity"):
                if hasattr(physx_iface, getter):
                    value = getattr(physx_iface, getter)(body_path)
                    if value is not None:
                        angular = (float(value[0]), float(value[1]), float(value[2]))
                        break

            return {
                "world_pos": (float(position[0]), float(position[1]), float(position[2])),
                "linear_vel": linear,
                "angular_vel": angular,
                "radius": self._body_radius(body_path),
            }
        except Exception:  # noqa: BLE001
            return None

    def _body_radius(self, body_path: str) -> float:
        """Radius of a body, from its measured bound.

        Uses the same measurement helper the force and kill volumes use, so a
        stale authored `extent` cannot silently lie here in a way it would not
        elsewhere.  The measurement is cached, because it is a BBoxCache build and
        this runs on every body on every physics step.
        """
        cached = self._radius_cache.get(body_path)
        if cached is not None:
            return cached
        radius = self._measure_radius(body_path)
        if radius > 0.0:
            self._radius_cache[body_path] = radius
        return radius

    def _measure_radius(self, body_path: str) -> float:
        if not HAS_KIT or not HAS_VOLUME_BOUNDS or not body_path:
            return 0.0
        stage = self._current_stage()
        if not stage:
            return 0.0
        prim = stage.GetPrimAtPath(body_path)
        if not prim or not prim.IsValid():
            return 0.0
        try:
            measured = volume_bounds.measure_bound(prim, reference_prim=prim)
        except Exception:  # noqa: BLE001
            return 0.0
        if measured is None:
            return 0.0
        _centre, half_extents = measured
        return float(max(half_extents))

    def _write_velocities(self, body_path: str, linear, angular) -> None:
        physx_iface = get_physx_interface() if HAS_PHYSX else None
        if not physx_iface:
            return
        try:
            for setter in ("set_rigidbody_linear_velocity", "set_linear_velocity"):
                if hasattr(physx_iface, setter):
                    getattr(physx_iface, setter)(
                        body_path, carb.Float3(linear[0], linear[1], linear[2])
                    )
                    break
            for setter in ("set_rigidbody_angular_velocity", "set_angular_velocity"):
                if hasattr(physx_iface, setter):
                    getattr(physx_iface, setter)(
                        body_path, carb.Float3(angular[0], angular[1], angular[2])
                    )
                    break
        except Exception:  # noqa: BLE001
            pass

    # -- queries for the controllers ------------------------------------------

    def surface_y(self, world_x: float, world_z: float) -> Optional[float]:
        """Highest water surface at a world position across all water bodies."""
        best: Optional[float] = None
        for zone in self._zones.values():
            if not zone.is_enabled or not zone.contains_xz(world_x, world_z):
                continue
            height = zone.surface_y(world_x, world_z)
            if best is None or height > best:
                best = height
        return best

    def is_resting_on_water(self, world_pos, radius: float) -> bool:
        """Whether a body counts as standing on any water surface.

        Exposed so the player and foe controllers can ask instead of duplicating
        the rule.  Nothing here touches a scene query, which is the whole point.
        """
        for zone in self._zones.values():
            if not zone.is_enabled:
                continue
            if zone.is_resting_on_surface(world_pos, radius):
                return True
        return False

    def can_jump_from(self, world_pos, radius: float) -> bool:
        for zone in self._zones.values():
            if not zone.is_enabled or not zone.can_jump_off_surface:
                continue
            if zone.is_resting_on_surface(world_pos, radius):
                return True
        return False

    def sync_now(self) -> None:
        self._sync_zones_now()
        for zone in self._zones.values():
            zone.refresh()
