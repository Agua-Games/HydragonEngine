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


def test_game_hud_dormant_without_canvas():
    print("--- 7. Testing Game HUD Strict Opt-In Dormancy on Bare Stages ---")
    hud = HydragonGameHUD()
    hud.startup()

    # Defaults before/after discovery on a bare stage must be False
    assert hud._show_controls is False
    assert hud._show_countdown is False
    assert hud._show_score_popups is False

    hud._discover_canvas_config()
    assert hud._show_controls is False
    assert hud._show_countdown is False
    assert hud._show_score_popups is False
    assert hud._active_canvas_path is None
    print("  [PASS] Game HUD features remain completely disabled when no UICanvas exists on stage")

    # When a canvas is explicitly bound with settings enabled:
    class MockCanvasPrim:
        def __init__(self):
            self._path = "/World/GameHUD"
        def IsValid(self):
            return True
        def GetPath(self):
            return self._path
        def HasAttribute(self, name):
            return True
        def GetAttribute(self, name):
            vals = {
                "hud:showControls": True,
                "hud:showCountdown": True,
                "hud:showScorePopups": True,
                "hud:autoActivateOnPlay": True,
                "hud:canvasType": "InGame",
            }
            class MockAttr:
                def __init__(self, val):
                    self._val = val
                def IsValid(self):
                    return True
                def Get(self):
                    return self._val
            return MockAttr(vals.get(name))

    from hydragon.editor.core.schemas import HydragonUICanvas
    canvas = HydragonUICanvas(MockCanvasPrim())
    hud._apply_canvas_schema(canvas)
    assert hud._show_controls is True
    assert hud._show_countdown is True
    assert hud._show_score_popups is True
    assert hud._active_canvas_path == "/World/GameHUD"
    print("  [PASS] Game HUD features properly activate when UICanvas is present")

    hud.shutdown()


def test_main_menu_lifecycle_and_start_game():
    print("--- 8. Testing Main Menu Lifecycle and Start Game ---")
    from hydragon.editor.core.game_hud import HydragonUISystem
    assert HydragonUISystem is HydragonGameHUD, "HydragonUISystem must alias HydragonGameHUD"

    ui = HydragonUISystem()
    ui.startup()

    class MockCanvasPrim:
        def __init__(self, path="/World/UI/Menus/MainMenu", canvas_type="MainMenu"):
            self._path = path
            self._type = canvas_type
        def IsValid(self):
            return True
        def GetPath(self):
            return self._path
        def HasAttribute(self, name):
            return True
        def GetAttribute(self, name):
            vals = {
                "hud:canvasType": self._type,
                "hud:title": "HYDRAGON",
                "hud:subtitle": "A High-Performance OpenUSD Experience",
                "hud:showStartGame": True,
                "hud:showSettings": True,
                "hud:showQuit": True,
                "hud:autoActivateOnPlay": True,
            }
            class MockAttr:
                def __init__(self, val):
                    self._val = val
                def IsValid(self):
                    return True
                def Get(self):
                    return self._val
            return MockAttr(vals.get(name))

    from hydragon.editor.core.schemas import HydragonUICanvas
    canvas = HydragonUICanvas(MockCanvasPrim())
    ui._apply_canvas_schema(canvas)

    assert ui.is_main_menu_active is True
    assert ui.active_menu_type == "MainMenu"
    assert ui.is_menu_active is True
    print("  [PASS] Main Menu activated properly from schema binding")

    # Start Game transition
    ui.start_game()
    assert ui.is_main_menu_active is False
    assert ui.active_menu_type is None
    assert ui.is_menu_active is False
    print("  [PASS] start_game() transitioned from MainMenu to in-game state")

    ui.shutdown()


def test_pause_menu_lifecycle_and_resume():
    print("--- 9. Testing Pause Menu Lifecycle and Resume ---")
    ui = HydragonGameHUD()
    ui.startup()

    # Simulate in-game state
    ui._is_simulating = True
    assert ui.is_menu_active is False

    # Open Pause Menu
    ui.open_pause_menu()
    assert ui.is_pause_menu_active is True
    assert ui.active_menu_type == "PauseMenu"
    assert ui.is_menu_active is True
    print("  [PASS] Pause Menu opened")

    # Resume Game
    ui.resume_game()
    assert ui.is_pause_menu_active is False
    assert ui.active_menu_type is None
    assert ui.is_menu_active is False
    print("  [PASS] Pause Menu resumed to gameplay")

    ui.shutdown()


def test_settings_menu_navigation():
    print("--- 10. Testing Settings Menu Navigation (From MainMenu & PauseMenu) ---")
    ui = HydragonGameHUD()
    ui.startup()

    # 1. Navigation from MainMenu -> Settings -> Back to MainMenu
    ui._open_main_menu()
    assert ui.is_main_menu_active is True

    ui.open_settings_menu()
    assert ui.is_settings_menu_active is True
    assert ui.active_menu_type == "SettingsMenu"
    assert ui._previous_menu == "MainMenu"
    assert ui.is_main_menu_active is False

    ui.back_from_settings()
    assert ui.is_settings_menu_active is False
    assert ui.active_menu_type == "MainMenu"
    assert ui.is_main_menu_active is True
    print("  [PASS] MainMenu <-> SettingsMenu navigation verified")

    # 2. Navigation from PauseMenu -> Settings -> Back to PauseMenu
    ui.open_pause_menu()
    assert ui.is_pause_menu_active is True
    assert ui.active_menu_type == "PauseMenu"

    ui.open_settings_menu()
    assert ui.is_settings_menu_active is True
    assert ui.active_menu_type == "SettingsMenu"
    assert ui._previous_menu == "PauseMenu"
    assert ui.is_pause_menu_active is False

    ui.back_from_settings()
    assert ui.is_settings_menu_active is False
    assert ui.active_menu_type == "PauseMenu"
    assert ui.is_pause_menu_active is True
    print("  [PASS] PauseMenu <-> SettingsMenu navigation verified")

    ui.shutdown()


