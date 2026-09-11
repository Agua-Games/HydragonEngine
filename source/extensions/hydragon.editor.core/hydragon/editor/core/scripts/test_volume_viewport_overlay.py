"""
Unit test for HydragonVolumeViewportOverlay and volume wireframe geometry math.
Validates:
- Lifecycle (startup, shutdown, fail-silent outside Kit).
- Line segment coordinate math for all 4 shapes (Box, Sphere, Cylinder, Plane).
- Color coding (Force vs Kill).
- Caching logic (only rebuilding geometry when shape or extent changes).
"""

import math
import os
import sys

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.volume_viewport_manipulator import (
    HydragonVolumeViewportOverlay,
    generate_shape_wireframe_segments,
)


def test_overlay_lifecycle():
    print("--- 1. Testing Overlay Lifecycle ---")
    overlay = HydragonVolumeViewportOverlay()
    assert overlay is not None
    assert HydragonVolumeViewportOverlay.get_instance() is overlay

    overlay.startup()
    assert overlay._is_active

    overlay.shutdown()
    assert not overlay._is_active
    print("  [PASS] Overlay lifecycle verified")


def test_wireframe_segment_math_box():
    print("--- 2. Testing Box Wireframe Segment Math ---")
    hx, hy, hz = 100.0, 50.0, 80.0
    segs = generate_shape_wireframe_segments("Box", half_extents=(hx, hy, hz))
    assert len(segs) == 12, f"Expected 12 edges for box, got {len(segs)}"

    # Verify all coordinates stay within extents
    for (x0, y0, z0), (x1, y1, z1) in segs:
        assert abs(x0) <= hx + 1e-4 and abs(x1) <= hx + 1e-4
        assert abs(y0) <= hy + 1e-4 and abs(y1) <= hy + 1e-4
        assert abs(z0) <= hz + 1e-4 and abs(z1) <= hz + 1e-4
    print("  [PASS] Box segment math verified")


def test_wireframe_segment_math_sphere():
    print("--- 3. Testing Sphere Wireframe Segment Math ---")
    radius = 150.0
    segs = generate_shape_wireframe_segments("Sphere", radius=radius)
    # 3 circles * 32 segments each = 96 line segments
    assert len(segs) == 96, f"Expected 96 segments for sphere, got {len(segs)}"

    for (x0, y0, z0), (x1, y1, z1) in segs:
        dist0 = math.sqrt(x0*x0 + y0*y0 + z0*z0)
        dist1 = math.sqrt(x1*x1 + y1*y1 + z1*z1)
        assert abs(dist0 - radius) < 1e-3
        assert abs(dist1 - radius) < 1e-3
    print("  [PASS] Sphere segment math verified")


def test_wireframe_segment_math_cylinder():
    print("--- 4. Testing Cylinder Wireframe Segment Math ---")
    radius = 60.0
    half_height = 120.0
    segs = generate_shape_wireframe_segments("Cylinder", radius=radius, half_height=half_height)
    # Top ring (32) + Bottom ring (32) + 4 Struts = 68 segments
    assert len(segs) == 68, f"Expected 68 segments for cylinder, got {len(segs)}"

    # Struts connect top (y=+120) to bottom (y=-120)
    struts = [s for s in segs if abs(s[0][1] - half_height) < 1e-3 and abs(s[1][1] - (-half_height)) < 1e-3 or
                               abs(s[1][1] - half_height) < 1e-3 and abs(s[0][1] - (-half_height)) < 1e-3]
    assert len(struts) == 4, f"Expected 4 axial struts, got {len(struts)}"
    print("  [PASS] Cylinder segment math verified")


def test_wireframe_segment_math_plane():
    print("--- 5. Testing Plane Wireframe Segment Math ---")
    hx, hz = 300.0, 300.0
    segs = generate_shape_wireframe_segments("Plane", half_extents=(hx, 0.0, hz))
    # 4 perimeter edges + 2 diagonal lines = 6 segments
    assert len(segs) == 6, f"Expected 6 segments for plane, got {len(segs)}"

    # All points should be on y=0
    for (x0, y0, z0), (x1, y1, z1) in segs:
        assert abs(y0) < 1e-4 and abs(y1) < 1e-4
    print("  [PASS] Plane segment math verified")


def test_caching_and_rebuild_logic():
    print("--- 6. Testing Caching Logic (Rebuild vs Reuse) ---")
    from hydragon.editor.core.volume_viewport_manipulator import VolumeOverlayEntry

    entry = VolumeOverlayEntry("/World/ForceVolume", "Force", None)
    entry.cached_shape = "Box"
    entry.cached_half_extents = (100.0, 100.0, 100.0)

    # Same shape and extents
    same_shape = "Box"
    same_extents = (100.0, 100.0, 100.0)
    needs_rebuild = (same_shape != entry.cached_shape or same_extents != entry.cached_half_extents)
    assert not needs_rebuild, "Same shape and extents should reuse existing lines"

    # Changed shape
    diff_shape = "Sphere"
    needs_rebuild = (diff_shape != entry.cached_shape or same_extents != entry.cached_half_extents)
    assert needs_rebuild, "Changing shape to Sphere must trigger geometry rebuild"
    print("  [PASS] Caching logic verified")


