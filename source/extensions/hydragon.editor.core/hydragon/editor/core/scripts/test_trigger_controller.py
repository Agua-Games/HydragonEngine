"""
Unit test for HydragonTriggerSystem and HydragonTriggerZone
Validates lifecycle, fail-silent behavior outside Kit, 3D cylindrical trigger zone overlap math,
one-shot deactivation, and level complete victory handling.
"""

import math
import sys
import os

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.trigger_controller import HydragonTriggerSystem, HydragonTriggerZone


def test_trigger_system_lifecycle():
    print("--- 1. Testing Trigger System Lifecycle ---")
    system = HydragonTriggerSystem()
    assert system is not None, "Failed to instantiate HydragonTriggerSystem"
    assert not system._is_active, "System should start inactive"
    assert not system.is_level_completed, "Level completed should start False"
    assert HydragonTriggerSystem.get_instance() is system, "Singleton should match instantiated system"

    # Startup & shutdown outside Kit (fail-silent)
    system.startup()
    assert not system._is_active, "Should remain inactive when HAS_KIT is False"

    system.shutdown()
    assert not system._is_active, "System should be inactive after shutdown"
    assert len(system._active_triggers) == 0, "Active triggers should be empty after shutdown"
    assert HydragonTriggerSystem.get_instance() is None, "Singleton should be cleared after shutdown"
    print("  [PASS] HydragonTriggerSystem lifecycle verified")


def test_trigger_zone_overlap_math():
    print("--- 2. Testing Trigger Zone Spatial Overlap Math ---")
    # Cylinder volume centered at (100, 50, 200) with radius=75.0, half_height=40.0
    zone = HydragonTriggerZone(
        prim=None,
        world_pos=(100.0, 50.0, 200.0),
        radius=75.0,
        half_height=40.0,
    )

    # 1. Point dead-center in trigger zone
    assert zone.check_overlap((100.0, 50.0, 200.0)), "Center point must overlap"

    # 2. Point inside cylinder horizontally and vertically
    # dx=30, dz=40 -> horiz_dist = 50 <= 75, dy = 20 <= 40
    assert zone.check_overlap((130.0, 70.0, 240.0)), "Interior point must overlap"

    # 3. Point on edge of horizontal radius: dx=75, dz=0 -> horiz_dist=75 <= 75
    assert zone.check_overlap((175.0, 50.0, 200.0)), "Point on horizontal boundary must overlap"

    # 4. Point outside horizontal radius: dx=80, dz=0 -> horiz_dist=80 > 75
    assert not zone.check_overlap((180.0, 50.0, 200.0)), "Point outside horizontal radius must NOT overlap"

    # 5. Point outside vertical envelope: dy=50 > half_height 40
    assert not zone.check_overlap((100.0, 100.0, 200.0)), "Point above trigger height must NOT overlap"

    # 6. Point below trigger floor: dy=-45 > half_height 40
    assert not zone.check_overlap((100.0, 5.0, 200.0)), "Point below trigger floor must NOT overlap"

    print("  [PASS] Trigger zone spatial overlap math verified")


def test_trigger_zone_one_shot_behavior():
    print("--- 3. Testing One-Shot Trigger Activation & Deactivation ---")
    zone = HydragonTriggerZone(
        prim=None,
        world_pos=(0.0, 0.0, 0.0),
        radius=100.0,
        half_height=50.0,
    )
    assert not zone.is_triggered
    assert zone.is_enabled

    # Overlap passes initially
    assert zone.check_overlap((10.0, 0.0, 10.0))

    # Trigger is entered
    zone.on_trigger_entered()
    assert zone.is_triggered

    # If schema is mock/None, default is_one_shot is True
    # Now subsequent overlap checks should fail because trigger was consumed
    # (zone.is_enabled becomes False)
    # Wait, when prim is None, is_enabled is `not self._triggered`
    assert not zone.is_enabled, "Trigger should be disabled after one-shot trigger"
    assert not zone.check_overlap((10.0, 0.0, 10.0)), "Disabled trigger must reject overlap"

    print("  [PASS] Trigger zone one-shot behavior verified")


def test_level_complete_handling():
    print("--- 4. Testing Level Complete Victory Transition ---")
    system = HydragonTriggerSystem()
    assert not system.is_level_completed

    # Trigger victory
    system._handle_level_complete(stage=None)
    assert system.is_level_completed, "System must transition to level_completed = True"

    # Duplicate call should be idempotent
    system._handle_level_complete(stage=None)
    assert system.is_level_completed

    system.shutdown()
    print("  [PASS] Level complete victory transition verified")


if __name__ == "__main__":
    test_trigger_system_lifecycle()
    test_trigger_zone_overlap_math()
    test_trigger_zone_one_shot_behavior()
    test_level_complete_handling()
    print("\n=======================================================")
    print(" ALL TRIGGER CONTROLLER SYSTEM TESTS PASSED! (4/4)")
    print("=======================================================")
