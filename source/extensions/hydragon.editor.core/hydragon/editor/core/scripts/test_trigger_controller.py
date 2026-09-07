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
    # Includes 50.0 margin for player marble radius (effective horiz=125.0, effective vert=90.0)
    zone = HydragonTriggerZone(
        prim=None,
        world_pos=(100.0, 50.0, 200.0),
        radius=75.0,
        half_height=40.0,
    )

    # 1. Point dead-center in trigger zone
    assert zone.check_overlap((100.0, 50.0, 200.0)), "Center point must overlap"

    # 2. Point inside cylinder horizontally and vertically
    # dx=30, dz=40 -> horiz_dist = 50 <= 125, dy = 20 <= 90
    assert zone.check_overlap((130.0, 70.0, 240.0)), "Interior point must overlap"

    # 3. Point touching cylinder boundary: dx=75, dz=0 -> horiz_dist=75 <= 125
    assert zone.check_overlap((175.0, 50.0, 200.0)), "Point on cylinder boundary must overlap"

    # 4. Point within marble radius margin: dx=120, dz=0 -> horiz_dist=120 <= 125
    assert zone.check_overlap((220.0, 50.0, 200.0)), "Point within marble margin must overlap"

    # 5. Point outside horizontal envelope: dx=135, dz=0 -> horiz_dist=135 > 125
    assert not zone.check_overlap((235.0, 50.0, 200.0)), "Point outside horizontal envelope must NOT overlap"

    # 6. Point outside vertical envelope: dy=100 > effective half_height 90
    assert not zone.check_overlap((100.0, 150.0, 200.0)), "Point above trigger height must NOT overlap"

    # 7. Point below trigger floor: dy=-100 > effective half_height 90
    assert not zone.check_overlap((100.0, -50.0, 200.0)), "Point below trigger floor must NOT overlap"

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


def test_trigger_report_event_filtering():
    print("--- 5. Testing Trigger Report Event Filtering ---")
    system = HydragonTriggerSystem()

    # Test _decode_prim_path helper
    assert system._decode_prim_path(None) == ""
    assert system._decode_prim_path("/World/Player") == "/World/Player"

    class MockPathObj:
        def __init__(self, path: str):
            self.pathString = path
    assert system._decode_prim_path(MockPathObj("/World/Goal")) == "/World/Goal"

    # Test event filtering:
    # 1. When not simulating, reports must be ignored
    system._is_simulating = False
    class MockTriggerEvent:
        def __init__(self, event_type, trigger_collider, other_collider):
            self.event_type = event_type
            self.trigger_collider = trigger_collider
            self.other_collider = other_collider

    system._on_physx_trigger_report(MockTriggerEvent("TRIGGER_ON_ENTER", "/World/Goal", "/World/Player"))
    assert not system.is_level_completed, "Should ignore trigger report when not simulating"

    # 2. When simulating, LEAVE events must be ignored
    system._is_simulating = True
    system._on_physx_trigger_report(MockTriggerEvent("TRIGGER_ON_LEAVE", "/World/Goal", "/World/Player"))
    assert not system.is_level_completed, "Should strictly ignore TRIGGER_ON_LEAVE events"

    # 3. Non-player collider (e.g. foe or debris) must be ignored
    system._active_triggers["/World/Goal"] = HydragonTriggerZone(None, world_pos=(0, 0, 0))
    system._on_physx_trigger_report(MockTriggerEvent("TRIGGER_ON_ENTER", "/World/Goal", "/World/Foes/Foe_01"))
    assert not system.is_level_completed, "Should ignore non-player collider triggering"

    system.shutdown()
    print("  [PASS] Trigger report event filtering verified")


def test_repeated_play_session_reset():
    print("--- 6. Testing Repeated Play Session Reset ---")
    system = HydragonTriggerSystem()

    # Create trigger zone
    zone = HydragonTriggerZone(None, world_pos=(100.0, 50.0, 200.0), radius=75.0, half_height=40.0)
    system._active_triggers["/World/Goal"] = zone

    # Play session 1:
    assert not system.is_level_completed
    assert zone.is_enabled
    assert zone.check_overlap((100.0, 50.0, 200.0))

    # Player reaches goal
    zone.on_trigger_entered()
    assert zone.is_triggered
    assert not zone.is_enabled, "Trigger should be one-shot disabled during active session"
    system._handle_level_complete(stage=None)
    assert system.is_level_completed

    # User presses STOP
    system._level_completed = False
    assert not system.is_level_completed

    # User presses PLAY again: trigger zone is re-enabled for new session
    zone._triggered = False
    assert zone.is_enabled, "Trigger must be ready again on new play session"
    assert zone.check_overlap((100.0, 50.0, 200.0)), "Trigger must detect overlap on new play session"

    # Player reaches goal in second playthrough
    system._handle_level_complete(stage=None)
    assert system.is_level_completed, "Victory must cleanly trigger on repeated play sessions"

    system.shutdown()
    print("  [PASS] Repeated play session reset verified")


def test_trigger_audio_playback_and_filtering():
    print("--- 7. Testing Trigger Audio Playback & One-Shot Filtering ---")
    system = HydragonTriggerSystem()
    zone = HydragonTriggerZone(None, world_pos=(0.0, 0.0, 0.0))

    # 1. Verify schema property defaults
    assert "achievement_02.wav" in zone.sound_asset_path
    assert zone.sound_enabled is True
    assert zone.sound_play_once is True
    assert zone.sound_played is False

    # 2. Verify sound asset resolution on disk
    resolved = system._resolve_sound_path(zone.sound_asset_path)
    assert resolved != "", "Failed to resolve achievement_02.wav path"
    assert os.path.exists(resolved), f"Sound file not found: {resolved}"

    # 3. Verify first trigger overlap triggers sound playback
    system._play_trigger_sound(zone)
    assert system._mock_sound_play_count == 1, "Expected mock audio play count to be 1"
    assert zone.sound_played is True, "sound_played should be True after firing"

    # 4. Verify sound_play_once prevents repeated playback
    system._play_trigger_sound(zone)
    assert system._mock_sound_play_count == 1, "sound_play_once must prevent duplicate audio triggers"

    # 5. Verify sound_enabled flag suppresses playback
    zone_disabled = HydragonTriggerZone(None, world_pos=(50.0, 0.0, 0.0))
    # Mock schema disabled
    class MockSchema:
        sound_asset_path = "achievement_02.wav"
        sound_enabled = False
        sound_play_once = True
    zone_disabled._schema = MockSchema()
    assert zone_disabled.sound_enabled is False
    system._play_trigger_sound(zone_disabled)
    assert system._mock_sound_play_count == 1, "Disabled sound must not trigger playback"

    system.shutdown()
    print("  [PASS] Trigger audio playback and one-shot filtering verified")


if __name__ == "__main__":
    test_trigger_system_lifecycle()
    test_trigger_zone_overlap_math()
    test_trigger_zone_one_shot_behavior()
    test_level_complete_handling()
    test_trigger_report_event_filtering()
    test_repeated_play_session_reset()
    test_trigger_audio_playback_and_filtering()
    print("\n=======================================================")
    print(" ALL TRIGGER CONTROLLER SYSTEM TESTS PASSED! (7/7)")
    print("=======================================================")
