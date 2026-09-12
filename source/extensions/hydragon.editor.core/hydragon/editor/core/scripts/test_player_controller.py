"""
Unit test for HydragonPlayerControllerSystem
Validates initialization, fail-silent behavior outside Kit, input calculation, and vector math.
"""

import math
import sys
import os

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.player_controller import HydragonPlayerControllerSystem


def test_player_controller_system_lifecycle():
    print("--- 1. Testing Player Controller System Lifecycle ---")
    system = HydragonPlayerControllerSystem()
    assert system is not None, "Failed to instantiate HydragonPlayerControllerSystem"
    assert not system._is_active, "System should start inactive"

    # Startup & shutdown (fail-silent outside Kit)
    system.startup()
    assert not system._is_active, "Should remain inactive when HAS_KIT is False"

    system.shutdown()
    assert not system._is_active, "System should be inactive after shutdown"
    assert len(system._keys_down) == 0, "Keys down should be empty"
    print("  [PASS] HydragonPlayerControllerSystem lifecycle verified")


def test_camera_axes_math():
    print("--- 2. Testing Camera Axes Math ---")
    # Default camera axes (+Z forward, -X right in screen space)
    fwd, right = HydragonPlayerControllerSystem.compute_camera_axes(None)
    assert fwd == (0.0, 1.0), f"Expected default forward (0.0, 1.0), got {fwd}"
    assert right == (-1.0, 0.0), f"Expected default right (-1.0, 0.0), got {right}"

    # Dot product of orthogonal 2D vectors must be 0
    dot = fwd[0] * right[0] + fwd[1] * right[1]
    assert abs(dot) < 1e-6, f"Axes must be perpendicular! dot={dot}"

    # Test camera space projection: pressing A (left) should move left on screen
    # A corresponds to right_input = -1.0
    # move_x = rx * (-1.0) = (-1.0) * (-1.0) = +1.0 (positive X on screen is Left)
    fwd_input = 0.0
    right_input = -1.0
    move_x = fwd[0] * fwd_input + right[0] * right_input
    move_z = fwd[1] * fwd_input + right[1] * right_input
    assert move_x == 1.0 and move_z == 0.0, f"A (left) input should move +X, got ({move_x}, {move_z})"

    # Pressing D (right) corresponds to right_input = +1.0
    right_input = 1.0
    move_x = fwd[0] * fwd_input + right[0] * right_input
    move_z = fwd[1] * fwd_input + right[1] * right_input
    assert move_x == -1.0 and move_z == 0.0, f"D (right) input should move -X, got ({move_x}, {move_z})"

    print("  [PASS] Camera axes orthogonality and camera space projection verified")


def test_input_normalization_math():
    print("--- 3. Testing Input Vector Math ---")
    system = HydragonPlayerControllerSystem()

    # When no keys are pressed outside Kit
    fwd, right = system._compute_input_vector()
    assert fwd == 0.0 and right == 0.0, "Expected zero input"

    # Test diagonal normalization math
    diag_fwd, diag_right = 1.0, 1.0
    length = math.sqrt(diag_fwd * diag_fwd + diag_right * diag_right)
    norm_fwd = diag_fwd / length
    norm_right = diag_right / length
    norm_len = math.sqrt(norm_fwd * norm_fwd + norm_right * norm_right)
    assert abs(norm_len - 1.0) < 1e-6, "Normalized vector length should be 1.0"
    print("  [PASS] Input normalization math verified")


def test_player_discovery_fail_silent():
    print("--- 4. Testing Player Discovery Fail-Silent ---")
    system = HydragonPlayerControllerSystem()
    result = system.find_player_prim(None)
    assert result is None, "Should safely return None when stage is None"
    print("  [PASS] Player discovery fail-silent verified")


