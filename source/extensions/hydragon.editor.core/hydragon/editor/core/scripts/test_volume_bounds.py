"""
Headless tests for volume_bounds.py

Mirrors the structure of the SHIPPED asset, including its double scale, because that is where the
old derivation went wrong:

    /World/ForceVolume_vortex               scale (2, 1, 2)      <- the volume root
        volumes/force_bounds                scale (3, 1.5, 3)    <- the bounds prim itself
            extent = +/-100, BasisCurves

The correct answer is the bound in the volume root's OWN frame: (300, 150, 300). The old code
returned `extent * world-scale` = 100 * (6, 1.5, 6) = (600, 150, 600) and then compared it against
points it had already transformed into local space, so the effective region was 6x too large.

Needs pxr, which is NOT installed in the target-deps Python; this script bootstraps it from the
extscache and sets up the DLL search directory:

    & "_build/target-deps/python/python.exe" \
        "source/extensions/hydragon.editor.core/hydragon/editor/core/scripts/test_volume_bounds.py"
"""

import glob
import os
import sys

# --- bootstrap pxr from the extscache --------------------------------------------------------
_HERE = os.path.dirname(os.path.abspath(__file__))


def _find_repo_root(start):
    current = start
    while True:
        if os.path.isdir(os.path.join(current, "_build")):
            return current
        parent = os.path.dirname(current)
        if parent == current:
            return None
        current = parent


_ROOT = _find_repo_root(_HERE)
if not _ROOT:
    print("Could not locate the repository root (no _build directory above this file).")
    sys.exit(2)

_LIBS = glob.glob(os.path.join(_ROOT, "_build", "windows-x86_64", "release", "extscache", "omni.usd.libs-*"))
if not _LIBS:
    print("Could not find the omni.usd.libs extscache - is the project built?")
    sys.exit(2)
_LIBS = _LIBS[0].rstrip("\\/")
sys.path.insert(0, _LIBS)
_DLL_DIR = os.add_dll_directory(os.path.join(_LIBS, "bin"))

# --- make the extension importable ----------------------------------------------------------
_EXT_DIR = os.path.join(_ROOT, "source", "extensions", "hydragon.editor.core")
if _EXT_DIR not in sys.path:
    sys.path.insert(0, _EXT_DIR)

from pxr import Gf, Sdf, Usd, UsdGeom  # noqa: E402

from hydragon.editor.core import volume_bounds  # noqa: E402
from hydragon.editor.core.volume_geometry import (  # noqa: E402
    MIN_HALF_EXTENT,
    build_shape_geometry,
)

FAILURES = []


def check(label, condition, detail=""):
    print(f"  [{'PASS' if condition else 'FAIL'}] {label}{(' - ' + detail) if detail else ''}")
    if not condition:
        FAILURES.append(label)


def set_scale(prim, value):
    """Sets the prim's scale xformOp, creating it only when it does not already exist.

    `AddScaleOp()` throws when the op is already present, which is easy to hit when the prim was
    built with an identity scale and is then rescaled.
    """
    xformable = UsdGeom.Xformable(prim)
    for op in xformable.GetOrderedXformOps():
        if op.GetOpType() == UsdGeom.XformOp.TypeScale:
            op.Set(Gf.Vec3f(*value))
            return
    xformable.AddScaleOp().Set(Gf.Vec3f(*value))


