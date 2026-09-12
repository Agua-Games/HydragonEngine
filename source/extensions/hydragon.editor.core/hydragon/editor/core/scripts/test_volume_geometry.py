"""
Headless tests for volume_geometry.py

Runs outside Kit - the module has no pxr or Kit imports on purpose:

    & "_build/target-deps/python/python.exe" \
        "source/extensions/hydragon.editor.core/hydragon/editor/core/scripts/test_volume_geometry.py"

What is asserted, and why each one matters:

  1. SCHEMA VALIDITY - every curveVertexCounts entry must be > 2, because the UsdGeom schema
     rejects 2-vertex segments for `type=linear` + `wrap=nonperiodic`. The production generator
     authored `counts = [2] * 12` for the Box shape, which is the bug this guards against.
  2. CONSISTENCY - counts and points must agree, and the declared extent must match the real
     axis-aligned bound of the points.
  3. CONVERGENCE - the edit -> bound -> edit cycle must reach a fixed point immediately for
     every shape. This is the property that makes it safe to derive dimensions from geometry.
  4. THE COUNTER-EXAMPLE - the circumscribed (half-diagonal) radius rule must be shown to
     DIVERGE, so nobody "fixes" the rule back into a geometric radius without noticing.
  5. NON-UNIFORM DISTORTION - a distorted bound must collapse to one stable radius.
  6. THE TRIGGER COLLIDER - the Gprim generated for a shape must reproduce, point for point, the
     analytic containment test it replaced. This is the assertion the whole trigger migration rests
     on: if it holds, moving overlap detection from Python into PhysX cannot change which bodies a
     volume affects.
"""

import math
import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
_CORE_DIR = os.path.dirname(_HERE)
_EXT_DIR = os.path.dirname(os.path.dirname(os.path.dirname(_CORE_DIR)))
if _EXT_DIR not in sys.path:
    sys.path.insert(0, _EXT_DIR)

from hydragon.editor.core.volume_geometry import (  # noqa: E402
    DEFAULT_HALF_EXTENT,
    MIN_HALF_EXTENT,
    PLANE_TRIGGER_THICKNESS,
    SHAPES,
    box_geometry,
    build_shape_geometry,
    cylinder_geometry,
    plane_geometry,
    recover_half_extents,
    recover_shape_parameters,
    sphere_geometry,
    trigger_collider_spec,
)

FAILURES = []


def check(label, condition, detail=""):
    print(f"  [{'PASS' if condition else 'FAIL'}] {label}{(' - ' + detail) if detail else ''}")
    if not condition:
        FAILURES.append(label)


def bound_half_extents(points):
    """Axis-aligned half-extents of a point cloud, centred on the origin."""
    return tuple(max(abs(p[axis]) for p in points) for axis in range(3))


def cycle(shape, state, iterations=5):
    """Runs params -> geometry -> bound -> params repeatedly; returns (states, converged_at)."""
    history = [state]
    for i in range(iterations):
        points, _counts, _extent = build_shape_geometry(shape, *state)
        state = recover_shape_parameters(shape, bound_half_extents(points))
        history.append(state)
        if history[-1] == history[-2]:
            return history, i
    return history, None


# ---------------------------------------------------------------------------------------------
print("=" * 78)
print("1. Schema validity: linear nonperiodic requires curveVertexCounts[i] > 2")
print("=" * 78)

CASES = {
    "Box": box_geometry((100.0, 50.0, 75.0)),
    "Sphere": sphere_geometry(120.0),
    "Cylinder": cylinder_geometry(80.0, 150.0),
    "Plane": plane_geometry((200.0, 0.0, 300.0)),
}

for name, (points, counts, extent) in CASES.items():
    smallest = min(counts)
    print(f"\n  {name}: {len(counts)} curve(s), {len(points)} point(s), min count = {smallest}")
    check(f"{name}: every count > 2 (the counts=[2]*12 bug)", smallest > 2, f"min={smallest}")
    check(f"{name}: points match the counts", len(points) == sum(counts),
          f"{len(points)} vs {sum(counts)}")
    check(f"{name}: extent has 2 corners", len(extent) == 2)