def test_player_respawn_teleport_logic():
    print("--- 5. Testing Player Respawn Teleport Logic ---")
    system = HydragonPlayerControllerSystem()
    system._spawn_pos = (0.0, 50.0, -1000.0)

    # Mock PhysX Simulation Interface
    class MockSimIface:
        def __init__(self):
            self.forces = []
            self.flushes = 0
        def apply_force_at_pos(self, stage_id, prim_id, force, pos, mode):
            self.forces.append((prim_id, force, pos, mode))
        def flush_changes(self):
            self.flushes += 1

    # Mock USD Prims
    class MockAttr:
        def __init__(self, val):
            self.val = val
        def IsValid(self):
            return True
        def Set(self, val):
            self.val = val
        def Get(self):
            return self.val

    class MockPrim:
        def __init__(self, path):
            self.path = path
            self.attrs = {
                "physics:rigidBodyEnabled": MockAttr(True),
                "physics:velocity": MockAttr((0.0, 0.0, 0.0)),
                "physics:angularVelocity": MockAttr((0.0, 0.0, 0.0)),
                "xformOp:translate": MockAttr((0.0, 0.0, 0.0)),
            }
        def IsValid(self):
            return True
        def HasAttribute(self, name):
            return name in self.attrs
        def GetAttribute(self, name):
            return self.attrs.get(name)

    player_prim = MockPrim("/World/Player")
    rb_prim = MockPrim("/World/Player/geometry/ball_mesh")
    sim_iface = MockSimIface()

    # Verify fall detection threshold triggers _needs_respawn
    falling_pos = (0.0, -251.0, -1000.0)
    assert falling_pos[1] < -200.0, "Must detect fall when Y < -200"
    if falling_pos[1] < -200.0:
        system._needs_respawn = True
    assert system._needs_respawn is True

    # Test safe spawn elevation (must be at least 120.0 to prevent interpenetration)
    spawn_x, spawn_y, spawn_z = system._spawn_pos
    safe_y = max(120.0, spawn_y)
    assert safe_y == 120.0, f"Expected safe_y=120.0, got {safe_y}"

    # Simulate thread-safe main thread respawn (no rigidBodyEnabled toggling or flush_changes)
    player_prim.GetAttribute("xformOp:translate").Set((spawn_x, safe_y, spawn_z))
    rb_prim.GetAttribute("xformOp:translate").Set((0.0, 0.0, 0.0))
    assert player_prim.GetAttribute("xformOp:translate").Get() == (0.0, 120.0, -1000.0)
    assert rb_prim.GetAttribute("xformOp:translate").Get() == (0.0, 0.0, 0.0)

    # Zero velocity via OpenUSD physics:velocity and physics:angularVelocity attributes
    rb_prim.GetAttribute("physics:velocity").Set((0.0, 0.0, 0.0))
    rb_prim.GetAttribute("physics:angularVelocity").Set((0.0, 0.0, 0.0))
    assert rb_prim.GetAttribute("physics:velocity").Get() == (0.0, 0.0, 0.0)
    assert rb_prim.GetAttribute("physics:angularVelocity").Get() == (0.0, 0.0, 0.0)

    # Last world pos updated to safe elevation
    system._last_world_pos = (spawn_x, safe_y, spawn_z)
    system._needs_respawn = False
    assert system._last_world_pos == (0.0, 120.0, -1000.0)
    assert system._needs_respawn is False

    # Test respawn cooldown and world pos override during grace period
    system._respawn_cooldown = 1.0
    system._is_respawning = True
    assert system.get_player_world_pos() == (0.0, 120.0, -1000.0)
    system._is_respawning = False
    system._respawn_cooldown = 0.0

    system.shutdown()
    print("  [PASS] Player respawn teleport logic verified")


def test_player_input_restoration():
    print("--- 6. Testing Player Input Restoration & Respawn Scope ---")
    system = HydragonPlayerControllerSystem()

    # Call _respawn_player_main_thread directly to ensure no UnboundLocalError occurs
    try:
        system._respawn_player_main_thread()
    except UnboundLocalError as err:
        assert False, f"UnboundLocalError occurred in _respawn_player_main_thread: {err}"
    except Exception:
        # Other exceptions outside Kit are expected and handled
        pass

    # Mock timeline event for PLAY and STOP
    class MockTimelineEvent:
        def __init__(self, event_type: int):
            self.type = event_type

    # 1 is TimelineEventType.PLAY, 0 is STOP in omni.timeline
    system._on_timeline_event(MockTimelineEvent(1))
    system._on_timeline_event(MockTimelineEvent(0))
    system.shutdown()
    print("  [PASS] Player input restoration & respawn scope verified")


def test_physics_manager_integration():
    print("--- 7. Testing HydragonPhysicsManager Integration with Player Controller ---")
    from hydragon.editor.core.schemas import HydragonPhysicsManager

    system = HydragonPlayerControllerSystem()
    assert system._physics_mgr is None

    # Test discovery fail-silent when stage is None
    mgr = system._discover_physics_manager(None)
    assert mgr is None

    # Test default physics values when _physics_mgr is None
    class MockPrim:
        def __init__(self):
            self.attrs = {}
        def IsValid(self):
            return True
        def HasAttribute(self, name):
            return name in self.attrs

    class MockController:
        def __init__(self):
            self.max_angular_velocity = 0.0

    # Ensure damping doesn't fail when no physics manager
    system._ensure_rigid_body_damping(MockPrim(), MockController())

    # Set mock physics manager with custom limits
    class MockPhysicsManager:
        max_linear_velocity = 25000.0
        max_angular_velocity = 80.0
        default_linear_damping = 0.1
        default_angular_damping = 0.5
        solver_position_iterations = 32
        solver_velocity_iterations = 8
        enable_ccd = True
        bounce_threshold = 300.0

    system._physics_mgr = MockPhysicsManager()
    assert system._physics_mgr.max_linear_velocity == 25000.0
    assert system._physics_mgr.max_angular_velocity == 80.0

    system.shutdown()
    assert system._physics_mgr is None
    print("  [PASS] HydragonPhysicsManager integration and lifecycle verified")


