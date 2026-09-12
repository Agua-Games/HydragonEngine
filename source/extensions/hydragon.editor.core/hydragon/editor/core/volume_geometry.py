"""
Hydragon Engine - Volume wireframe geometry (pure, Kit-independent)

Single source of truth for three directions of the same conversion:

    shape parameters  ->  BasisCurves points / vertex counts / extent
    shape parameters  ->  PhysX trigger collider (Gprim type + scale)
    bounding box      ->  shape parameters

Deliberately free of Kit and pxr imports (plain tuples in, plain tuples out) so it is fully
testable outside the editor:

    _build/target-deps/python/python.exe source/extensions/hydragon.editor.core/hydragon/editor/core/scripts/test_volume_geometry.py

Two invariants drive the design.

1. SCHEMA VALIDITY
   For `type = linear` + `wrap = nonperiodic` the UsdGeom schema requires
   `curveVertexCounts[i] > 2`. A bare 2-vertex segment is therefore INVALID geometry. Every
   segment here is authored as (start, midpoint, end): two collinear sub-segments, which draw
   exactly the same line while satisfying the schema.

2. CONVERGENCE
   The data flow is a CYCLE, not a one-way pipeline:

       shape params -> points/extent -> bounding box -> shape params

   so the recovery rule must be a FIXED POINT of the composition. `max(half_extents)` is one.
   The circumscribed half-diagonal is NOT: a sphere generated as three orthogonal circles has a
   cube-shaped AABB whose diagonal is r*sqrt(3), not r, so recovering it grows the sphere by 73%
   on every sync. Measured in `scratch/verify_radius_convergence.py` (x27 growth in 6 syncs).

3. HARD FLOOR
   Every recovered half-extent is clamped to at least MIN_HALF_EXTENT. A degenerate measurement
   (a Plane genuinely has no thickness on Y; an empty bound yields zeros) must never reach the
   shape maths, where a zero radius or a zero divisor would produce a collapsed volume or a
   division by zero. One clamp here is cheaper and more predictable than guarding every formula
   that consumes these numbers.
"""

import math
from typing import Dict, List, NamedTuple, Sequence, Tuple

#: Vertex count for each of the three orthogonal circles of a sphere wireframe.
SPHERE_SEGMENTS = 32

#: Vertex count for each ring of a cylinder wireframe.
CYLINDER_SEGMENTS = 32

Vec3 = Tuple[float, float, float]

#: Hard lower bound for a recovered half-extent, in stage units.
#:
#: Guarantees that no degenerate measurement can reach the shape maths: no zero radius, no zero
#: divisor, no volume collapsed to nothing. Deliberately a plain floor rather than a geometric
#: correction, because a floor is trivially predictable and cannot introduce an unstable rule.
MIN_HALF_EXTENT = 1.0

#: Fallback used when no bound could be measured at all (empty range, missing geometry).
#: That is a NORMAL state right after a prim is created, not an error.
DEFAULT_HALF_EXTENT = 100.0

#: Full thickness of the trigger collider generated for a `Plane` volume, in stage units.
#:
#: PhysX has no infinite-plane collider, so a `Plane` volume is represented by a thin BOX slab.
#: The value is 200.0 - i.e. a HALF thickness of 100.0 - because that reproduces, exactly, the
#: half-extent the analytic test used for the plane's normal axis (`max(100.0, half_height)`).
#: A genuinely thin slab (say 10.0) is a deliberate gameplay change, not a refactor: it would let
#: a body cross the plane without ever overlapping it at a sampling instant.
PLANE_TRIGGER_THICKNESS = 200.0

#: The four volume shapes supported by both the Force and Kill volume schemas.
SHAPES = ("Box", "Sphere", "Cylinder", "Plane")


# ---------------------------------------------------------------------------------------------
# shape parameters -> geometry
# ---------------------------------------------------------------------------------------------


def _segment(p0: Vec3, p1: Vec3) -> List[Vec3]:
    """A single line as three collinear points: start, midpoint, end.

    Two sub-segments instead of one, because a 2-vertex linear nonperiodic curve is invalid.
    The midpoint form is preferred over repeating a vertex because it draws each half exactly
    once instead of overdrawing the whole segment.
    """
    mid = ((p0[0] + p1[0]) * 0.5, (p0[1] + p1[1]) * 0.5, (p0[2] + p1[2]) * 0.5)
    return [p0, mid, p1]


def _circle(axis_a: int, axis_b: int, radius: float, segments: int) -> List[Vec3]:
    """A closed circle in the plane spanned by two axes, as segments + 1 points."""
    points: List[Vec3] = []
    for i in range(segments + 1):
        theta = 2.0 * math.pi * (i / segments)
        p = [0.0, 0.0, 0.0]
        p[axis_a] = math.cos(theta) * radius
        p[axis_b] = math.sin(theta) * radius
        points.append((p[0], p[1], p[2]))
    return points


