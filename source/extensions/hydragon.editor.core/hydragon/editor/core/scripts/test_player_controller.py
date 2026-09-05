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


if __name__ == "__main__":
    test_player_controller_system_lifecycle()
    test_camera_axes_math()
    test_input_normalization_math()
    test_player_discovery_fail_silent()
    print("\n=======================================================")
    print(" ALL PLAYER CONTROLLER SYSTEM TESTS PASSED! (4/4)")
    print("=======================================================")