# ---------------------------------------------------------------------------------------------
print("\n" + "=" * 78)
print("2. The declared extent must describe the real geometry")
print("=" * 78)

for name, (points, _counts, extent) in CASES.items():
    measured = bound_half_extents(points)
    declared = tuple(abs(extent[1][axis]) for axis in range(3))
    deltas = [abs(measured[axis] - declared[axis]) for axis in range(3)]
    print(f"\n  {name}: measured={tuple(round(v, 3) for v in measured)} "
          f"declared={tuple(round(v, 3) for v in declared)}")
    check(f"{name}: extent matches the point cloud", all(d < 1e-4 for d in deltas), str(deltas))

# ---------------------------------------------------------------------------------------------
print("\n" + "=" * 78)
print("3. Convergence: shape params -> geometry -> bound -> shape params")
print("=" * 78)

INITIAL_STATES = {
    "Box": ((100.0, 50.0, 75.0), 100.0, 50.0),
    "Sphere": ((120.0, 120.0, 120.0), 120.0, 120.0),
    "Cylinder": ((80.0, 150.0, 80.0), 80.0, 150.0),
    # A plane's radius comes from X/Z, so the consistent value is max(200, 300); its Y axis is the
    # degenerate one and is floored to MIN_HALF_EXTENT by the recovery rule.
    "Plane": ((200.0, MIN_HALF_EXTENT, 300.0), 300.0, 0.0),
}

for shape in SHAPES:
    state = INITIAL_STATES[shape]
    history, converged_at = cycle(shape, state)
    print(f"\n  {shape}: start={state}")
    for i, entry in enumerate(history[1:], start=1):
        marker = "  <= fixed point" if converged_at is not None and i == converged_at + 1 else ""
        print(f"    sync {i}: {entry}{marker}")
    check(f"{shape}: reaches a fixed point on the first sync", converged_at == 0,
          f"converged_at={converged_at}")

# ---------------------------------------------------------------------------------------------
print("\n" + "=" * 78)
print("4. Counter-example: the circumscribed (half-diagonal) rule DIVERGES")
print("=" * 78)

# Reproduces the sync loop with radius = |half_extents| instead of max(half_extents).
radius = 10.0
history = [radius]
for _ in range(6):
    _points, _counts, extent = sphere_geometry(radius)
    half = bound_half_extents(_points)
    radius = math.sqrt(sum(h * h for h in half))  # the circumscribed rule
    history.append(radius)

print(f"  sphere, circumscribed rule: {[round(v, 2) for v in history]}")
growth = history[-1] / history[0]
check(
    "circumscribed rule diverges (this is WHY max is used)",
    growth > 10.0,
    f"grew x{growth:.1f} over {len(history) - 1} syncs - a generated sphere's AABB is a cube",
)

# ---------------------------------------------------------------------------------------------
print("\n" + "=" * 78)
print("5. Non-uniform distortion collapses to one stable radius")
print("=" * 78)

DISTORTIONS = {
    "Sphere": ((240.0, 120.0, 120.0), 0.0, 0.0),
    "Cylinder": ((240.0, 150.0, 80.0), 0.0, 0.0),
}

for shape, (distorted, _r, _h) in DISTORTIONS.items():
    half_extents, radius, half_height = recover_shape_parameters(shape, distorted)
    print(f"\n  {shape}: distorted bound {distorted} -> radius={radius} "
          f"half_height={half_height}")
    check(f"{shape}: radius is the LONGEST bound, not the half-diagonal", radius == max(distorted))
    check(f"{shape}: collapsed state is stable",
          cycle(shape, (half_extents, radius, half_height))[1] == 0)

print("\n" + "=" * 78)
print("5b. Non-uniform scaling is PRESERVED per axis for Box and Plane")
print("=" * 78)

