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
    print("--- 3. Testing Position and Rotation Lag Damping (Ease-in / Ease-out) ---")
    pos_a = (0.0, 50.0, -1000.0)
    pos_b = (100.0, 50.0, -900.0)
    dt = 1.0 / 60.0
    pos_damping = 8.0
    pos_factor = 1.0 - math.exp(-pos_damping * dt)

    lerp_x = pos_a[0] + (pos_b[0] - pos_a[0]) * pos_factor
    lerp_z = pos_a[2] + (pos_b[2] - pos_a[2]) * pos_factor

    assert 0.0 < lerp_x < 100.0, f"Interpolated X should be between 0 and 100, got {lerp_x}"
    assert -1000.0 < lerp_z < -900.0, f"Interpolated Z should be between -1000 and -900, got {lerp_z}"

    # Test rotational inertia & shortest angle interpolation
    cur_yaw = 350.0
    target_yaw = 10.0
    rot_damping = 12.0
    rot_factor = 1.0 - math.exp(-rot_damping * dt)

    yaw_diff = (target_yaw - cur_yaw + 180.0) % 360.0 - 180.0
    assert abs(yaw_diff - 20.0) < 1e-4, f"Yaw diff across 0/360 boundary should be +20 deg, got {yaw_diff}"
    new_yaw = (cur_yaw + yaw_diff * rot_factor) % 360.0
    assert new_yaw > 350.0 or new_yaw < 10.0, f"New yaw should rotate forward through 0 deg boundary, got {new_yaw}"

    print(f"  [PASS] Position & rotation damping verified (pos_factor={pos_factor:.3f}, rot_factor={rot_factor:.3f})")


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


def test_mouse_yaw_direction():
    print("--- 6. Testing Mouse Yaw Direction (Clockwise on Drag Left) ---")
    system = HydragonCameraControllerSystem()
    system._yaw = 0.0
    system._mouse_sensitivity = 0.2

    # User drags mouse to the left: dx < 0
    dx = -10.0
    system._yaw = (system._yaw - dx * system._mouse_sensitivity) % 360.0

    # Moving mouse left must increase yaw angle (clockwise rotation around player)
    assert system._yaw == 2.0, f"Expected yaw 2.0, got {system._yaw}"
    system.shutdown()
    print("  [PASS] Mouse dragging left rotates camera clockwise as requested")


def test_target_resolution_methods():
    print("--- 7. Testing Target Resolution Methods ---")
    system = HydragonCameraControllerSystem()
    assert hasattr(system, "_resolve_target_world_position"), "Must expose _resolve_target_world_position"
    assert hasattr(system, "get_target_world_pos"), "Must expose get_target_world_pos"

    res1 = system._resolve_target_world_position(None)
    assert res1 is None, f"Expected None outside Kit, got {res1}"

    res2 = system.get_target_world_pos(None)
    assert res2 is None, f"Expected None outside Kit, got {res2}"

    system.shutdown()
    print("  [PASS] Target resolution methods verified")


def test_camera_dirty_checking_and_probe_caching():
    print("--- 8. Testing Camera Dirty Checking & Probe Caching ---")
    system = HydragonCameraControllerSystem()

    # 1. Test probe cache hit
    focus = (0.0, 50.0, 0.0)
    desired = (0.0, 100.0, -800.0)
    res1 = system._probe_camera_collision(focus, desired)
    assert res1 == desired
    assert system._last_probe_focus == focus
    assert system._last_probe_desired == desired
    assert system._last_probe_result == desired

    # Call again with identical coordinates: should hit cache immediately
    res2 = system._probe_camera_collision(focus, desired)
    assert res2 == desired

    # Call with slightly jittered coordinates (< 1e-4): should also hit cache
    jitter_focus = (0.000001, 50.0, 0.0)
    jitter_desired = (0.0, 100.000001, -800.0)
    res3 = system._probe_camera_collision(jitter_focus, jitter_desired)
    assert res3 == desired

    # Call with moving coordinates: should re-evaluate and update cache
    new_desired = (10.0, 100.0, -800.0)
    res4 = system._probe_camera_collision(focus, new_desired)
    assert res4 == new_desired
    assert system._last_probe_desired == new_desired

    # 2. Test transform dirty-checking
    class MockAttr:
        def __init__(self):
            self.set_calls = 0
        def IsValid(self):
            return True
        def Set(self, val):
            self.set_calls += 1

    class MockCamPrim:
        def __init__(self):
            self.trans_attr = MockAttr()
            self.rot_attr = MockAttr()
        def GetAttribute(self, name):
            if name == "xformOp:translate":
                return self.trans_attr
            elif name == "xformOp:rotateXYZ":
                return self.rot_attr
            return None

    mock_prim = MockCamPrim()
    pos1 = (100.0, 200.0, 300.0)
    rot1 = (155.0, 0.0, -180.0)

    # First apply sets attributes
    system.apply_camera_transform(mock_prim, pos1, rot1)
    assert mock_prim.trans_attr.set_calls == 1
    assert mock_prim.rot_attr.set_calls == 1
    assert system._last_applied_pos == pos1
    assert system._last_applied_rot == rot1

    # Second apply with identical pos/rot must be skipped by dirty-checking!
    system.apply_camera_transform(mock_prim, pos1, rot1)
    assert mock_prim.trans_attr.set_calls == 1, "Should not write to USD when transform is unchanged"
    assert mock_prim.rot_attr.set_calls == 1, "Should not write to USD when rotation is unchanged"

    # Apply with changed pos should write
    pos2 = (110.0, 200.0, 300.0)
    system.apply_camera_transform(mock_prim, pos2, rot1)
    assert mock_prim.trans_attr.set_calls == 2
    assert mock_prim.rot_attr.set_calls == 2

    system.shutdown()
    print("  [PASS] Camera dirty checking & probe caching verified")