def test_ui_hierarchy_multi_canvas_discovery():
    print("--- 11. Testing Multi-Canvas Stage Hierarchy Discovery ---")
    ui = HydragonGameHUD()
    ui.startup()

    class MockPrim:
        def __init__(self, path, canvas_type):
            self._path = path
            self._type = canvas_type
        def IsValid(self):
            return True
        def GetPath(self):
            return self._path
        def HasAttribute(self, name):
            return name in ("hud:canvasType", "hud:autoActivateOnPlay")
        def GetAttribute(self, name):
            class MockAttr:
                def __init__(self, val):
                    self._val = val
                def IsValid(self):
                    return True
                def Get(self):
                    return self._val
            if name == "hud:canvasType":
                return MockAttr(self._type)
            elif name == "hud:autoActivateOnPlay":
                return MockAttr(True)
            return MockAttr(None)

    class MockStage:
        def __init__(self):
            self.prims = {
                "/World/UI/Menus/MainMenu": MockPrim("/World/UI/Menus/MainMenu", "MainMenu"),
                "/World/UI/Menus/SettingsMenu": MockPrim("/World/UI/Menus/SettingsMenu", "SettingsMenu"),
                "/World/UI/GameHUD": MockPrim("/World/UI/GameHUD", "InGame"),
                "/World/UI/PauseMenu": MockPrim("/World/UI/PauseMenu", "PauseMenu"),
            }
        def GetPrimAtPath(self, path):
            return self.prims.get(path, None)

    # Call _discover_canvas_config directly with our mock stage
    ui._discover_canvas_config(MockStage())

    assert "MainMenu" in ui._canvases
    assert "SettingsMenu" in ui._canvases
    assert "InGame" in ui._canvases
    assert "PauseMenu" in ui._canvases
    assert ui.is_main_menu_active is True
    assert ui.active_menu_type == "MainMenu"
    print("  [PASS] Discovered all 4 canvases from /World/UI hierarchy and activated MainMenu")

    ui.shutdown()


def test_game_hud_simulation_pause_lifecycle():
    print("--- 12. Testing Simulation Pause / Resume Lifecycle ---")
    ui = HydragonGameHUD()
    ui.startup()

    assert ui.is_paused is False
    assert ui.is_game_started is False

    # Simulate discovering MainMenu: _pending_menu_pause should be set
    class MockMainMenuCanvas:
        def __init__(self):
            self.canvas_type = "MainMenu"
            self.title = "TITLE"
            self.subtitle = "SUBTITLE"
            self.show_start_game = True
            self.show_settings = False
            self.show_quit = False
            self.auto_activate_on_play = True
            class MockPrim:
                def GetPath(self):
                    return "/World/UI/Menus/MainMenu"
            self.prim = MockPrim()

    ui._apply_canvas_schema(MockMainMenuCanvas())
    assert ui.is_main_menu_active is True
    assert ui._pending_menu_pause is True

    # Start Game: activates gameplay, unpauses, clears pending
    ui.start_game(play_timeline=True)
    assert ui.is_game_started is True
    assert ui.is_paused is False
    assert ui._pending_menu_pause is False
    assert ui.is_main_menu_active is False

    # Open Pause Menu: pauses game
    ui.open_pause_menu(pause_timeline=True)
    assert ui.is_paused is True
    assert ui.is_pause_menu_active is True

    # Resume Game: unpauses
    ui.resume_game(resume_timeline=True)
    assert ui.is_paused is False
    assert ui.is_pause_menu_active is False

    # Quit to Main Menu: returns to main menu, pauses, and resets game_started
    ui.quit_to_main_menu()
    assert ui.is_main_menu_active is True
    assert ui.is_game_started is False
    assert ui.is_paused is True

    ui.shutdown()
    assert ui.is_paused is False
    assert ui.is_game_started is False
    print("  [PASS] Simulation pause/resume lifecycle verified")


if __name__ == "__main__":
    test_game_hud_lifecycle()
    test_countdown_state_machine()
    test_score_popup_and_victory_modal_fail_silent()
    test_timeline_event_handling()
    test_canvas_config_binding()
    test_controls_frame_and_window_lifecycle()
    test_game_hud_dormant_without_canvas()
    test_main_menu_lifecycle_and_start_game()
    test_pause_menu_lifecycle_and_resume()
    test_settings_menu_navigation()
    test_ui_hierarchy_multi_canvas_discovery()
    test_game_hud_simulation_pause_lifecycle()
    print("\n=======================================================")
    print(" ALL GAME HUD & MENU TESTS PASSED! (12/12)")
    print("=======================================================")