def build_volume(shape="Box", half=100.0, root_scale=(1, 1, 1), bounds_scale=(1, 1, 1),
                 bounds_translate=None, purpose="default", bounds_visible=True):
    """Builds the same hierarchy the asset uses and returns the volume root prim."""
    stage = Usd.Stage.CreateInMemory()
    root = UsdGeom.Xform.Define(stage, "/World/Volume")
    root.AddScaleOp().Set(Gf.Vec3f(*root_scale))
    UsdGeom.Xform.Define(stage, "/World/Volume/volumes")

    points, counts, extent = build_shape_geometry(shape, (half, half, half), half, half)
    curves = UsdGeom.BasisCurves.Define(stage, "/World/Volume/volumes/force_bounds")
    curves.CreateTypeAttr(UsdGeom.Tokens.linear)
    curves.CreateWrapAttr(UsdGeom.Tokens.nonperiodic)
    curves.CreateCurveVertexCountsAttr(counts)
    curves.CreatePointsAttr([Gf.Vec3f(*p) for p in points])
    curves.CreateWidthsAttr([2.5] * len(points))
    curves.CreateExtentAttr().Set([Gf.Vec3f(*extent[0]), Gf.Vec3f(*extent[1])])
    UsdGeom.Imageable(curves.GetPrim()).CreatePurposeAttr().Set(purpose)
    set_scale(curves.GetPrim(), bounds_scale)
    if bounds_translate is not None:
        UsdGeom.Xformable(curves.GetPrim()).AddTranslateOp().Set(Gf.Vec3f(*bounds_translate))
    if not bounds_visible:
        UsdGeom.Imageable(curves.GetPrim()).CreateVisibilityAttr().Set(UsdGeom.Tokens.invisible)

    return stage, stage.GetPrimAtPath("/World/Volume")


print("=" * 78)
print("1. The frame: own frame, NOT world, NOT parent")
print("=" * 78)

# The exact numbers from the shipped asset: root (2,1,2), bounds (3,1.5,3), extent +/-100.
stage, volume = build_volume("Cylinder", half=100.0, root_scale=(2, 1, 2), bounds_scale=(3, 1.5, 3))
half_extents, radius, half_height = volume_bounds.measure_shape_parameters(volume, "Force")
print(f"     {volume_bounds.describe(volume, 'Force')}")
check(
    "half-extents come from the BOUNDS prim's own scale (3, 1.5, 3) x 100",
    tuple(round(v, 3) for v in half_extents) == (300.0, 150.0, 300.0),
    f"{tuple(round(v, 3) for v in half_extents)}",
)
check(
    "the volume root's own scale (2, 1, 2) is EXCLUDED",
    abs(half_extents[0] - 600.0) > 1.0,
    "600 would mean the root scale was applied - the double-application bug",
)
check("Cylinder radius comes from X/Z", radius == 300.0 and half_height == 150.0)

print("\n" + "=" * 78)
print("2. A child edit propagates; the root's own transform is not our business")
print("=" * 78)

stage, volume = build_volume("Box", half=100.0, root_scale=(9, 9, 9), bounds_scale=(1, 1, 1))
scaled = volume_bounds.measure_shape_parameters(volume, "Force")[0]
print(f"     root scale x9 only -> half_extents = {tuple(round(v, 3) for v in scaled)}")
check("root scale alone changes nothing (it is inherited, not ours)", scaled == (100.0, 100.0, 100.0))

bounds_prim = stage.GetPrimAtPath("/World/Volume/volumes/force_bounds")
set_scale(bounds_prim, (4.0, 4.0, 4.0))
rescaled = volume_bounds.measure_shape_parameters(volume, "Force")[0]
print(f"     then bounds scaled x4 -> half_extents = {tuple(round(v, 3) for v in rescaled)}")
check("a child edit IS picked up (and not from a stale cache)", rescaled == (400.0, 400.0, 400.0))

print("\n" + "=" * 78)
print("3. The Shift+V trap: an INVISIBLE bounds prim must still measure")
print("=" * 78)

stage, volume = build_volume("Box", half=100.0, bounds_visible=False)
half_extents, _radius, _half_height = volume_bounds.measure_shape_parameters(volume, "Force")
print(f"     invisible bounds -> half_extents = {tuple(round(v, 3) for v in half_extents)}")
check(
    "ignoreVisibility=True keeps the measurement (no collapse to zero)",
    half_extents == (100.0, 100.0, 100.0),
    f"{tuple(round(v, 3) for v in half_extents)}",
)

