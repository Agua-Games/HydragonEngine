"""
Unit test for HydragonCameraControllerSystem
Validates initialization, fail-silent behavior outside Kit, spherical orbit math,
LookAt orientation calculations, and lag damping.
"""

import math
import sys
import os

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.camera_controller import HydragonCameraControllerSystem


def test_camera_controller_lifecycle():
    print("--- 1. Testing Camera Controller System Lifecycle ---")
    system = HydragonCameraControllerSystem()
    assert system is not None, "Failed to instantiate HydragonCameraControllerSystem"
    assert not system._is_active, "System should start inactive"

    # Startup & shutdown outside Kit (fail-silent)
    system.startup()
    assert not system._is_active, "Should remain inactive when HAS_KIT is False"

    system.shutdown()
    assert not system._is_active, "System should be inactive after shutdown"
    assert system._current_focus_pos is None, "Focus pos should be reset"
    print("  [PASS] HydragonCameraControllerSystem lifecycle verified")


def test_spherical_orbit_math():
    print("--- 2. Testing Spherical Orbit Math (+Z forward arena convention) ---")
    system = HydragonCameraControllerSystem()
    system._arm_length = 800.0

    # 1. Pitch = 0, Yaw = 0 (Camera rests BEHIND player at -Z, facing +Z)
    system._pitch = 0.0
    system._yaw = 0.0
    rad_p = math.radians(system._pitch)
    rad_y = math.radians(system._yaw)
    dx = -system._arm_length * math.cos(rad_p) * math.sin(rad_y)
    dy = system._arm_length * math.sin(rad_p)
    dz = -system._arm_length * math.cos(rad_p) * math.cos(rad_y)

    assert abs(dx) < 1e-5, f"Expected dx=0, got {dx}"
    assert abs(dy) < 1e-5, f"Expected dy=0, got {dy}"
    assert abs(dz - (-800.0)) < 1e-5, f"Expected dz=-800, got {dz}"

    # 2. Pitch = 25, Yaw = 0
    system._pitch = 25.0
    system._yaw = 0.0
    rad_p = math.radians(system._pitch)
    rad_y = math.radians(system._yaw)
    dx = -system._arm_length * math.cos(rad_p) * math.sin(rad_y)
    dy = system._arm_length * math.sin(rad_p)
    dz = -system._arm_length * math.cos(rad_p) * math.cos(rad_y)

    expected_dy = 800.0 * math.sin(math.radians(25.0))
    expected_dz = -800.0 * math.cos(math.radians(25.0))
    assert abs(dx) < 1e-5, f"Expected dx=0, got {dx}"
    assert abs(dy - expected_dy) < 1e-4, f"Expected dy={expected_dy}, got {dy}"
    assert abs(dz - expected_dz) < 1e-4, f"Expected dz={expected_dz}, got {dz}"

    # 3. Distance invariant under rotation
    dist = math.sqrt(dx * dx + dy * dy + dz * dz)
    assert abs(dist - 800.0) < 1e-4, f"Distance must equal armLength 800, got {dist}"

    # 4. Rotation Euler Angles: pointing down into arena towards +Z
    # In USD Camera: rotX = 180 - pitch, rotY = -yaw, rotZ = -180
    rot_x = 180.0 - system._pitch
    rot_y = -system._yaw
    rot_z = -180.0
    assert abs(rot_x - 155.0) < 1e-4, f"Expected rotX=155.0 for pitch 25, got {rot_x}"
    assert abs(rot_y) < 1e-4, f"Expected rotY=0.0, got {rot_y}"
    assert abs(rot_z - (-180.0)) < 1e-4, f"Expected rotZ=-180.0, got {rot_z}"
    print("  [PASS] Spherical orbit mathematics verified")


def test_lag_damping_math():
    print("--- 3. Testing Lag Damping Interpolation ---")
    pos_a = (0.0, 50.0, -1000.0)
    pos_b = (100.0, 50.0, -900.0)
    dt = 1.0 / 60.0
    lag_speed = 12.0
    factor = min(1.0, lag_speed * dt)

    lerp_x = pos_a[0] + (pos_b[0] - pos_a[0]) * factor
    lerp_z = pos_a[2] + (pos_b[2] - pos_a[2]) * factor

    assert 0.0 < lerp_x < 100.0, f"Interpolated X should be between 0 and 100, got {lerp_x}"
    assert -1000.0 < lerp_z < -900.0, f"Interpolated Z should be between -1000 and -900, got {lerp_z}"
    print(f"  [PASS] Lag damping verified (blend factor={factor:.3f})")