def box_geometry(half_extents: Vec3) -> Tuple[List[Vec3], List[int], List[Vec3]]:
    """The 12 edges of a box, each as a valid 3-point segment."""
    hx, hy, hz = (abs(float(v)) for v in half_extents)
    corners = [
        (-hx, -hy, -hz), (hx, -hy, -hz), (hx, -hy, hz), (-hx, -hy, hz),
        (-hx, hy, -hz), (hx, hy, -hz), (hx, hy, hz), (-hx, hy, hz),
    ]
    edges = [
        (0, 1), (1, 2), (2, 3), (3, 0),          # bottom rectangle
        (4, 5), (5, 6), (6, 7), (7, 4),          # top rectangle
        (0, 4), (1, 5), (2, 6), (3, 7),          # vertical edges
    ]
    points: List[Vec3] = []
    counts: List[int] = []
    for a, b in edges:
        segment = _segment(corners[a], corners[b])
        points.extend(segment)
        counts.append(len(segment))

    extent = [(-hx, -hy, -hz), (hx, hy, hz)]
    return points, counts, extent


def sphere_geometry(radius: float) -> Tuple[List[Vec3], List[int], List[Vec3]]:
    """Three orthogonal circles. The AABB of this is a CUBE of side 2r - see the module docstring."""
    r = abs(float(radius))
    points: List[Vec3] = []
    counts: List[int] = []
    for axis_a, axis_b in ((0, 1), (0, 2), (1, 2)):
        circle = _circle(axis_a, axis_b, r, SPHERE_SEGMENTS)
        points.extend(circle)
        counts.append(len(circle))

    extent = [(-r, -r, -r), (r, r, r)]
    return points, counts, extent


def cylinder_geometry(radius: float, half_height: float) -> Tuple[List[Vec3], List[int], List[Vec3]]:
    """Two rings plus four axial struts, all schema-valid."""
    r = abs(float(radius))
    h = abs(float(half_height))
    points: List[Vec3] = []
    counts: List[int] = []

    # Top ring (y = +h) and bottom ring (y = -h), in the XZ plane.
    for y in (h, -h):
        ring: List[Vec3] = []
        for i in range(CYLINDER_SEGMENTS + 1):
            theta = 2.0 * math.pi * (i / CYLINDER_SEGMENTS)
            ring.append((math.cos(theta) * r, y, math.sin(theta) * r))
        points.extend(ring)
        counts.append(len(ring))

    # Four axial struts, each a valid 3-point segment.
    for angle in (0.0, math.pi * 0.5, math.pi, math.pi * 1.5):
        cx = math.cos(angle) * r
        cz = math.sin(angle) * r
        segment = _segment((cx, -h, cz), (cx, h, cz))
        points.extend(segment)
        counts.append(len(segment))

    extent = [(-r, -h, -r), (r, h, r)]
    return points, counts, extent


def plane_geometry(half_extents: Vec3) -> Tuple[List[Vec3], List[int], List[Vec3]]:
    """A perimeter rectangle plus two diagonals, on the XZ plane (y = 0)."""
    hx = abs(float(half_extents[0]))
    hz = abs(float(half_extents[2]))

    perimeter = [(-hx, 0.0, -hz), (hx, 0.0, -hz), (hx, 0.0, hz), (-hx, 0.0, hz), (-hx, 0.0, -hz)]
    points: List[Vec3] = list(perimeter)
    counts: List[int] = [len(perimeter)]

    for p0, p1 in (((-hx, 0.0, -hz), (hx, 0.0, hz)), ((hx, 0.0, -hz), (-hx, 0.0, hz))):
        segment = _segment(p0, p1)
        points.extend(segment)
        counts.append(len(segment))

    extent = [(-hx, 0.0, -hz), (hx, 0.0, hz)]
    return points, counts, extent


def build_shape_geometry(
    shape: str,
    half_extents: Vec3 = (100.0, 100.0, 100.0),
    radius: float = 100.0,
    half_height: float = 100.0,
) -> Tuple[List[Vec3], List[int], List[Vec3]]:
    """Dispatches to the per-shape builder. Unknown shapes fall back to Box, as before."""
    if shape == "Sphere":
        return sphere_geometry(radius)
    if shape == "Cylinder":
        return cylinder_geometry(radius, half_height)
    if shape == "Plane":
        return plane_geometry(half_extents)
    return box_geometry(half_extents)


# ---------------------------------------------------------------------------------------------
# shape parameters -> PhysX trigger collider
# ---------------------------------------------------------------------------------------------