print("\n" + "=" * 78)
print("4. Purpose filtering: guide/proxy helper geometry must not count")
print("=" * 78)

for purpose, expected_counted in (("default", True), ("render", True), ("guide", False), ("proxy", False)):
    stage, volume = build_volume("Box", half=100.0, purpose=purpose)
    bound = volume_bounds.measure_relative_half_extents(volume)
    counted = bound is not None
    print(f"     purpose={purpose:8s} -> bound = {bound}")
    check(f"purpose={purpose} is {'counted' if expected_counted else 'excluded'}",
          counted == expected_counted)

print("\n" + "=" * 78)
print("5. Missing geometry is a normal transient state, not an error")
print("=" * 78)

stage = Usd.Stage.CreateInMemory()
empty_volume = UsdGeom.Xform.Define(stage, "/World/Empty").GetPrim()
half_extents, radius, _half_height = volume_bounds.measure_shape_parameters(empty_volume, "Force")
print(f"     empty volume -> {tuple(round(v, 3) for v in half_extents)} radius={radius}")
check("an empty volume yields the documented default, never zero",
      all(v > 0 for v in half_extents) and radius > 0)
check("read_shape falls back to Box when the attribute is absent",
      volume_bounds.read_shape(empty_volume, "Force") == "Box")

print("\n" + "=" * 78)
print("6. Shape recovery in the own frame")
print("=" * 78)

for shape, half, expected in (
    ("Box", 100.0, (100.0, 100.0, 100.0)),
    ("Sphere", 120.0, (120.0, 120.0, 120.0)),
    ("Cylinder", 100.0, (100.0, 100.0, 100.0)),
    # A plane genuinely has no thickness on Y, so that axis comes back as the HARD FLOOR value
    # and not as 0. This is the intended behaviour, not a rounding artefact.
    ("Plane", 100.0, (100.0, MIN_HALF_EXTENT, 100.0)),
):
    stage, volume = build_volume(shape, half=half)
    half_extents, radius, half_height = volume_bounds.measure_shape_parameters(volume, "Force")
    print(f"     {shape:9s} half={half} -> half_extents={tuple(round(v, 3) for v in half_extents)}"
          f" radius={round(radius, 3)}")
    check(f"{shape}: recovers its authored size", tuple(round(v, 3) for v in half_extents) == expected)

print("\n" + "=" * 78)
print("7. read_shape reads the authored token per volume type")
print("=" * 78)

stage = Usd.Stage.CreateInMemory()
prim = UsdGeom.Xform.Define(stage, "/World/V").GetPrim()
prim.CreateAttribute("force:volumeShape", Sdf.ValueTypeNames.Token).Set("Sphere")
print(f"     force:volumeShape = {volume_bounds.read_shape(prim, 'Force')}")
check("reads the Force attribute", volume_bounds.read_shape(prim, "Force") == "Sphere")
check("a missing Kill attribute falls back to Box",
      volume_bounds.read_shape(prim, "Kill") == volume_bounds.DEFAULT_SHAPE)

print("\n" + "=" * 78)
print("8. The bounds prim can be OFFSET from the volume root (the shipped asset does exactly this)")
print("=" * 78)

# ForceVolume_vortex offsets its force_bounds with xformOp:translate = (0, 22, 0), so the drawn
# region is NOT centred on the volume root's origin. GetSize() alone would hide that, and the
# containment test would then treat the region as centred on the wrong point.
stage, volume = build_volume("Box", half=100.0, bounds_translate=(0.0, 22.0, 0.0))
centre, half_extents = volume_bounds.measure_bound(volume)
print(f"     bounds translated (0, 22, 0) -> centre={tuple(round(v, 3) for v in centre)} "
      f"half_extents={tuple(round(v, 3) for v in half_extents)}")
check("the centre follows the GEOMETRY, not the volume root's origin",
      tuple(round(v, 3) for v in centre) == (0.0, 22.0, 0.0),
      f"{tuple(round(v, 3) for v in centre)}")
