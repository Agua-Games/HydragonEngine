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

    # Mock Label for headless testing
    class MockLabel:
        def __init__(self, text=""):
            self.text = text
            self.style = {}

    hud._countdown_label = MockLabel("3...")
    hud._countdown_active = True
    hud._countdown_elapsed = 0.0

    # 0.5s -> 3...
    hud._update_countdown(0.5)
    assert hud._countdown_label.text == "3...", f"Expected '3...', got {hud._countdown_label.text}"
    assert hud._countdown_active is True

    # 1.5s -> 2...
    hud._update_countdown(1.0)
    assert hud._countdown_label.text == "2...", f"Expected '2...', got {hud._countdown_label.text}"
    assert hud._countdown_active is True

    # 2.5s -> 1...
    hud._update_countdown(1.0)
    assert hud._countdown_label.text == "1...", f"Expected '1...', got {hud._countdown_label.text}"
    assert hud._countdown_active is True

    # 3.5s -> MARBLE GAME START!
    hud._update_countdown(1.0)
    assert hud._countdown_label.text == "MARBLE GAME START!", f"Expected 'MARBLE GAME START!', got {hud._countdown_label.text}"
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


def test_canvas_config_binding():
    print("--- 5. Testing Canvas Config Binding & Filtering ---")
    hud = HydragonGameHUD()

    class MockCanvas:
        def __init__(self, controls=True, countdown=True, score_popups=True, path="/World/GameHUD"):
            self.show_controls = controls
            self.show_countdown = countdown
            self.show_score_popups = score_popups
            class MockPrim:
                def GetPath(self):
                    return path
            self.prim = MockPrim()

    # Apply canvas with controls and score popups disabled
    canvas = MockCanvas(controls=False, countdown=True, score_popups=False)
    hud._apply_canvas_schema(canvas)
    assert hud._show_controls is False
    assert hud._show_countdown is True
    assert hud._show_score_popups is False
    assert hud._active_canvas_path == "/World/GameHUD"

    # Score popups should not spawn when disabled
    hud.show_score_popup(world_pos=(0, 0, 0), points=100)
    assert len(hud._active_popups) == 0

    hud.shutdown()
    print("  [PASS] Canvas config binding and popups filtering verified")


def test_controls_frame_and_window_lifecycle():
    print("--- 6. Testing All Overlays (Frame & Window) Cleanup Lifecycle ---")
    hud = HydragonGameHUD()

    class MockFrame:
        def __init__(self):
            self.cleared = False
            self.visible = True
        def clear(self):
            self.cleared = True

    class MockWindow:
        def __init__(self):
            self.visible = True

    hud._controls_frame = MockFrame()
    hud._controls_window = MockWindow()
    hud._countdown_frame = MockFrame()
    hud._countdown_window = MockWindow()
    hud._popups_frame = MockFrame()
    hud._victory_frame = MockFrame()
    hud._victory_window = MockWindow()

    # Destroy calls must clear frames and hide windows
    hud._destroy_controls_ui()
    assert hud._controls_frame is None
    assert hud._controls_window is None

    hud._destroy_countdown_ui()
    assert hud._countdown_frame is None
    assert hud._countdown_window is None

    hud._destroy_all_popups()
    assert hud._popups_frame is None

    hud._destroy_victory_ui()
    assert hud._victory_frame is None
    assert hud._victory_window is None

    # Re-assign and test shutdown cleans them all up
    f_ctrl = MockFrame()
    w_ctrl = MockWindow()
    f_count = MockFrame()
    w_count = MockWindow()
    f_pop = MockFrame()
    f_vic = MockFrame()
    w_vic = MockWindow()

    hud._controls_frame = f_ctrl
    hud._controls_window = w_ctrl
    hud._countdown_frame = f_count
    hud._countdown_window = w_count
    hud._popups_frame = f_pop
    hud._victory_frame = f_vic
    hud._victory_window = w_vic

    hud.shutdown()
    assert f_ctrl.cleared and w_ctrl.visible is False
    assert f_count.cleared and w_count.visible is False
    assert f_pop.cleared
    assert f_vic.cleared and w_vic.visible is False
    assert hud._controls_frame is None
    assert hud._countdown_frame is None
    assert hud._popups_frame is None
    assert hud._victory_frame is None
    print("  [PASS] All overlays frame & window cleanup verified")


if __name__ == "__main__":
    test_game_hud_lifecycle()
    test_countdown_state_machine()
    test_score_popup_and_victory_modal_fail_silent()
    test_timeline_event_handling()
    test_canvas_config_binding()
    test_controls_frame_and_window_lifecycle()
    print("\n=======================================================")
    print(" ALL GAME HUD TESTS PASSED! (6/6)")
    print("=======================================================")