# The radius collapse must NOT leak into the shapes whose geometry is not built from a radius.
# A naive "half_extents = (radius, radius, radius)" for every shape would destroy non-uniform
# scaling on Box and Plane, which is exactly what the user flagged.
PER_AXIS_CASES = {
    "Box": (100.0, 50.0, 75.0),
    "Plane": (200.0, 0.0, 300.0),
}
for shape, distorted in PER_AXIS_CASES.items():
    half_extents, radius, _half_height = recover_shape_parameters(shape, distorted)
    points, _counts, _extent = build_shape_geometry(
        shape, half_extents, radius, 0.0
    )
    measured = bound_half_extents(points)
    print(f"\n  {shape}: distorted {distorted} -> half_extents={half_extents}"
          f"  measured={tuple(round(v, 3) for v in measured)}")
    check(
        f"{shape}: every axis survives (no max collapse on the geometry)",
        all(abs(measured[i] - distorted[i]) < 1e-4 for i in range(3)),
        f"{measured} vs {distorted}",
    )

# And prove the contrast: the single-radius shapes MUST collapse, because they only have one dial.
for shape, distorted, expected in (
    ("Sphere", (240.0, 120.0, 80.0), 240.0),
    ("Cylinder", (240.0, 150.0, 80.0), 240.0),
):
    half_extents, radius, _half_height = recover_shape_parameters(shape, distorted)
    print(f"  {shape}: distorted {distorted} -> radius={radius}")
    check(f"{shape}: collapses to the longest bound", radius == expected)

print("\n" + "=" * 78)
print("6. HARD FLOOR: a degenerate measurement can never reach the shape maths")
print("=" * 78)

for label, raw in (
    ("all zeros", (0.0, 0.0, 0.0)),
    ("flat axis (Plane on Y)", (200.0, 0.0, 300.0)),
    ("sub-unit values", (0.25, 0.5, 0.75)),
    ("negative / malformed bound", (-10.0, -20.0, -30.0)),
    ("no measurable bound (None)", None),
):
    recovered = recover_half_extents(raw)
    radius = recover_shape_parameters("Sphere", raw)[1]
    print(f"  {label:28s} {str(raw):26s} -> {tuple(round(v, 3) for v in recovered)}"
          f"  radius={radius}")
    check(f"{label}: every axis >= {MIN_HALF_EXTENT}",
          all(v >= MIN_HALF_EXTENT for v in recovered))
    check(f"{label}: radius is never zero", radius >= MIN_HALF_EXTENT)

check("an unmeasurable bound yields DEFAULT_HALF_EXTENT",
      recover_half_extents(None) == (DEFAULT_HALF_EXTENT,) * 3,
      f"{recover_half_extents(None)}")
check("the floor does NOT distort a real shape",
      recover_half_extents((250.0, 120.0, 375.0)) == (250.0, 120.0, 375.0))
check("convergence still holds with the floor in place (Plane)",
      cycle("Plane", ((200.0, MIN_HALF_EXTENT, 300.0), 300.0, 0.0))[1] == 0)
check("convergence still holds with the floor in place (Sphere)",
      cycle("Sphere", ((MIN_HALF_EXTENT,) * 3, MIN_HALF_EXTENT, MIN_HALF_EXTENT))[1] == 0)


# ---------------------------------------------------------------------------------------------
# 7. THE TRIGGER COLLIDER reproduces the analytic test it replaced
# ---------------------------------------------------------------------------------------------

print("\n7. TRIGGER COLLIDER: the generated Gprim must mean the same thing as the old Python test")


def inside_collider(spec, point):
    """Whether a point sits inside the collider prim.

    The Gprim keeps UNIT intrinsic dimensions and carries the volume's size in `xformOp:scale`.
    A point in the prim's parent frame is therefore inside when its coordinates divided by the
    scale are inside the unit prim - which is exactly how PhysX will see it.
    """
    sx, sy, sz = spec.scale
    nx, ny, nz = point[0] / sx, point[1] / sy, point[2] / sz

    if spec.gprim_type == "Sphere":
        return (nx * nx + ny * ny + nz * nz) <= spec.radius * spec.radius
    if spec.gprim_type == "Cylinder":
        return (nx * nx + nz * nz) <= spec.radius * spec.radius and abs(ny) <= spec.height * 0.5
    half = spec.size * 0.5
    return abs(nx) <= half and abs(ny) <= half and abs(nz) <= half