check("the offset does not change the size",
      tuple(round(v, 3) for v in half_extents) == (100.0, 100.0, 100.0))

print("\n" + "=" * 78)
print("9. Sibling immunity: adding a collider (or any helper) must NOT change the volume's size")
print("=" * 78)

# This is the case that motivates measuring the WIREFRAME prim relative to the volume root instead
# of measuring the root's whole subtree. A PhysX collider parented under the volume is a sibling
# of `volumes/force_bounds`, and a subtree measurement would silently absorb it.
stage, volume = build_volume("Box", half=100.0)
before_centre, before_half = volume_bounds.measure_bound(volume, "Force")

sibling = UsdGeom.Cube.Define(stage, "/World/Volume/collider")
sibling.CreateSizeAttr(1000.0)  # deliberately enormous, so any leak is unmissable
_after_centre, after_half = volume_bounds.measure_bound(volume, "Force")

check("a huge sibling collider does NOT inflate the volume",
      tuple(round(v, 3) for v in after_half) == tuple(round(v, 3) for v in before_half),
      f"{tuple(round(v, 3) for v in after_half)}")
check("the centre is unchanged too", _after_centre == before_centre)

# And the same for a sibling INSIDE the volumes group, which is the likelier layout.
sibling2 = UsdGeom.Cube.Define(stage, "/World/Volume/volumes/trigger_collider")
sibling2.CreateSizeAttr(4000.0)
_final_centre, final_half = volume_bounds.measure_bound(volume, "Force")
print(f"     after a 4000-unit sibling inside `volumes` -> half_extents={tuple(round(v, 3) for v in final_half)}")
check("a sibling inside the `volumes` group does not inflate it either",
      tuple(round(v, 3) for v in final_half) == tuple(round(v, 3) for v in before_half))

# A volume with no wireframe at all must yield NO measurement rather than a subtree guess.
bare_stage = Usd.Stage.CreateInMemory()
bare = UsdGeom.Xform.Define(bare_stage, "/World/Bare").GetPrim()
UsdGeom.Cube.Define(bare_stage, "/World/Bare/some_collider").CreateSizeAttr(900.0)
print(f"     volume with no wireframe but a 900-unit child -> "
      f"measure_bound = {volume_bounds.measure_bound(bare, 'Force')}")
check("no wireframe means no measurement (never a subtree guess)",
      volume_bounds.measure_bound(bare, "Force") is None)


# ---------------------------------------------------------------------------------------------
# 8. TRIGGER AUTHORING must be repeatable
# ---------------------------------------------------------------------------------------------

print("\n8. TRIGGER AUTHORING: authoring the same prim twice must not raise")

import hydragon.editor.core.volume_triggers as volume_triggers  # noqa: E402

# `volume_triggers` keeps pxr behind a Kit-guarded import, so outside Kit it has already collapsed
# `Usd`, `UsdGeom` and `Gf` to None. The authoring itself needs no Kit - only pxr - so hand it the
# pxr this script bootstrapped.
volume_triggers.HAS_KIT = True
volume_triggers.Usd = Usd
volume_triggers.UsdGeom = UsdGeom
volume_triggers.Gf = Gf

_probe_stage = Usd.Stage.CreateInMemory()
_probe = UsdGeom.Cube.Define(_probe_stage, "/World/probe/volumes/force_trigger")
_probe.GetSizeAttr().Set(2.0)

# THE REGRESSION. `AddTranslateOp()` / `AddScaleOp()` raise when the op is ALREADY in xformOpOrder -
# they do not return the existing op. A trigger collider persists in the stage layer, so the second
# pass over it (an extension reload, or any edit that changes the size) raised
# "The xformOp 'xformOp:translate' already exists in xformOpOrder", which then skipped the API
# schema application and escaped startup() and killed the whole system.
_errors = []
for _pass in range(3):
    try:
        volume_triggers.author_trigger_transform(
            _probe.GetPrim(), (0.0, 22.0, 0.0), (300.0, 150.0, 300.0)
        )
    except Exception as _exc:
        _errors.append(f"pass {_pass}: {_exc!r}")

