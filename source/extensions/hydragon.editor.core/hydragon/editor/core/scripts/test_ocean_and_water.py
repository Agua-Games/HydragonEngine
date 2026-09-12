# =============================================================================
# test_ocean_and_water.py
# Headless checks for the ocean entity and the water body medium.
#
# These cover the parts that are pure logic and therefore testable with no Kit,
# no stage and no PhysX: containment, the sphere-cap submersion test, the
# buoyancy/drag response, and the resting-on-water rule.  Everything that needs a
# live stage (discovery, notices, mesh authoring, texture publication) is
# exercised by the in-editor harness instead, not here.
#
# Run:
#   _build\target-deps\python\python.exe ^
#     source\extensions\hydragon.editor.core\hydragon\editor\core\scripts\test_ocean_and_water.py
# =============================================================================

import inspect
import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))


def _find_repo_root(start: str) -> str:
    current = start
    while True:
        if os.path.isdir(os.path.join(current, "_build")):
            return current
        parent = os.path.dirname(current)
        if parent == current:
            raise RuntimeError(f"could not locate the repository root above {start}")
        current = parent


_ROOT = _find_repo_root(_HERE)
sys.path.insert(0, os.path.join(_ROOT, "source", "extensions", "hydragon.editor.core"))

from hydragon.editor.core import ocean_controller as ocean  # noqa: E402
from hydragon.editor.core import water_body_controller as water  # noqa: E402
from hydragon.editor.core.schemas import HydragonWaterBody  # noqa: E402

FAILURES = []
CHECKS = [0]


def check(label: str, condition: bool, detail: str = "") -> None:
    CHECKS[0] += 1
    if not condition:
        FAILURES.append(label if not detail else f"{label} ({detail})")
    print(f"  [{'ok' if condition else 'FAIL'}] {label}" + (f" :: {detail}" if detail else ""))


def section(title: str) -> None:
    print(f"\n=== {title} ===")


def check_and_stop(label: str, condition: bool, detail: str = "") -> bool:
    """Report a check whose failure makes everything after it meaningless."""
    check(label, condition, detail)
    return condition


def make_zone(**attributes):
    """A water body with no Kit: the waterline sits at y = 0."""
    body = HydragonWaterBody(None)
    for name, value in attributes.items():
        setattr(body, name, value)
    zone = water.HydragonWaterBodyZone(None)
    zone._body = body
    return zone


# -----------------------------------------------------------------------------
# 1. Containment
# -----------------------------------------------------------------------------


def test_containment() -> None:
    section("1. Containment")

    infinite = make_zone(mode="Infinite")
    check("an infinite body contains any position", infinite.contains_xz(12345.0, -6789.0))

    bounded = make_zone(mode="Bounded")
    # With no measured bounds yet, a Bounded body must not silently accept
    # everything: that would turn a local pond into a global ocean.
    check(
        "a bounded body with no measured bounds contains nothing",
        bounded.contains_xz(0.0, 0.0),
    )
    bounded._has_bounds = True
    bounded._min_x, bounded._max_x = -100.0, 100.0
    bounded._min_z, bounded._max_z = -50.0, 50.0
    check("inside the bounds is contained", bounded.contains_xz(0.0, 0.0))
    check("outside on x is rejected", not bounded.contains_xz(101.0, 0.0))
    check("outside on z is rejected", not bounded.contains_xz(0.0, 51.0))
    check("edges count as inside", bounded.contains_xz(-100.0, 50.0))


# -----------------------------------------------------------------------------
# 2. Submersion, the sphere cap
# -----------------------------------------------------------------------------


