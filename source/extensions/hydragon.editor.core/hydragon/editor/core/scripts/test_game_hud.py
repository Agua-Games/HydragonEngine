"""
Unit test for HydragonGameHUD
Validates lifecycle, fail-silent execution outside Omniverse Kit,
countdown state machine timing, score popup handling, and timeline event responses.
"""

import sys
import os

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.game_hud import HydragonGameHUD


def test_game_hud_lifecycle():
    print("--- 1. Testing Game HUD Lifecycle ---")
    hud = HydragonGameHUD()
    assert hud is not None, "Failed to instantiate HydragonGameHUD"
    assert not hud._is_active, "HUD should start inactive"
    assert not hud._is_simulating, "HUD should start non-simulating"
    assert HydragonGameHUD.get_instance() is hud, "Singleton accessor must match instance"

    # Startup & shutdown outside Kit (fail-silent)
    hud.startup()
    assert not hud._is_active, "Should remain inactive when HAS_KIT is False"

    hud.shutdown()
    assert not hud._is_active, "HUD must be inactive after shutdown"
    assert HydragonGameHUD.get_instance() is None, "Singleton accessor must clear after shutdown"
    print("  [PASS] Game HUD lifecycle verified")


def test_countdown_state_machine():
    print("--- 2. Testing Countdown State Machine Timing ---")
    hud = HydragonGameHUD()

    # Mock SimpleStringModel for headless testing
    class MockStringModel:
        def __init__(self, s=""):
            self.as_string = s

    hud._countdown_model = MockStringModel("3...")
    hud._countdown_active = True
    hud._countdown_elapsed = 0.0

    # 0.5s -> 3...
    hud._update_countdown(0.5)
    assert hud._countdown_model.as_string == "3...", f"Expected '3...', got {hud._countdown_model.as_string}"
    assert hud._countdown_active is True

    # 1.5s -> 2...
    hud._update_countdown(1.0)
    assert hud._countdown_model.as_string == "2...", f"Expected '2...', got {hud._countdown_model.as_string}"
    assert hud._countdown_active is True

    # 2.5s -> 1...
    hud._update_countdown(1.0)
    assert hud._countdown_model.as_string == "1...", f"Expected '1...', got {hud._countdown_model.as_string}"
    assert hud._countdown_active is True

    # 3.5s -> MARBLE GAME START!
    hud._update_countdown(1.0)
    assert hud._countdown_model.as_string == "MARBLE GAME START!", f"Expected 'MARBLE GAME START!', got {hud._countdown_model.as_string}"
    assert hud._countdown_active is True

    # 4.5s -> Countdown complete and deactivated
    hud._update_countdown(1.0)
    assert hud._countdown_active is False, "Countdown should auto-deactivate after 4.2s"

    hud.shutdown()
    print("  [PASS] Countdown state machine timing verified")


def test_score_popup_and_victory_modal_fail_silent():
    print("--- 3. Testing Score Popup & Victory Modal Fail-Silent ---")
    hud = HydragonGameHUD()

    # Outside Kit, show_score_popup and show_victory must execute without throwing
    try:
        hud.show_score_popup(world_pos=(100.0, 50.0, 200.0), points=100)
        hud.show_score_popup(world_pos=(-50.0, 50.0, 10.0), points=150)
    except Exception as e:
        assert False, f"show_score_popup raised unexpected exception: {e}"

    try:
        hud.show_victory(score=1250, elapsed=38.4, foes=3)
    except Exception as e:
        assert False, f"show_victory raised unexpected exception: {e}"

    hud.shutdown()
    print("  [PASS] Score popup and victory modal fail-silent verified")


def test_timeline_event_handling():
    print("--- 4. Testing HUD Timeline Event Transitions ---")
    hud = HydragonGameHUD()

    class MockTimelineEvent:
        def __init__(self, event_type: int):
            self.type = event_type

    # 1 = TimelineEventType.PLAY, 0 = STOP
    hud._on_timeline_event(MockTimelineEvent(1))
    assert hud._is_simulating is False, "Outside Kit, _on_timeline_event exits early fail-silently"

    hud.shutdown()
    print("  [PASS] HUD timeline event transitions verified")


if __name__ == "__main__":
    test_game_hud_lifecycle()
    test_countdown_state_machine()
    test_score_popup_and_victory_modal_fail_silent()
    test_timeline_event_handling()
    print("\n=======================================================")
    print(" ALL GAME HUD TESTS PASSED! (4/4)")
    print("=======================================================")
