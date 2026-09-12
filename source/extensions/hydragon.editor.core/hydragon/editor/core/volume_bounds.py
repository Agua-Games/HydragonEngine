"""
Hydragon Engine - Volume bounds measurement (USD, no Kit)

This is step (a) of the volume-gizmo plan: the ASSET stays the sole author of the wireframe
geometry (`volumes/force_bounds` / `volumes/kill_bounds` BasisCurves) and Python only DERIVES the
volume's shape parameters from it. Nothing here regenerates geometry.

Why a single shared module
--------------------------
`force_volume_controller` and `kill_volume_controller` each used to derive the volume dimensions
their own way, and the viewport overlay derived them a third way. They disagreed. Everything now
goes through `measure_shape_parameters()`, so gameplay and the gizmo cannot drift apart again.

The frame is the whole point
----------------------------
The measurement is taken with `ComputeRelativeBound(prim, prim)`, i.e. the bound of the prim's own
subtree expressed in the prim's OWN frame with the prim's own transform EXCLUDED. Measured:

    /A (scale 3) -> /A/V (scale 5) -> child of size 60
        ComputeWorldBound(/A/V)                  = 900   (all transforms)
        ComputeLocalBound(/A/V)                  = 300   (own transform applied = PARENT frame)
        ComputeRelativeBound(/A/V, /A/V)         =  60   <- the frame we need

That is the correct frame because the volume's transform is inherited by whatever consumes these
numbers: a point is transformed into the volume's local space before being compared. Taking a
WORLD bound would apply the volume's own scale a second time. Measured on a real asset, that
mistake made a force volume's effective region 6x too large in every axis.

Two traps this module deliberately handles
------------------------------------------
1. VISIBILITY. `ignoreVisibility=True` is passed always. With the default (False), an `invisible`
   child contributes nothing and the range comes back EMPTY - so the existing Shift+V hide toggle
   would collapse a volume to zero.
2. CACHING. `UsdGeom.BBoxCache` caches its results, and a long-lived cache keeps returning STALE
   bounds after the geometry changes (measured: authoring a new `extent` had no effect). A fresh
   cache is built per measurement. This runs on change, not per frame, so the cost is irrelevant
   and correctness is not.
"""

from typing import Optional, Tuple

try:
    from pxr import Usd, UsdGeom

    HAS_PXR = True
except ImportError:  # pragma: no cover - only true outside a USD environment
    Usd = None
    UsdGeom = None
    HAS_PXR = False

from .volume_geometry import Vec3, recover_shape_parameters

#: Attribute holding the authored shape token, per volume type.
SHAPE_ATTRIBUTES = {
    "Force": "force:volumeShape",
    "Kill": "kill:volumeShape",
}

#: Fallback shape when the attribute is missing or empty.
DEFAULT_SHAPE = "Box"

#: Where the wireframe geometry lives, per volume type. Shared so the overlay and the gameplay
#: modules can never disagree about which prim defines the volume's size.
BOUNDS_PRIM_REL_PATH = {
    "Force": "volumes/force_bounds",
    "Kill": "volumes/kill_bounds",
}

#: Purposes that count as real geometry, named as `UsdGeom.Tokens` attributes.
#:
#: `guide` and `proxy` are deliberately excluded: they are non-rendered, non-picked helper
#: geometry, so they must not influence the volume's real size. The shipped assets author the
#: bounds with `purpose = "default"`, which this set covers.
#: NOTE: `UsdGeom.Tokens` spells the default purpose `default_` (trailing underscore).
BOUND_PURPOSE_TOKENS = ("default_", "render")


def read_shape(volume_prim, volume_type: str) -> str:
    """Reads the authored shape token for a volume prim, defaulting to Box."""
    attribute_name = SHAPE_ATTRIBUTES.get(volume_type)
    if not attribute_name or not volume_prim or not hasattr(volume_prim, "HasAttribute"):
        return DEFAULT_SHAPE
    if not volume_prim.HasAttribute(attribute_name):
        return DEFAULT_SHAPE
    attribute = volume_prim.GetAttribute(attribute_name)
    if not attribute or not attribute.IsValid():
        return DEFAULT_SHAPE
    value = attribute.Get()
    return str(value) if value else DEFAULT_SHAPE


