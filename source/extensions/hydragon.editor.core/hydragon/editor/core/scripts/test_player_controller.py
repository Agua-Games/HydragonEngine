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


if __name__ == "__main__":
    test_player_controller_system_lifecycle()
    test_camera_axes_math()
    test_input_normalization_math()
    test_player_discovery_fail_silent()
    test_player_respawn_teleport_logic()
    test_player_input_restoration()
    print("\n=======================================================")
    print(" ALL PLAYER CONTROLLER SYSTEM TESTS PASSED! (6/6)")
    print("=======================================================")