def test_submerged_fraction() -> None:
    section("2. Submersion (sphere cap)")

    zone = make_zone()
    radius = 100.0

    above = zone.submerged_fraction((0.0, radius + 1.0, 0.0), radius)
    check("a body clear of the water is dry", above == 0.0, f"{above}")

    touching = zone.submerged_fraction((0.0, radius, 0.0), radius)
    check("a body just touching is almost dry", 0.0 <= touching < 0.02, f"{touching}")

    half = zone.submerged_fraction((0.0, 0.0, 0.0), radius)
    check(
        "a body centred on the surface is half submerged",
        abs(half - 0.5) < 1.0e-6,
        f"{half:.6f}",
    )

    full = zone.submerged_fraction((0.0, -radius, 0.0), radius)
    check("a body fully under is fully submerged", full == 1.0, f"{full}")

    deeper = zone.submerged_fraction((0.0, -radius * 5.0, 0.0), radius)
    check("going deeper saturates at one", deeper == 1.0, f"{deeper}")

    # Monotonic, and continuous around the waterline.  A binary inside/outside
    # test would jump from 0 to 1 here, which is what makes a ball snap between
    # full gravity and full lift as it crosses the surface.
    samples = [zone.submerged_fraction((0.0, y, 0.0), radius) for y in
               (radius, radius * 0.5, 0.0, -radius * 0.5, -radius)]
    check("submersion increases monotonically with depth", all(
        samples[i] <= samples[i + 1] for i in range(len(samples) - 1)
    ), f"{[round(s, 4) for s in samples]}")
    check(
        "submersion is continuous across the waterline",
        abs(samples[0] - samples[1]) < 0.2 and abs(samples[1] - samples[2]) < 0.35,
        f"{[round(s, 4) for s in samples]}",
    )

    check("a zero radius is dry, not a division by zero",
          zone.submerged_fraction((0.0, 0.0, 0.0), 0.0) == 0.0)

    outside = make_zone(mode="Bounded")
    outside._has_bounds = True
    outside._min_x, outside._max_x = -10.0, 10.0
    outside._min_z, outside._max_z = -10.0, 10.0
    check(
        "a body outside a bounded region is dry even when deep",
        outside.submerged_fraction((500.0, -500.0, 500.0), radius) == 0.0,
    )


# -----------------------------------------------------------------------------
# 3. Buoyancy and drag
# -----------------------------------------------------------------------------


def test_forces() -> None:
    section("3. Buoyancy and drag")

    zone = make_zone(buoyancy_scale=1.0, linear_drag=0.6, angular_drag=0.6)
    radius = 100.0
    dt = 1.0 / 60.0
    still = (0.0, 0.0, 0.0)

    dry_linear, dry_angular = zone.compute_forces((0.0, 500.0, 0.0), still, still, dt, radius, 1.0)
    check("a dry body is untouched", dry_linear == still and dry_angular == still,
          f"{dry_linear}")

    submerged_linear, _ = zone.compute_forces((0.0, -radius, 0.0), still, still, dt, radius, 1.0)
    expected_lift = water.GRAVITY_CM_PER_S2 * dt * (1.0 - 0.6)
    check(
        "a fully submerged body gains upward velocity",
        abs(submerged_linear[1] - expected_lift) < 1.0e-6,
        f"{submerged_linear[1]:.4f} vs {expected_lift:.4f} cm/s",
    )
    check("lift is applied on Y only",
          submerged_linear[0] == 0.0 and submerged_linear[2] == 0.0)

    # A partially submerged body must get less lift than a fully submerged one.
    partial, _ = zone.compute_forces((0.0, 0.0, 0.0), still, still, dt, radius, 1.0)
    check("partial submersion gives less lift than full submersion",
          partial[1] < submerged_linear[1],
          f"{partial[1]:.4f} < {submerged_linear[1]:.4f}")

    # Dragging velocity towards zero, never past it.
    moving = (500.0, -300.0, 200.0)
    damped, _ = zone.compute_forces((0.0, -radius, 0.0), moving, moving, dt, radius, 0.0)
    check("drag reduces horizontal speed", abs(damped[0]) < abs(moving[0]),
          f"{moving[0]} -> {damped[0]:.2f}")
    check("drag never reverses the direction of travel", damped[0] * moving[0] >= 0.0)
    check("drag never overshoots through zero", abs(damped[0]) <= abs(moving[0]))

    # A longer step must produce proportionally more lift: the result has to be
    # independent of the physics step size, or the sea changes with the framerate.
    slow_step, _ = zone.compute_forces((0.0, -radius, 0.0), still, still, dt * 2.0, radius, 1.0)
    check("the response scales with the step size",
          abs(slow_step[1] - 2.0 * submerged_linear[1]) < 1.0e-6,
          f"{slow_step[1]:.5f} vs {2.0 * submerged_linear[1]:.5f}")

    # A zero-length step must be a no-op rather than a division by zero.
    same, _ = zone.compute_forces((0.0, -radius, 0.0), moving, moving, 0.0, radius, 1.0)
    check("a zero step changes nothing", same == moving)

    heavy = make_zone(buoyancy_scale=2.0, linear_drag=0.0)
    light = make_zone(buoyancy_scale=0.25, linear_drag=0.0)
    heavy_lift, _ = heavy.compute_forces((0.0, -radius, 0.0), still, still, dt, radius, 2.0)
    light_lift, _ = light.compute_forces((0.0, -radius, 0.0), still, still, dt, radius, 0.25)
    check("buoyancyScale scales the lift", heavy_lift[1] > 3.0 * light_lift[1],
          f"{heavy_lift[1]:.3f} vs {light_lift[1]:.3f}")

    # Angular drag must also apply, or a ball in water keeps spinning forever.
    spinning = (0.0, 0.0, 0.0)
    _, angular = zone.compute_forces((0.0, -radius, 0.0), spinning, (10.0, 10.0, 10.0),
                                     dt, radius, 0.0)
    check("angular drag is applied", all(abs(component) < 10.0 for component in angular),
          f"{angular}")