class TriggerSpec(NamedTuple):
    """The Gprim a volume's PhysX trigger collider is built from, plus its intrinsic size.

    The collider exists because PhysX cannot cook a collision volume from `BasisCurves` - a trigger
    needs a real Gprim. The mapping below is chosen so each shape reproduces, EXACTLY, the analytic
    containment test it replaces:

        Box       abs(rel) <= half_extents
        Sphere    dist_sq <= radius^2
        Cylinder  r_sq <= radius^2 and abs(rel_y) <= half_height
        Plane     abs(rel_x) <= hx and abs(rel_z) <= hz and abs(rel_y) <= thickness / 2

    `scale` is the prim's own `xformOp:scale`. The Gprim's intrinsic dimensions stay at unit values
    so that `scale` alone carries the volume's size, which keeps the size readable from a single
    attribute and makes the wireframe and the collider provably derive from the same numbers.
    """

    gprim_type: str
    scale: Vec3
    size: float = 2.0
    radius: float = 1.0
    height: float = 2.0


def trigger_collider_spec(
    shape: str,
    half_extents: Vec3,
    radius: float,
    half_height: float,
) -> TriggerSpec:
    """The trigger collider for a shape, derived from the SAME parameters as its wireframe.

    Because both representations come out of this module, the drawn cage and the region PhysX
    reports on cannot drift apart.

    `Plane` becomes a BOX slab: PhysX has no infinite-plane collider. See
    `PLANE_TRIGGER_THICKNESS` for why the slab is 200 units thick rather than hairline.
    """
    hx, hy, hz = (abs(float(v)) for v in half_extents)

    if shape == "Sphere":
        r = abs(float(radius))
        return TriggerSpec("Sphere", (r, r, r), radius=1.0)

    if shape == "Cylinder":
        r = abs(float(radius))
        h = abs(float(half_height))
        return TriggerSpec("Cylinder", (r, h, r), radius=1.0, height=2.0)

    if shape == "Plane":
        half_thickness = PLANE_TRIGGER_THICKNESS * 0.5
        return TriggerSpec("Cube", (hx, half_thickness, hz))

    return TriggerSpec("Cube", (hx, hy, hz))


# ---------------------------------------------------------------------------------------------
# geometry -> shape parameters (the inverse direction; must be a FIXED POINT of the above)
# ---------------------------------------------------------------------------------------------


def recover_half_extents(bound_half_extents) -> Vec3:
    """Half-extents from a measured bound, floored at MIN_HALF_EXTENT.

    `bound_half_extents` may be None, meaning "no measurable bound" - an empty range, or geometry
    that is invisible or not there yet. That is not an error, so it yields DEFAULT_HALF_EXTENT
    rather than raising or collapsing to zero.

    The floor also absorbs the legitimate zero of a Plane's normal axis: the flat axis comes back
    as MIN_HALF_EXTENT, which the plane generator ignores because it never uses Y.
    """
    if bound_half_extents is None:
        return (DEFAULT_HALF_EXTENT, DEFAULT_HALF_EXTENT, DEFAULT_HALF_EXTENT)
    return tuple(  # type: ignore[return-value]
        max(MIN_HALF_EXTENT, abs(float(v))) for v in bound_half_extents
    )


def recover_shape_parameters(
    shape: str, bound_half_extents
) -> Tuple[Vec3, float, float]:
    """Turns a measured bound back into (half_extents, radius, half_height).

    The radius uses `max(...)`, NOT the circumscribed half-diagonal. The half-diagonal is not a
    fixed point of `build_shape_geometry`: a generated sphere's bound is a cube, so its diagonal
    is r*sqrt(3). Using it would inflate the volume a little on every sync. `max` is stable for
    every shape and still means "the longest bound as the base", which is the intended semantics.

    This is the rule that keeps the edit -> bound -> edit cycle convergent, so it must not be
    "improved" into a geometric radius without re-checking convergence.
    """
    hx, hy, hz = recover_half_extents(bound_half_extents)

    if shape == "Sphere":
        # Any axis may have been distorted, so the sphere grows to the longest one.
        r = max(hx, hy, hz)
        return (r, r, r), r, r

    if shape == "Cylinder":
        # Radius lives in X/Z, height in Y. A distortion on either X or Z collapses to the larger.
        r = max(hx, hz)
        return (r, hy, r), r, hy

    if shape == "Plane":
        # Y is the plane's normal axis: the geometry genuinely has no thickness there, so a
        # measured 0 on Y is correct and must not be mistaken for a degenerate or empty bound.
        return (hx, hy, hz), max(hx, hz), 0.0

    return (hx, hy, hz), max(hx, hy, hz), hy


def describe(shape: str) -> Dict[str, str]:
    """Human-readable summary used by the tests and by log messages."""
    return {
        "shape": shape,
        "radius_rule": "max(half_extents) - convergent fixed point, NOT the half-diagonal",
        "segment_rule": "every segment is 3 collinear points, because linear nonperiodic needs > 2",
        "floor": f"every half-extent is clamped to >= {MIN_HALF_EXTENT} stage unit(s)",
    }