check("authoring the same transform three times raises nothing", not _errors, "; ".join(_errors))

_xformable = UsdGeom.Xformable(_probe.GetPrim())
_ops = [op.GetOpName() for op in _xformable.GetOrderedXformOps()]
check("the op order stays exactly [translate, scale] - no duplicates", len(_ops) == 2, f"{_ops}")

_translation = _xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default()).ExtractTranslation()
check("the authored placement survives re-authoring",
      abs(_translation[1] - 22.0) < 1e-6,
      f"translation={tuple(round(float(v), 3) for v in _translation)}")

_attr_names = [attr.GetName() for attr in _probe.GetPrim().GetAttributes()]
check("no duplicate xformOp attributes accumulate in the layer",
      _attr_names.count("xformOp:translate") == 1 and _attr_names.count("xformOp:scale") == 1,
      f"{sorted(name for name in _attr_names if name.startswith('xformOp'))}")


# ---------------------------------------------------------------------------------------------
# 9. USD NOTICE FILTERING must see a NEWLY CREATED volume
# ---------------------------------------------------------------------------------------------

print("\n9. USD NOTICES: a newly created volume must be noticed")

from hydragon.editor.core.schemas import HydragonForceVolume  # noqa: E402


class _MockNotice:
    """The two path getters `ObjectsChanged` exposes."""

    def __init__(self, resynced=(), changed=()):
        self._resynced = list(resynced)
        self._changed = list(changed)

    def GetResyncedPaths(self):
        return self._resynced

    def GetChangedInfoOnlyPaths(self):
        return self._changed


_notice_paths = volume_triggers.notice_prim_paths(
    _MockNotice(
        resynced=["/World/ForceVolume"],
        # Stamping a schema onto an existing prim reports the ATTRIBUTE path, not the prim.
        changed=["/World/ForceVolume.apiSchemas"],
    )
)
check("property paths are trimmed to their prim",
      _notice_paths == {"/World/ForceVolume"}, f"{sorted(_notice_paths)}")

check("an edit under an already-registered volume is detected",
      volume_triggers.touches_any({"/World/A/volumes/force_bounds"}, {"/World/A"}))
check("an unrelated path is not",
      not volume_triggers.touches_any({"/World/B"}, {"/World/A"}))

# THE REGRESSION. `touches_any` alone can never see the FIRST volume placed in a scene, because
# nothing is registered yet. The system used to filter on registered paths only, so
# `Create > Hydragon > Force Volume` left a bare prim and the wireframe only appeared at the next
# PLAY, which is what forced a sync.
_notice_stage = Usd.Stage.CreateInMemory()
_new_volume = UsdGeom.Xform.Define(_notice_stage, "/World/ForceVolume").GetPrim()
_new_volume.CreateAttribute("force:mode", Sdf.ValueTypeNames.Token).Set("Continuous")
UsdGeom.Xform.Define(_notice_stage, "/World/Ground")

check("a newly created volume is detected with an EMPTY registry",
      volume_triggers.paths_include_volume(
          {"/World/ForceVolume"}, HydragonForceVolume.is_applied, _notice_stage))
check("a newly created non-volume is not",
      not volume_triggers.paths_include_volume(
          {"/World/Ground"}, HydragonForceVolume.is_applied, _notice_stage))
check("a stale path that no longer exists is handled without raising",
      not volume_triggers.paths_include_volume(
          {"/World/Deleted"}, HydragonForceVolume.is_applied, _notice_stage))

print("\n" + "=" * 78)
if FAILURES:
    print(f"FAILED ({len(FAILURES)}): " + "; ".join(FAILURES))
    sys.exit(1)
print("ALL VOLUME BOUNDS TESTS PASSED")
print("=" * 78)