# -----------------------------------------------------------------------------
# 4. Resting on the surface
# -----------------------------------------------------------------------------


def test_resting() -> None:
    section("4. Resting on the water surface")

    zone = make_zone(grounded_tolerance=15.0)
    radius = 100.0

    floating = zone.is_resting_on_surface((0.0, radius, 0.0), radius)
    check("a body floating at one radius above the surface is resting", floating)

    high = zone.is_resting_on_surface((0.0, radius + 500.0, 0.0), radius)
    check("a body well above the surface is not resting", not high)

    low = zone.is_resting_on_surface((0.0, -500.0, 0.0), radius)
    check("a body deep under the surface is not resting", not low)

    threshold = zone.is_resting_on_surface((0.0, radius + 114.0, 0.0), radius)
    check("just inside the tolerance is still resting", threshold)

    beyond = zone.is_resting_on_surface((0.0, radius + 116.0, 0.0), radius)
    check("just beyond the tolerance is not resting", not beyond)


# -----------------------------------------------------------------------------
# 5. The still-water surface without an ocean
# -----------------------------------------------------------------------------


def test_surface() -> None:
    section("5. Surface height")

    zone = make_zone()
    check("with no ocean the surface is the flat waterline",
          zone.surface_y(10.0, 20.0) == 0.0)

    zone._world_y = 1234.5
    check("the waterline is honoured", zone.surface_y(10.0, 20.0) == 1234.5)

    # A body with no ocean system running must not raise.
    check("no ocean system is a supported state",
          water.HydragonOceanSystem.get_instance() is None)


# -----------------------------------------------------------------------------
# 6. Stage notice wiring
# -----------------------------------------------------------------------------


class _FakeNotice:
    """The two accessors the notice handlers use, and nothing else."""

    def GetResyncedPaths(self):
        return []

    def GetChangedInfoOnlyPaths(self):
        return []


def test_notice_handlers() -> None:
    section("6. Stage notice wiring")
    # `Tf.Notice.Register` calls the handler with (notice, sender), NOT with the
    # notice alone.  Declaring only `notice` makes EVERY notice raise TypeError,
    # and that failure is quiet in the worst way: the handler never runs, nothing
    # is ever discovered, and a newly created ocean has no mesh and no material.
    # Reported symptom: `Create > Hydragon > Ocean` left an empty `Looks` behind.
    for name, system in (
        ("ocean", ocean.HydragonOceanSystem()),
        ("water", water.HydragonWaterBodySystem()),
    ):
        parameters = list(inspect.signature(system._on_objects_changed).parameters)
        check(
            f"the {name} notice handler takes (notice, sender)",
            parameters == ["notice", "sender"],
            f"got {parameters}",
        )
        system._is_active = True
        try:
            system._on_objects_changed(_FakeNotice(), None)
            error = ""
        except Exception as exc:  # noqa: BLE001 - the assertion is just below
            error = f"{type(exc).__name__}: {exc}"
        check(
            f"the {name} notice handler survives a real notice call",
            not error,
            error,
        )