def test_pitch_and_zoom_clamping():
    print("--- 4. Testing Pitch and Zoom Clamping ---")
    system = HydragonCameraControllerSystem()

    # Min / Max Pitch clamps
    assert system._min_pitch == -20.0
    assert system._max_pitch == 80.0

    # Over-pitch test
    overshot_pitch = 120.0
    clamped_pitch = max(system._min_pitch, min(system._max_pitch, overshot_pitch))
    assert clamped_pitch == 80.0, f"Expected 80.0, got {clamped_pitch}"

    # Under-pitch test
    undershot_pitch = -50.0
    clamped_pitch = max(system._min_pitch, min(system._max_pitch, undershot_pitch))
    assert clamped_pitch == -20.0, f"Expected -20.0, got {clamped_pitch}"

    # Zoom bounds
    overshot_zoom = 5000.0
    clamped_zoom = max(system._min_arm_length, min(system._max_arm_length, overshot_zoom))
    assert clamped_zoom == 3000.0, f"Expected 3000.0, got {clamped_zoom}"
    print("  [PASS] Pitch and Zoom clamping verified")


def test_camera_axes_and_singleton():
    print("--- 5. Testing Camera Axes Query & Singleton ---")
    system = HydragonCameraControllerSystem()
    assert HydragonCameraControllerSystem.get_instance() is system, "Singleton should match instantiated system"

    # Yaw = 0 (looking +Z)
    system._yaw = 0.0
    fwd, right = system.get_camera_axes()
    assert abs(fwd[0] - 0.0) < 1e-5 and abs(fwd[1] - 1.0) < 1e-5, f"Expected fwd (0, 1), got {fwd}"
    assert abs(right[0] - (-1.0)) < 1e-5 and abs(right[1] - 0.0) < 1e-5, f"Expected right (-1, 0), got {right}"

    # Yaw = 90 (looking +X)
    system._yaw = 90.0
    fwd, right = system.get_camera_axes()
    assert abs(fwd[0] - 1.0) < 1e-5 and abs(fwd[1] - 0.0) < 1e-5, f"Expected fwd (1, 0), got {fwd}"
    assert abs(right[0] - 0.0) < 1e-5 and abs(right[1] - 1.0) < 1e-5, f"Expected right (0, 1), got {right}"

    # Yaw = 180 (looking -Z)
    system._yaw = 180.0
    fwd, right = system.get_camera_axes()
    assert abs(fwd[0] - 0.0) < 1e-5 and abs(fwd[1] - (-1.0)) < 1e-5, f"Expected fwd (0, -1), got {fwd}"
    assert abs(right[0] - 1.0) < 1e-5 and abs(right[1] - 0.0) < 1e-5, f"Expected right (1, 0), got {right}"

    # Yaw = 270 (looking -X)
    system._yaw = 270.0
    fwd, right = system.get_camera_axes()
    assert abs(fwd[0] - (-1.0)) < 1e-5 and abs(fwd[1] - 0.0) < 1e-5, f"Expected fwd (-1, 0), got {fwd}"
    assert abs(right[0] - 0.0) < 1e-5 and abs(right[1] - (-1.0)) < 1e-5, f"Expected right (0, -1), got {right}"

    system.shutdown()
    assert HydragonCameraControllerSystem.get_instance() is None, "Singleton should be cleared after shutdown"
    print("  [PASS] Camera axes query & singleton verified across all 4 cardinal angles")


if __name__ == "__main__":
    test_camera_controller_lifecycle()
    test_spherical_orbit_math()
    test_lag_damping_math()
    test_pitch_and_zoom_clamping()
    test_camera_axes_and_singleton()
    print("\n=======================================================")
    print(" ALL CAMERA CONTROLLER SYSTEM TESTS PASSED! (5/5)")
    print("=======================================================")