def analytic_inside(shape, point, half_extents, radius, half_height):
    """The containment test the migration removed, transcribed from the old controllers."""
    x, y, z = point
    if shape == "Sphere":
        return x * x + y * y + z * z <= radius * radius
    if shape == "Cylinder":
        return x * x + z * z <= radius * radius and abs(y) <= half_height
    if shape == "Plane":
        # The old test used max(100.0, half_height) on the plane's normal axis.
        return (abs(x) <= half_extents[0] and abs(z) <= half_extents[2]
                and abs(y) <= max(100.0, half_height))
    return abs(x) <= half_extents[0] and abs(y) <= half_extents[1] and abs(z) <= half_extents[2]


# Deliberately NON-CUBIC parameters, so a mapping that quietly squared or swapped an axis would
# disagree on the very first sample rather than passing by symmetry.
_CASES = (
    ("Box", (300.0, 150.0, 400.0), 400.0, 150.0),
    ("Sphere", (250.0, 250.0, 250.0), 250.0, 250.0),
    ("Cylinder", (120.0, 90.0, 120.0), 120.0, 90.0),
    ("Plane", (300.0, 1.0, 400.0), 400.0, 0.0),
)

for _shape, _half_extents, _radius, _half_height in _CASES:
    _spec = trigger_collider_spec(_shape, _half_extents, _radius, _half_height)
    _reach = max(max(_half_extents), PLANE_TRIGGER_THICKNESS * 0.5) * 1.2
    _step = _reach / 3.0
    _mismatches = []
    for _ix in range(-3, 4):
        for _iy in range(-3, 4):
            for _iz in range(-3, 4):
                _pt = (_ix * _step, _iy * _step, _iz * _step)
                _by_collider = inside_collider(_spec, _pt)
                _by_analytic = analytic_inside(_shape, _pt, _half_extents, _radius, _half_height)
                if _by_collider != _by_analytic:
                    _mismatches.append((_pt, _by_collider, _by_analytic))
    check(
        f"{_shape}: the collider agrees with the old analytic test on all 343 samples",
        not _mismatches,
        f"{_spec.gprim_type} scale={tuple(round(v, 1) for v in _spec.scale)}"
        + (f" -- {len(_mismatches)} mismatches, first {_mismatches[0]}" if _mismatches else ""),
    )

_spec_by_shape = {s: trigger_collider_spec(s, (100.0, 100.0, 100.0), 100.0, 100.0) for s in SHAPES}
check("Plane becomes a Cube slab, because PhysX has no infinite-plane collider",
      _spec_by_shape["Plane"].gprim_type == "Cube")
check(f"the Plane slab is {PLANE_TRIGGER_THICKNESS} thick, i.e. half thickness 100 - the old Y tolerance",
      _spec_by_shape["Plane"].scale[1] == 100.0)
check("a shape change also changes the Gprim TYPE, which is why the prim is recreated, not edited",
      _spec_by_shape["Box"].gprim_type == "Cube" and _spec_by_shape["Sphere"].gprim_type == "Sphere"
      and _spec_by_shape["Cylinder"].gprim_type == "Cylinder")
check("every Gprim keeps unit intrinsic dimensions, so `scale` alone carries the volume's size",
      all(s.size == 2.0 and s.radius == 1.0 and s.height == 2.0 for s in _spec_by_shape.values()))
check("an unknown shape falls back to the Box collider, matching build_shape_geometry()",
      trigger_collider_spec("Nonsense", (10.0, 20.0, 30.0), 10.0, 20.0).scale == (10.0, 20.0, 30.0))

print("\n" + "=" * 78)
if FAILURES:
    print(f"FAILED ({len(FAILURES)}): " + "; ".join(FAILURES))
    sys.exit(1)
print("ALL VOLUME GEOMETRY TESTS PASSED")
print("=" * 78)