# -----------------------------------------------------------------------------
# 7. Looks scope placement
# -----------------------------------------------------------------------------


def test_looks_scope_placement() -> None:
    section("7. Looks scope placement")
    # A `Looks` scope is a STAGE-level resource: one per stage, under the root
    # prim, so a prim in any hierarchy can bind a material from it.  A `Looks`
    # nested inside the ocean prim is invisible to everything outside that prim.
    check(
        "an ocean under World shares /World/Looks",
        ocean.looks_scope_path("/World/Ocean") == "/World/Looks",
        ocean.looks_scope_path("/World/Ocean"),
    )
    check(
        "a deeper hierarchy shares the same root Looks",
        ocean.looks_scope_path("/World/Environment/Water/Ocean") == "/World/Looks",
        ocean.looks_scope_path("/World/Environment/Water/Ocean"),
    )
    check(
        "a root-child prim gets a root-level Looks",
        ocean.looks_scope_path("/Ocean") == "/Looks",
        ocean.looks_scope_path("/Ocean"),
    )
    check(
        "an empty path is not a crash",
        ocean.looks_scope_path("") == "/Looks",
        ocean.looks_scope_path(""),
    )


# -----------------------------------------------------------------------------
# 8. Surface mesh authoring (needs pxr, not Kit)
# -----------------------------------------------------------------------------


def _bootstrap_pxr():
    """Import pxr from the build's `omni.usd.libs`, as `test_volume_bounds.py` does.

    Returns `((Gf, Sdf, Usd, UsdGeom, Vt), None)` or `(None, reason)`.
    """
    import glob

    candidates = sorted(
        glob.glob(
            os.path.join(
                _ROOT, "_build", "windows-x86_64", "release", "extscache", "omni.usd.libs-*"
            )
        )
    )
    if not candidates:
        return None, "no omni.usd.libs in extscache - is the project built?"
    libs = candidates[-1].rstrip("\\/")
    if libs not in sys.path:
        sys.path.insert(0, libs)
    os.add_dll_directory(os.path.join(libs, "bin"))
    try:
        from pxr import Gf, Sdf, Usd, UsdGeom, Vt
    except Exception as exc:  # noqa: BLE001
        return None, f"{type(exc).__name__}: {exc}"
    return (Gf, Sdf, Usd, UsdGeom, Vt), None


def _prepare_ocean_for_usd():
    """Bootstrap pxr and hand it to `ocean_controller`.

    That module keeps pxr behind a Kit-guarded import, so outside Kit it has
    collapsed Usd/UsdGeom/Sdf/Gf/Vt to None.  The authoring needs no Kit - only
    pxr - so it gets the pxr this script bootstrapped.  `schemas` collapsed
    HAS_PXR the same way, and `_has_api_schema` short-circuits on it.
    """
    modules, reason = _bootstrap_pxr()
    if modules is None:
        return None, reason
    Gf, Sdf, Usd, UsdGeom, Vt = modules

    ocean.HAS_KIT = True
    ocean.Usd = Usd
    ocean.UsdGeom = UsdGeom
    ocean.Sdf = Sdf
    ocean.Gf = Gf
    ocean.Vt = Vt

    from hydragon.editor.core import schemas as schemas_module

    schemas_module.HAS_PXR = True
    return modules, None