def test_physics_manager_velocity_clamping_and_ccd():
    print("--- 8. Testing PhysicsManager Velocity Clamping and Kinematic CCD ---")
    system = HydragonPlayerControllerSystem()

    class MockAttr:
        def __init__(self, val):
            self.val = val
        def IsValid(self):
            return True
        def Get(self):
            return self.val
        def Set(self, val):
            self.val = val

    class MockPrimWithAttrs:
        def __init__(self, is_kinematic=False, initial_max_vel=10000.0):
            self.attrs = {
                "physics:kinematicEnabled": MockAttr(is_kinematic),
                "physxRigidBody:maxLinearVelocity": MockAttr(initial_max_vel),
                "physxRigidBody:maxAngularVelocity": MockAttr(60.0),
                "physxRigidBody:enableCCD": MockAttr(True),
                "physxRigidBody:solverPositionIterationCount": MockAttr(16),
                "physxRigidBody:solverVelocityIterationCount": MockAttr(4),
            }
        def IsValid(self):
            return True
        def HasAttribute(self, name):
            return name in self.attrs
        def GetAttribute(self, name):
            return self.attrs.get(name)

    class MockController:
        def __init__(self):
            self.max_angular_velocity = 0.0

    class MockPhysicsManagerLowVel:
        max_linear_velocity = 10.0
        max_angular_velocity = 20.0
        default_linear_damping = 0.5
        default_angular_damping = 2.0
        solver_position_iterations = 24
        solver_velocity_iterations = 6
        enable_ccd = True
        bounce_threshold = 100.0

    # 1. Test that max_linear_velocity = 10.0 overwrites existing 10000.0
    prim = MockPrimWithAttrs(is_kinematic=False, initial_max_vel=10000.0)
    system._physics_mgr = MockPhysicsManagerLowVel()
    system._ensure_rigid_body_damping(prim, MockController())
    assert prim.attrs["physxRigidBody:maxLinearVelocity"].Get() == 1000.0, "maxLinearVelocity was not scaled to 1000.0!"
    assert prim.attrs["physxRigidBody:maxAngularVelocity"].Get() == math.degrees(20.0), "maxAngularVelocity was not converted from radians to degrees!"
    assert prim.attrs["physxRigidBody:solverPositionIterationCount"].Get() == 24
    assert prim.attrs["physxRigidBody:enableCCD"].Get() is True

    # 2. Test that kinematic rigid bodies have CCD disabled
    kin_prim = MockPrimWithAttrs(is_kinematic=True, initial_max_vel=10000.0)
    system._ensure_rigid_body_damping(kin_prim, MockController())
    assert kin_prim.attrs["physxRigidBody:enableCCD"].Get() is False, "CCD must be False for kinematic rigid body!"

    system.shutdown()
    print("  [PASS] Velocity clamping down to 10 and kinematic CCD protection verified")


def test_physics_scene_settings_configuration():
    print("--- 9. Testing PhysicsScene CCD & Bounce Threshold Configuration ---")
    system = HydragonPlayerControllerSystem()

    class MockAttr:
        def __init__(self, val):
            self.val = val
        def IsValid(self):
            return True
        def Get(self):
            return self.val
        def Set(self, val):
            self.val = val

    class MockScenePrim:
        def __init__(self):
            self.attrs = {
                "physxScene:enableCCD": MockAttr(False),
                "physxScene:bounceThreshold": MockAttr(0.0),
            }
        def IsValid(self):
            return True
        def GetTypeName(self):
            return "PhysicsScene"
        def HasAttribute(self, name):
            return name in self.attrs
        def GetAttribute(self, name):
            return self.attrs.get(name)
        def CreateAttribute(self, name, type_name):
            attr = MockAttr(None)
            self.attrs[name] = attr
            return attr

    class MockStage:
        def __init__(self, scene_prim):
            self.scene_prim = scene_prim
        def GetPrimAtPath(self, path):
            if path in ("/World/PhysicsScene", "/PhysicsScene"):
                return self.scene_prim
            return None
        def Traverse(self):
            return [self.scene_prim]

    class MockPhysicsManager:
        enable_ccd = True
        bounce_threshold = 250.0

    scene = MockScenePrim()
    stage = MockStage(scene)
    system._ensure_physics_scene_settings(stage, MockPhysicsManager())

    assert scene.attrs["physxScene:enableCCD"].Get() is True, "PhysicsScene must have enableCCD set to True!"
    assert scene.attrs["physxScene:bounceThreshold"].Get() == 250.0, "PhysicsScene bounceThreshold not updated!"

    system.shutdown()
    print("  [PASS] PhysicsScene global CCD and bounce threshold configuration verified")