def test_probe_ignores_volume_trigger_colliders():
    print("--- 9. Testing the Anti-Clipping Probe Ignores Volume Trigger Colliders ---")
    import hydragon.editor.core.camera_controller as cam
    import hydragon.editor.core.volume_triggers as vt

    # The probe is only reachable with `carb` and the PhysX scene query interface present, so both
    # are stubbed. `raycast_all` replays a fixed list of hits through the REAL reporting callback,
    # which is the code under test.
    class _Vec:
        def __init__(self, x, y, z):
            self._v = (x, y, z)

        def __getitem__(self, index):
            return self._v[index]

    class _StubCarb:
        """Only what the probe and the lifecycle need: `Float3`, plus the logging the rest of the
        controller calls unconditionally."""

        @staticmethod
        def Float3(x, y, z):
            return _Vec(x, y, z)

        @staticmethod
        def log_info(message):
            pass

        @staticmethod
        def log_warn(message):
            pass

        @staticmethod
        def log_error(message):
            pass

    class _Hit:
        def __init__(self, collision, rigid_body, position):
            self.collision = collision
            self.rigid_body = rigid_body
            self.position = position

    class _Query:
        def __init__(self, hits):
            self._hits = hits

        def raycast_all(self, origin, direction, distance, report_fn, both_sides=False):
            for hit in self._hits:
                if not report_fn(hit):
                    break
            return True

    TRIGGER = "/World/ForceVolume_vortex/volumes/force_trigger"
    WALL = "/World/Environment/Wall/collider"

    original_carb = cam.carb
    # Outside Kit the guarded import never binds these names at all, so `getattr` plus an explicit
    # delete on the way out is required - assigning None would leave a name the module did not have.
    had_query = "get_physx_scene_query_interface" in cam.__dict__
    original_query = cam.__dict__.get("get_physx_scene_query_interface")
    cam.carb = _StubCarb()
    try:
        # Register the collider exactly as `ensure_trigger` does once it has authored one.
        vt._trigger_colliders.add(TRIGGER)
        assert vt.is_trigger_collider(TRIGGER)
        assert vt.is_trigger_collider(TRIGGER + "/child"), "a path under the collider must resolve too"
        assert not vt.is_trigger_collider(WALL)
        assert not vt.is_trigger_collider("")

        system = HydragonCameraControllerSystem()
        system._cached_target_rb_path = "/World/Player"
        focus = (0.0, 0.0, 0.0)
        desired = (0.0, 0.0, -800.0)

        def probe(hits):
            # The cache keys on (focus, desired), and every case below reuses the same pair, so it
            # has to be invalidated or case 2 would silently return case 1's answer.
            cam.get_physx_scene_query_interface = lambda: _Query(hits)
            system._last_probe_focus = None
            system._last_probe_desired = None
            system._last_probe_result = None
            return system._probe_camera_collision(focus, desired)

        # 1. A trigger collider ALONE must not shorten the arm. A hit at 50 is nearer than the arm,
        #    so without the filter the camera would be clamped to the 100-unit floor - which is
        #    exactly the "camera rushes into the player" report this test exists for.
        got = probe([_Hit(TRIGGER, "", (0.0, 0.0, -50.0))])
        assert got == desired, f"a volume trigger must be ignored, got {got}"

        # 2. Real geometry still shortens the arm: hit at 400, minus the 30 offset -> 370.
        got = probe([_Hit(WALL, "", (0.0, 0.0, -400.0))])
        assert abs(got[2] - (-370.0)) < 1e-4, f"a wall must clamp the arm to 370, got {got}"

        # 3. Both at once. This is the case that matters in practice: the trigger is centred exactly
        #    where the player is, so it is ALWAYS the nearest hit and would always win.
        got = probe([_Hit(TRIGGER, "", (0.0, 0.0, -50.0)), _Hit(WALL, "", (0.0, 0.0, -400.0))])
        assert abs(got[2] - (-370.0)) < 1e-4, f"the trigger must not outrank the wall, got {got}"

        # 4. The pre-existing self/target exclusion is unaffected.
        got = probe([_Hit("/World/Player/geometry/ball_mesh", "/World/Player", (0.0, 0.0, -30.0))])
        assert got == desired, f"the followed body must be ignored, got {got}"

        system.shutdown()
        print("  [PASS] Probe ignores volume trigger colliders but still respects geometry")
    finally:
        vt._trigger_colliders.discard(TRIGGER)
        cam.carb = original_carb
        if had_query:
            cam.get_physx_scene_query_interface = original_query
        else:
            del cam.get_physx_scene_query_interface


if __name__ == "__main__":
    test_camera_controller_lifecycle()
    test_spherical_orbit_math()
    test_lag_damping_math()
    test_pitch_and_zoom_clamping()
    test_camera_axes_and_singleton()
    test_mouse_yaw_direction()
    test_target_resolution_methods()
    test_camera_dirty_checking_and_probe_caching()
    test_probe_ignores_volume_trigger_colliders()
    print("\n=======================================================")
    print(" ALL CAMERA CONTROLLER SYSTEM TESTS PASSED! (9/9)")
    print("=======================================================")