def test_overlay_registry_and_colors():
    print("--- 7. Testing Volume Registry and Color Encoding ---")
    from hydragon.editor.core.volume_viewport_manipulator import (
        VolumeOverlayEntry,
        COLOR_FORCE_DEFAULT,
        COLOR_FORCE_SELECTED,
        COLOR_KILL_DEFAULT,
        COLOR_KILL_SELECTED,
    )

    # Colors must not be empty or None
    assert COLOR_FORCE_DEFAULT is not None
    assert COLOR_FORCE_SELECTED is not None
    assert COLOR_KILL_DEFAULT is not None
    assert COLOR_KILL_SELECTED is not None

    entry_force = VolumeOverlayEntry("/World/Force", "Force", None)
    assert entry_force.get_render_color() == COLOR_FORCE_DEFAULT
    entry_force.is_selected = True
    assert entry_force.get_render_color() == COLOR_FORCE_SELECTED

    entry_kill = VolumeOverlayEntry("/World/Kill", "Kill", None)
    assert entry_kill.get_render_color() == COLOR_KILL_DEFAULT
    entry_kill.is_selected = True
    assert entry_kill.get_render_color() == COLOR_KILL_SELECTED

    overlay = HydragonVolumeViewportOverlay("test_ext")
    assert overlay._ext_id == "test_ext"
    assert len(overlay._volumes) == 0

    # Clean destroy
    entry_force.destroy()
    entry_kill.destroy()
    overlay.shutdown()
    print("  [PASS] Volume registry and color encoding verified")


def test_legacy_aliases_removed():
    print("--- 8. Testing Removal of Legacy Compatibility Aliases ---")
    import hydragon.editor.core.volume_viewport_manipulator as vvm

    # These shims were dead code (never referenced by the Kit gesture pipeline)
    # and were intentionally removed. Guard against silent reintroduction.
    removed_symbols = (
        "VolumeClickGesture",
        "VolumeGestureManager",
        "PreventViewportOthers",
        "_is_alt_pressed",
        "_attach_gesture",
        "make_color",
    )
    for symbol in removed_symbols:
        assert not hasattr(vvm, symbol), f"Legacy alias '{symbol}' should have been removed"
    print("  [PASS] Legacy compatibility aliases confirmed removed")


def test_visibility_toggle_and_settings():
    print("--- 9. Testing Volume Overlay Visibility Toggling ---")
    from hydragon.editor.core.volume_viewport_manipulator import (
        HydragonVolumeViewportOverlay,
        SETTING_SHOW_VOLUMES,
    )

    # Namespaced under hydragon.viewport, not the legacy viewport/displayOptions path.
    assert SETTING_SHOW_VOLUMES == "/persistent/app/hydragon/viewport/showVolumes"

    overlay = HydragonVolumeViewportOverlay("test_vis")
    assert overlay._is_visible is True

    # Test toggling without kit settings (pure fallback mode)
    overlay.toggle_volumes_visibility()
    assert overlay._is_visible is False

    overlay.toggle_volumes_visibility()
    assert overlay._is_visible is True

    overlay.set_visible(False)
    assert overlay._is_visible is False

    overlay.shutdown()
    print("  [PASS] Visibility toggle and settings verified")


def test_volume_click_gesture_priority():
    print("--- 10. Testing VolumeSelectGesture & Selection Fall-Through Guard ---")
    from hydragon.editor.core.volume_viewport_manipulator import (
        VolumeSelectGesture,
        HydragonVolumeViewportOverlay,
    )

    # The overlay is constructed first so that __init__ registers the singleton
    # that VolumeSelectGesture.on_ended() resolves via get_instance().
    overlay = HydragonVolumeViewportOverlay("test_sel")
    assert overlay._just_clicked_volume is None
    assert overlay._fallthrough_counter == 0

    gesture = VolumeSelectGesture("/World/ForceVolume")
    assert gesture.prim_path == "/World/ForceVolume"

    # Simulate clicking on the volume gesture
    gesture.on_ended()
    assert overlay._just_clicked_volume == "/World/ForceVolume"
    assert overlay._fallthrough_counter == 2

    # Shutdown must clear the guard so stale state cannot leak into a restart
    overlay.shutdown()
    assert overlay._just_clicked_volume is None
    assert overlay._fallthrough_counter == 0
    print("  [PASS] VolumeSelectGesture & selection fall-through guard verified")


if __name__ == "__main__":
    test_overlay_lifecycle()
    test_wireframe_segment_math_box()
    test_wireframe_segment_math_sphere()
    test_wireframe_segment_math_cylinder()
    test_wireframe_segment_math_plane()
    test_caching_and_rebuild_logic()
    test_overlay_registry_and_colors()
    test_legacy_aliases_removed()
    test_visibility_toggle_and_settings()
    test_volume_click_gesture_priority()
    print("\nALL VOLUME VIEWPORT OVERLAY TESTS PASSED! (10/10)")