def find_bounds_prim(volume_prim, volume_type: str):
    """Returns the wireframe prim for a volume, or None when there is none.

    Tolerates the legacy flat layout, where the bounds prim was a direct child of the volume
    rather than living under `volumes/`.
    """
    relative_path = BOUNDS_PRIM_REL_PATH.get(volume_type)
    if not relative_path or not volume_prim or not hasattr(volume_prim, "GetPrimAtPath"):
        return None

    for candidate in (relative_path, relative_path.rsplit("/", 1)[-1]):
        prim = volume_prim.GetPrimAtPath(candidate)
        if prim and prim.IsValid():
            return prim
    return None


def measure_bound(volume_prim, volume_type: str = "Force", reference_prim=None):
    """Returns (centre, half_extents) of the volume's WIREFRAME, in `reference_prim`'s frame.

    `reference_prim` defaults to the volume root, which is the frame the gameplay overlap test
    works in (it transforms a world point into the volume root's local space). A caller that draws
    in a different frame - the viewport overlay builds its geometry in the `volumes` group's frame -
    must say so, otherwise the two would silently disagree about the volume's size.

    The wireframe prim is measured explicitly rather than measuring the root's whole subtree: a
    subtree measurement would silently absorb any sibling, so the moment a PhysX collider (or any
    other helper) is parented under the volume, the volume's size would change without anyone
    touching the wireframe.

    The centre is returned because the geometry is NOT necessarily centred on the volume root: the
    shipped asset offsets `force_bounds` with `xformOp:translate`. `GetSize()` would hide that.

    Returns None when no wireframe prim exists.
    """
    if not HAS_PXR or not volume_prim or not volume_prim.IsValid():
        return None

    target = find_bounds_prim(volume_prim, volume_type)
    if target is None:
        return None

    reference = reference_prim if reference_prim is not None and reference_prim.IsValid() else volume_prim
    purposes = [getattr(UsdGeom.Tokens, name) for name in BOUND_PURPOSE_TOKENS]
    # A FRESH cache every time: see the caching trap in the module docstring.
    cache = UsdGeom.BBoxCache(Usd.TimeCode.Default(), purposes, False, True)
    aligned = cache.ComputeRelativeBound(target, reference).ComputeAlignedRange()
    if aligned.IsEmpty():
        return None

    mid = aligned.GetMidpoint()
    size = aligned.GetSize()
    centre = (mid[0], mid[1], mid[2])
    half_extents = (size[0] * 0.5, size[1] * 0.5, size[2] * 0.5)
    return centre, half_extents


def measure_relative_half_extents(
    volume_prim, volume_type: str = "Force", reference_prim=None
) -> Optional[Vec3]:
    """Half-extents of the volume's wireframe in `reference_prim`'s frame, or None.

    None means "nothing measurable yet", which `recover_shape_parameters` turns into a sane
    default rather than an error, because it is a normal transient state.
    """
    bound = measure_bound(volume_prim, volume_type, reference_prim)
    return None if bound is None else bound[1]


def measure_shape_parameters(
    volume_prim, volume_type: str, default_half_extent: Optional[float] = None, reference_prim=None
) -> Tuple[Vec3, float, float]:
    """The volume's (half_extents, radius, half_height) derived from its actual geometry.

    This is the single source of truth for a volume's size. Both the gameplay overlap test and the
    viewport gizmo must use it; they only differ in the frame they declare via `reference_prim`.

    `default_half_extent` lets a caller keep its own legacy fallback for a volume that has no
    measurable geometry yet; the module default is used when it is omitted.
    """
    shape = read_shape(volume_prim, volume_type)
    half_extents = measure_relative_half_extents(volume_prim, volume_type, reference_prim)
    if half_extents is None and default_half_extent is not None:
        half_extents = (default_half_extent, default_half_extent, default_half_extent)
    return recover_shape_parameters(shape, half_extents)


def describe(volume_prim, volume_type: str) -> str:
    """One-line diagnostic used by the tests and by verbose logging."""
    shape = read_shape(volume_prim, volume_type)
    bound = measure_bound(volume_prim, volume_type)
    centre = bound[0] if bound else None
    half_extents, radius, half_height = recover_shape_parameters(
        shape, None if bound is None else bound[1]
    )
    return (
        f"shape={shape} centre={centre} "
        f"half_extents={tuple(round(v, 3) for v in half_extents)} "
        f"radius={round(radius, 3)} half_height={round(half_height, 3)}"
    )