def test_surface_mesh_authoring() -> None:
    section("8. Surface mesh authoring")
    modules, reason = _prepare_ocean_for_usd()
    if modules is None:
        check("pxr is importable for the mesh test", False, reason)
        return
    Gf, Sdf, Usd, UsdGeom, Vt = modules

    stage = Usd.Stage.CreateInMemory()
    prim = UsdGeom.Xform.Define(stage, "/World/Ocean").GetPrim()
    patch = ocean.HydragonOceanPatch(prim)
    patch._ensure_surface_mesh(1000.0)

    mesh_prim = stage.GetPrimAtPath("/World/Ocean/surface/ocean_surface")
    if not check_and_stop("the surface mesh is authored", mesh_prim.IsValid()):
        return
    mesh = UsdGeom.Mesh(mesh_prim)
    grid = ocean.MESH_GRID

    points = mesh.GetPointsAttr().Get()
    check("the mesh has one point per grid vertex", len(points) == grid * grid, f"{len(points)}")
    counts = mesh.GetFaceVertexCountsAttr().Get()
    check("the mesh is a grid of quads", list(counts) == [4] * ((grid - 1) ** 2), f"{len(counts)} quads")
    check(
        "the mesh is not subdivided by the renderer",
        mesh.GetSubdivisionSchemeAttr().Get() == "none",
        str(mesh.GetSubdivisionSchemeAttr().Get()),
    )
    # A surface with no authored extent is culled by Hydra, silently.
    extent = mesh.GetExtentAttr().Get()
    check("the mesh carries an extent", extent is not None and len(extent) == 2, str(extent))
    # 1000 m at 1 unit = 1 cm spans 100000 units.  Getting this wrong does not
    # error - it draws an ocean a hundred times too small.
    span = max(p[0] for p in points) - min(p[0] for p in points)
    check("the patch spans patchSize in stage units", abs(span - 100000.0) < 1.0, f"{span:.1f} units")

    # THE REGRESSION: `CreatePrimvar` is not a method of `UsdGeom.Mesh` (it is on
    # `UsdGeom.PrimvarsAPI`) and the interpolation token is `vertexInterpolation`.
    # Reported: "could not author the surface mesh: 'Mesh' object has no attribute
    # 'CreatePrimvar'", plus "Failed to load geometry ... storage data" from
    # rtx.hydra.geometrystreaming for the half-authored mesh it left behind.
    st = UsdGeom.PrimvarsAPI(mesh_prim).GetPrimvar("st")
    check("the mesh carries `st`", bool(st) and st.IsDefined(), "")
    check("`st` is vertex-interpolated", st.GetInterpolation() == "vertex", str(st.GetInterpolation()))
    check("`st` has one entry per vertex", len(st.Get()) == grid * grid, f"{len(st.Get())}")


# -----------------------------------------------------------------------------
# 9. Notices are judged one frame later
# -----------------------------------------------------------------------------


class _NoticeWith:
    """A notice naming resynced paths, with no USD behind it."""

    def __init__(self, paths):
        self._paths = list(paths)

    def GetResyncedPaths(self):
        return list(self._paths)

    def GetChangedInfoOnlyPaths(self):
        return []


def test_deferred_notice_evaluation() -> None:
    section("9. Notices are judged one frame later")
    modules, reason = _prepare_ocean_for_usd()
    if modules is None:
        check("pxr is importable for the notice test", False, reason)
        return
    Gf, Sdf, Usd, UsdGeom, Vt = modules

    stage = Usd.Stage.CreateInMemory()
    # A prim as it looks when the payload ARC has been authored but the payload has
    # not composed yet: a bare Xform, no apiSchemas, no children.
    UsdGeom.Xform.Define(stage, "/World/Ocean")
    check(
        "a bare prim is not recognised as an ocean",
        ocean.paths_include_schema(
            {"/World/Ocean"}, ocean.HydragonOcean.is_applied, stage
        )
        is False,
        "this is exactly what the notice handler used to see",
    )

    system = ocean.HydragonOceanSystem()
    system._is_active = True
    system._on_objects_changed(_NoticeWith(["/World/Ocean"]), None)

    # THE REGRESSION. Judged inside the notice, this path answered "not an ocean",
    # so no resync was ever scheduled and the mesh and the material only appeared
    # at the first PLAY (which resyncs through the timeline instead).
    check(
        "the notice is recorded, not acted on",
        system._sync_pending is False and "/World/Ocean" in system._notice_paths,
        f"pending={system._sync_pending} paths={sorted(system._notice_paths)}",
    )

    # One frame later the payload has composed.
    prim = stage.GetPrimAtPath("/World/Ocean")
    prim.SetMetadata("apiSchemas", Sdf.TokenListOp.CreateExplicit(["HydragonOceanAPI"]))
    check(
        "the deferred judgement sees the composed ocean",
        system._evaluate_notice_paths(system._notice_paths, stage) is True,
        "",
    )
    check(
        "an unrelated path is still ignored",
        system._evaluate_notice_paths({"/World/Ground"}, stage) is False,
        "",
    )
    system._patches["/World/Ocean"] = None
    check(
        "a path under a registered patch is a resync",
        system._evaluate_notice_paths({"/World/Ocean/surface/ocean_surface"}, stage) is True,
        "",
    )
    check(
        "an empty batch is not a resync",
        system._evaluate_notice_paths(set(), stage) is False,
        "",
    )