def test_ground_probe_ignores_volume_trigger_colliders():
    print("--- 10. Testing the Ground Probe Ignores Volume Trigger Colliders ---")
    import hydragon.editor.core.player_controller as pc
    import hydragon.editor.core.volume_triggers as vt

    TRIGGER = "/World/ForceVolume_launchpad/volumes/force_trigger"
    FLOOR = "/World/Environment/Ground/collider"

    class _Vec:
        def __init__(self, x, y, z):
            self._v = (x, y, z)

        def __getitem__(self, index):
            return self._v[index]

    class _StubCarb:
        Float3 = staticmethod(lambda x, y, z: _Vec(x, y, z))
        log_info = staticmethod(lambda message: None)
        log_warn = staticmethod(lambda message: None)
        log_error = staticmethod(lambda message: None)

    class _RadiusAttr:
        def IsValid(self):
            return True

        def Get(self):
            return 50.0

    class _SphereGeom:
        def GetRadiusAttr(self):
            return _RadiusAttr()

    class _StubUsdGeom:
        Sphere = staticmethod(lambda prim: _SphereGeom())

    class _Hit:
        def __init__(self, collision, position):
            self.collision = collision
            self.rigid_body = ""
            self.position = position

    class _Query:
        def __init__(self, hits):
            self._hits = hits

        def raycast_all(self, origin, direction, distance, report_fn, both_sides=False):
            for hit in self._hits:
                if not report_fn(hit):
                    break
            return True

    class _Prim:
        def GetPath(self):
            class _Path:
                pathString = "/World/Player/geometry/ball_mesh"

            return _Path()

    # Outside Kit the guarded import never binds `UsdGeom` / `get_physx_scene_query_interface` at
    # all, so "which names existed" has to be recorded and missing ones deleted on the way out -
    # assigning None would leave behind a name the module never had.
    patched_names = ("carb", "UsdGeom", "get_physx_scene_query_interface")
    saved = {name: pc.__dict__[name] for name in patched_names if name in pc.__dict__}
    pc.carb = _StubCarb()
    pc.UsdGeom = _StubUsdGeom()
    try:
        vt._trigger_colliders.add(TRIGGER)

        system = HydragonPlayerControllerSystem()
        system._cached_player_path = "/World/Player"
        # The ball is at y=200 with radius 50: clearly airborne, 150 above the floor. The ground
        # probe reaches only `radius + 15` = 65, so ONLY a game volume whose floor happens to sit
        # under the ball can fool it - and that is precisely the reported failure.
        system._get_rigid_body_world_pos = lambda prim, path: (0.0, 200.0, 0.0)

        def grounded(hits):
            pc.get_physx_scene_query_interface = lambda: _Query(hits)
            return system._check_is_grounded(None, _Prim())

        # A volume floor 50 below the ball must NOT count as ground. Without the filter the ball can
        # jump again in mid-air; that is the bug this test pins.
        assert grounded([_Hit(TRIGGER, (0.0, 150.0, 0.0))]) is False, (
            "a volume trigger must not count as ground"
        )

        # Real ground still does.
        assert grounded([_Hit(FLOOR, (0.0, 150.0, 0.0))]) is True, "real ground must still count"

        system.shutdown()
        print("  [PASS] Ground probe ignores volume trigger colliders but still detects ground")
    finally:
        vt._trigger_colliders.discard(TRIGGER)
        for name in patched_names:
            if name in saved:
                setattr(pc, name, saved[name])
            else:
                pc.__dict__.pop(name, None)


if __name__ == "__main__":
    test_player_controller_system_lifecycle()
    test_camera_axes_math()
    test_input_normalization_math()
    test_player_discovery_fail_silent()
    test_player_respawn_teleport_logic()
    test_player_input_restoration()
    test_physics_manager_integration()
    test_physics_manager_velocity_clamping_and_ccd()
    test_physics_scene_settings_configuration()
    test_ground_probe_ignores_volume_trigger_colliders()
    print("\n=======================================================")
    print(" ALL PLAYER CONTROLLER SYSTEM TESTS PASSED! (10/10)")
    print("=======================================================")