def _asset_path(value) -> str:
    """The readable path of an authored asset value.

    `str(Sdf.AssetPath)` is the SERIALISED USD form, delimiters included
    (`@HydragonWater.mdl@`), so comparing it against a file name never matches.
    The value itself lives on `.path`.
    """
    return getattr(value, "path", None) or str(value)


# -----------------------------------------------------------------------------
# 10. The water material's MDL contract
# -----------------------------------------------------------------------------


def test_water_mdl() -> None:
    section("10. Water material (MDL)")

    search_path = ocean.mdl_search_path()
    check(
        "the MDL search path is the extension's data/shaders",
        search_path.replace("\\", "/").endswith("hydragon.editor.core/data/shaders"),
        search_path,
    )
    module_file = ocean.water_mdl_file()
    check("the water material file exists", os.path.isfile(module_file), module_file)
    if not os.path.isfile(module_file):
        return

    with open(module_file, encoding="utf-8") as handle:
        source = handle.read()

    # USD names the MODULE (`HydragonWater.mdl`) and the material INSIDE it
    # (`HydragonWater`) separately, and `_bind_material` authors both from the
    # constants above.  Renaming either side has to break these checks rather than
    # silently bind a material that does not resolve.
    check(
        f"the MDL exports the material `{ocean.WATER_MDL_MODULE}`",
        f"export material {ocean.WATER_MDL_MODULE}" in source,
        ocean.WATER_MDL_MODULE,
    )
    check(
        f"the MDL samples the `{ocean.WATER_MDL_FIELD_INPUT}` input",
        ocean.WATER_MDL_FIELD_INPUT in source,
        ocean.WATER_MDL_FIELD_INPUT,
    )
    # The channel layout is a contract between three files: the packer in
    # `ocean_fft_kernels`, the reference in `ocean_spectrum`, and this reader.
    # Section 6 pins the packer; this pins the reader from the other end.
    #
    # These two checks also pin the DIALECT RULES the MDL compiler enforces and
    # that cost a round trip to discover: `float4` has no `.yz` member, and
    # `state::texture_coordinate` returns float3, not float2.
    check(
        "the MDL reads the normals and the foam component by component",
        "float2(field.y, field.z)" in source and "field.w" in source,
        "expected the (height, normal.x, normal.z, foam) layout",
    )
    check(
        "the MDL converts the float3 texture coordinate explicitly",
        "float3 coord = state::texture_coordinate(0)" in source
        and "float2 uv = float2(coord.x, coord.y)" in source,
        "state::texture_coordinate returns float3",
    )
    check(
        "the MDL guards the reconstructed normal against a negative square root",
        "math::saturate(1.0 - math::dot(slope, slope))" in source,
        "a float32 round trip can push the value below zero, which is a NaN",
    )
    check(
        "the MDL takes the foam mask only when it is bound",
        "tex::texture_isvalid(foam_mask_texture)" in source,
        "an empty texture must not reach the sampler",
    )

    # And the AUTHORED USD has to agree with the constants and with the file, or
    # the material points at a module that does not exist.
    modules, reason = _prepare_ocean_for_usd()
    if modules is None:
        check("pxr is importable for the material test", False, reason)
        return
    Gf, Sdf, Usd, UsdGeom, Vt = modules

    stage = Usd.Stage.CreateInMemory()
    prim = UsdGeom.Xform.Define(stage, "/World/Ocean").GetPrim()
    patch = ocean.HydragonOceanPatch(prim)
    patch._bind_material("hydragon_ocean_test")

    shader_prim = stage.GetPrimAtPath("/World/Looks/ocean_surface_mat_World_Ocean/Shader")
    if not check_and_stop(
        "the shader is authored in the shared stage-level Looks scope",
        shader_prim.IsValid(),
        "expected /World/Looks/...",
    ):
        return

    check(
        "the shader source asset is the engine's own MDL",
        _asset_path(shader_prim.GetAttribute("info:mdl:sourceAsset").Get())
        == ocean.WATER_MDL_FILE,
        _asset_path(shader_prim.GetAttribute("info:mdl:sourceAsset").Get()),
    )
    check(
        "the sub-identifier is the exported material name",
        str(shader_prim.GetAttribute("info:mdl:sourceAsset:subIdentifier").Get())
        == ocean.WATER_MDL_MODULE,
        str(shader_prim.GetAttribute("info:mdl:sourceAsset:subIdentifier").Get()),
    )

    field_attr = shader_prim.GetAttribute(f"inputs:{ocean.WATER_MDL_FIELD_INPUT}")
    if not check_and_stop("the surface field input is authored", bool(field_attr), ""):
        return
    check(
        "the surface field points at the dynamic texture",
        _asset_path(field_attr.Get()) == "dynamic://hydragon_ocean_test",
        _asset_path(field_attr.Get()),
    )
    check(
        "the surface field is declared as a 2D texture",
        field_attr.GetMetadata("renderType") == "texture_2d",
        str(field_attr.GetMetadata("renderType")),
    )

    # The registration is retried from `_ensure_texture`, so it must be a no-op
    # once the path is in, or every ocean would add the same path again.
    ocean._REGISTERED_MDL_PATH = "/already/registered"
    check(
        "registering the MDL library twice is a no-op",
        ocean.register_mdl_library() is True,
        "it must not call into neuraylib again",
    )
    ocean._REGISTERED_MDL_PATH = None

    # MIGRATION. An earlier build of `_bind_material` bound a stock OmniPBR there
    # and authored `inputs:diffuse_texture`. USD would keep that input alongside
    # the new `inputs:surface_field`, and the MDL-to-USD mapping would then have to
    # explain an input the engine's module does not declare.
    shader_prim.CreateAttribute("info:mdl:sourceAsset", Sdf.ValueTypeNames.Asset).Set(
        Sdf.AssetPath("OmniPBR.mdl")
    )
    shader_prim.CreateAttribute("inputs:diffuse_texture", Sdf.ValueTypeNames.Asset)

    patch._bind_material("hydragon_ocean_test")
    # The prim is fetched again ON PURPOSE: the stale material was removed and
    # re-authored, so the handle captured above is expired - which is exactly what
    # proves this was a replacement rather than a merge.
    rebound = stage.GetPrimAtPath("/World/Looks/ocean_surface_mat_World_Ocean/Shader")
    if not check_and_stop("the material is re-authored", rebound.IsValid(), ""):
        return
    check(
        "a stale stock input does not survive the rebind",
        not rebound.GetAttribute("inputs:diffuse_texture"),
        "the old OmniPBR input is still there",
    )
    check(
        "the rebound material keeps the engine's input",
        bool(rebound.GetAttribute(f"inputs:{ocean.WATER_MDL_FIELD_INPUT}")),
        "",
    )


# -----------------------------------------------------------------------------
# Runner
# -----------------------------------------------------------------------------


def main() -> int:
    print(f"repo root: {_ROOT}")
    test_containment()
    test_submerged_fraction()
    test_forces()
    test_resting()
    test_surface()
    test_notice_handlers()
    test_looks_scope_placement()
    test_surface_mesh_authoring()
    test_deferred_notice_evaluation()
    test_water_mdl()

    print(f"\n{CHECKS[0] - len(FAILURES)}/{CHECKS[0]} checks passed")
    if FAILURES:
        print("FAILED: " + "; ".join(FAILURES))
        return 1
    print("ALL OCEAN AND WATER BODY TESTS PASSED")
    return 0


if __name__ == "__main__":
    sys.exit(main())
