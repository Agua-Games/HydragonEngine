"""
Hydragon Engine - Game HUD & In-Game Visual Overlays

Manages non-intrusive on-screen visual feedback for gameplay:
1. Start Countdown Banner ("3... 2... 1... MARBLE GAME START!") with transparent background.
2. In-Game Controls / Hotkeys Card (WASD, Jump, Camera Orbit).
3. Floating Score Popups ("+100" / "+50") upon foe sphere destruction.
4. Polished Victory Modal with final score, time, and restart prompt.
"""

from typing import Any, Dict, List, Optional, Tuple

try:
    import carb
    import carb.input
    import omni.appwindow
    import omni.kit.app
    import omni.timeline
    import omni.ui as ui
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    ui = None


class HydragonGameHUD:
    """
    Singleton manager for in-game HUD widgets and gameplay visual overlays.
    Adheres strictly to the fail-silent principle outside Omniverse Kit.
    """

    _instance: Optional["HydragonGameHUD"] = None

    def __init__(self):
        HydragonGameHUD._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False

        self._timeline_sub = None
        self._app_update_sub = None
        self._keyboard_sub = None

        # Countdown State
        self._countdown_elapsed: float = 0.0
        self._countdown_active: bool = False
        self._countdown_window = None
        self._countdown_frame = None
        self._countdown_label = None

        # Controls HUD State
        self._controls_window = None
        self._controls_frame = None

        # Floating Score Popups
        self._active_popups: List[dict] = []
        self._popup_counter: int = 0
        self._popups_frame = None

        # Victory Modal
        self._victory_window = None
        self._victory_frame = None

        # Menus & Hierarchy State
        self._canvases: Dict[str, Any] = {}
        self._active_menu: Optional[str] = None
        self._previous_menu: str = "MainMenu"
        self._main_menu_window = None
        self._pause_menu_window = None
        self._settings_menu_window = None

        # Game State & Simulation Pause
        self._is_paused: bool = False
        self._game_started: bool = False
        self._pending_menu_pause: bool = False

        # Mock action counters for testing
        self._mock_start_game_count: int = 0
        self._mock_pause_count: int = 0
        self._mock_resume_count: int = 0
        self._mock_settings_count: int = 0
        self._mock_quit_count: int = 0

        # Declarative Canvas Settings (defaults to False for strict opt-in architecture)
        self._show_controls: bool = False
        self._show_countdown: bool = False
        self._show_score_popups: bool = False
        self._active_canvas_path: Optional[str] = None

    @classmethod
    def get_instance(cls) -> Optional["HydragonGameHUD"]:
        return cls._instance

    @property
    def is_paused(self) -> bool:
        return self._is_paused

    @property
    def is_game_started(self) -> bool:
        return self._game_started

    @property
    def active_menu_type(self) -> Optional[str]:
        return self._active_menu

    @property
    def is_menu_active(self) -> bool:
        return self._active_menu is not None

    @property
    def is_main_menu_active(self) -> bool:
        return self._active_menu == "MainMenu"

    @property
    def is_pause_menu_active(self) -> bool:
        return self._active_menu == "PauseMenu"

    @property
    def is_settings_menu_active(self) -> bool:
        return self._active_menu == "SettingsMenu"

    def _get_viewport_bounds(self) -> Tuple[int, int, int, int]:
        """Returns (x, y, width, height) of active viewport window for proper overlay placement."""
        try:
            import omni.kit.viewport.utility as vp_util
            vp_win = vp_util.get_active_viewport_window()
            if vp_win:
                x = int(getattr(vp_win, "position_x", 0))
                y = int(getattr(vp_win, "position_y", 0))
                w = int(getattr(vp_win, "width", 800))
                h = int(getattr(vp_win, "height", 600))
                if w > 100 and h > 100:
                    return x, y, w, h
        except Exception:
            pass
        return 50, 50, 800, 600

    def _world_to_screen(self, world_pos: Tuple[float, float, float]) -> Tuple[int, int]:
        """Projects 3D world coordinates to 2D screen coordinates within active viewport."""
        vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()
        default_x = int(vp_x + vp_w * 0.5 - 80)
        default_y = int(vp_y + vp_h * 0.45)

        try:
            from pxr import Usd, UsdGeom, Gf
            import omni.usd
            stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
            if stage:
                from .camera_controller import HydragonCameraControllerSystem
                cam_sys = HydragonCameraControllerSystem.get_instance()
                cam_prim = cam_sys.find_camera_prim(stage) if cam_sys else None
                if cam_prim and cam_prim.IsValid():
                    usd_cam = UsdGeom.Camera(cam_prim)
                    gf_cam = usd_cam.GetCamera(Usd.TimeCode.Default())
                    frustum = gf_cam.frustum
                    view_matrix = frustum.ComputeViewMatrix()
                    proj_matrix = frustum.ComputeProjectionMatrix()
                    pt = Gf.Vec4d(float(world_pos[0]), float(world_pos[1]), float(world_pos[2]), 1.0)
                    clip_pt = pt * view_matrix * proj_matrix
                    w = clip_pt[3]
                    if w > 0.01:
                        ndc_x = clip_pt[0] / w
                        ndc_y = clip_pt[1] / w
                        screen_x = int(vp_x + (ndc_x + 1.0) * 0.5 * vp_w - 80)
                        screen_y = int(vp_y + (1.0 - ndc_y) * 0.5 * vp_h - 30)
                        screen_x = max(vp_x + 20, min(vp_x + vp_w - 180, screen_x))
                        screen_y = max(vp_y + 20, min(vp_y + vp_h - 80, screen_y))
                        return screen_x, screen_y
        except Exception:
            pass

        return default_x, default_y

    def _get_viewport_size(self) -> Tuple[int, int]:
        """Returns (width, height) of the active viewport."""
        try:
            import omni.kit.viewport.utility as vp_util
            vp_win = vp_util.get_active_viewport_window()
            if vp_win:
                w = int(getattr(vp_win, "width", 800))
                h = int(getattr(vp_win, "height", 600))
                if w > 100 and h > 100:
                    return w, h
        except Exception:
            pass
        return 800, 600

    def _world_to_viewport_local(self, world_pos: Tuple[float, float, float]) -> Tuple[int, int]:
        """Projects 3D world coordinates to 2D local coordinates within the active viewport frame."""
        vp_w, vp_h = self._get_viewport_size()
        default_x = int(vp_w * 0.5 - 50)
        default_y = int(vp_h * 0.45)

        try:
            from pxr import Usd, UsdGeom, Gf
            import omni.usd
            stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
            if stage:
                from .camera_controller import HydragonCameraControllerSystem
                cam_sys = HydragonCameraControllerSystem.get_instance()
                cam_prim = cam_sys.find_camera_prim(stage) if cam_sys else None
                if cam_prim and cam_prim.IsValid():
                    usd_cam = UsdGeom.Camera(cam_prim)
                    gf_cam = usd_cam.GetCamera(Usd.TimeCode.Default())
                    frustum = gf_cam.frustum
                    view_matrix = frustum.ComputeViewMatrix()
                    proj_matrix = frustum.ComputeProjectionMatrix()
                    pt = Gf.Vec4d(float(world_pos[0]), float(world_pos[1]), float(world_pos[2]), 1.0)
                    clip_pt = pt * view_matrix * proj_matrix
                    w = clip_pt[3]
                    if w > 0.01:
                        ndc_x = clip_pt[0] / w
                        ndc_y = clip_pt[1] / w
                        local_x = int((ndc_x + 1.0) * 0.5 * vp_w - 50)
                        local_y = int((1.0 - ndc_y) * 0.5 * vp_h - 20)
                        local_x = max(20, min(vp_w - 120, local_x))
                        local_y = max(20, min(vp_h - 60, local_y))
                        return local_x, local_y
        except Exception:
            pass

        return default_x, default_y

    def startup(self):
        """Initializes HUD subscriptions for timeline, keyboard input, and per-frame app updates."""
        if not HAS_KIT:
            return
        self._is_active = True
        self._subscribe_timeline()
        self._subscribe_keyboard()
        self._subscribe_app_update()
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonGameHUD initialized.")

    def shutdown(self):
        """Releases all subscriptions and cleans up all active HUD and menu windows."""
        self._is_active = False
        self._is_simulating = False
        self._timeline_sub = None
        self._app_update_sub = None

        if self._keyboard_sub and carb and hasattr(carb, "input"):
            try:
                input_iface = carb.input.acquire_input_interface()
                appwindow = omni.appwindow.get_default_app_window()
                if appwindow and input_iface:
                    keyboard = appwindow.get_keyboard()
                    if keyboard:
                        input_iface.unsubscribe_to_keyboard_events(keyboard, self._keyboard_sub)
            except Exception:
                pass
            self._keyboard_sub = None

        self._destroy_all_menus()
        self._destroy_countdown_ui()
        self._destroy_controls_ui()
        self._destroy_all_popups()
        self._destroy_victory_ui()

        self._canvases.clear()
        self._active_menu = None
        self._is_paused = False
        self._game_started = False
        self._pending_menu_pause = False

        HydragonGameHUD._instance = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonGameHUD shutdown.")

    # -------------------------------------------------------------------------
    # Event Subscriptions
    # -------------------------------------------------------------------------
    def _subscribe_timeline(self):
        try:
            timeline = omni.timeline.get_timeline_interface()
            stream = timeline.get_timeline_event_stream()
            self._timeline_sub = stream.create_subscription_to_pop(self._on_timeline_event)
            self._is_simulating = timeline.is_playing()
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Game HUD timeline subscription failed: {e}")

    def _subscribe_keyboard(self):
        if not HAS_KIT or not carb or not hasattr(carb, "input"):
            return
        try:
            input_iface = carb.input.acquire_input_interface()
            appwindow = omni.appwindow.get_default_app_window()
            if appwindow and input_iface:
                keyboard = appwindow.get_keyboard()
                if keyboard:
                    self._keyboard_sub = input_iface.subscribe_to_keyboard_events(
                        keyboard, self._on_keyboard_event
                    )
                    if carb:
                        carb.log_info("[hydragon.editor.core] UI System subscribed to keyboard events.")
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to subscribe keyboard for UI: {ex}")

    def _on_keyboard_event(self, e):
        if not self._is_simulating or not carb or not hasattr(carb, "input"):
            return True
        try:
            if e.type == carb.input.KeyboardEventType.KEY_PRESS:
                key = e.input
                # ESC (Escape) or P toggles PauseMenu
                if key in (carb.input.KeyboardInput.ESCAPE, carb.input.KeyboardInput.P):
                    if self._active_menu == "SettingsMenu":
                        self.back_from_settings()
                    elif self._active_menu == "PauseMenu":
                        self.resume_game()
                    elif self._active_menu is None and self._game_started:
                        if "PauseMenu" in self._canvases:
                            self.open_pause_menu()
        except Exception:
            pass
        return True

    def _subscribe_app_update(self):
        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                self._on_app_update
            )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Game HUD app update subscription failed: {e}")

    # -------------------------------------------------------------------------
    # Declarative UI Canvas Discovery (One-time discovery on PLAY)
    # -------------------------------------------------------------------------
    def _discover_canvas_config(self, stage=None):
        """
        Discovers active HydragonUICanvas prims on simulation start.
        Supports recommended UI hierarchy (/World/UI/Menus/MainMenu, /World/UI/GameHUD, etc.)
        as well as standalone canvas prims.
        """
        self._show_controls = False
        self._show_countdown = False
        self._show_score_popups = False
        self._active_canvas_path = None
        self._canvases.clear()
        self._active_menu = None

        try:
            if stage is None:
                try:
                    import omni.usd
                    stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                except Exception:
                    stage = None
            if not stage:
                return

            from .schemas import HydragonUICanvas

            # 1. Direct O(1) checks of canonical UI hierarchy paths (avoiding traversal anti-patterns)
            canonical_paths = [
                "/World/UI/Menus/MainMenu",
                "/World/UI/Menus/SettingsMenu",
                "/World/UI/GameHUD",
                "/World/UI/PauseMenu",
                "/UI/Menus/MainMenu",
                "/UI/Menus/SettingsMenu",
                "/UI/GameHUD",
                "/UI/PauseMenu",
                "/World/GameHUD",
                "/World/MainMenu",
                "/World/PauseMenu",
                "/World/SettingsMenu",
                "/World/UICanvas",
                "/World/hydragon_ui_canvas_root",
            ]
            for p in canonical_paths:
                prim = stage.GetPrimAtPath(p)
                if prim and prim.IsValid() and HydragonUICanvas.is_applied(prim):
                    c = HydragonUICanvas(prim)
                    if c.canvas_type not in self._canvases:
                        self._canvases[c.canvas_type] = c

            # 2. Stage traversal fallback (only if no canonical canvases found)
            if not self._canvases and hasattr(stage, "Traverse"):
                for prim in stage.Traverse():
                    if HydragonUICanvas.is_applied(prim):
                        c = HydragonUICanvas(prim)
                        if c.canvas_type not in self._canvases:
                            self._canvases[c.canvas_type] = c

            # Auto-activate MainMenu or InGame HUD if found
            if "MainMenu" in self._canvases:
                main_canvas = self._canvases["MainMenu"]
                if getattr(main_canvas, "auto_activate_on_play", True):
                    self._open_main_menu()
            elif "InGame" in self._canvases:
                in_game_canvas = self._canvases["InGame"]
                self._apply_in_game_canvas(in_game_canvas)

            if carb and self._canvases:
                types_str = ", ".join(self._canvases.keys())
                carb.log_info(f"[hydragon.editor.core] Discovered {len(self._canvases)} UI Canvases: {types_str}")

        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to discover UI canvas: {ex}")

    def _apply_in_game_canvas(self, canvas):
        self._active_canvas_path = str(canvas.prim.GetPath())
        self._show_controls = canvas.show_controls
        self._show_countdown = canvas.show_countdown
        self._show_score_popups = canvas.show_score_popups
        if carb:
            carb.log_info(
                f"[hydragon.editor.core] InGame UI Canvas bound at {self._active_canvas_path}: "
                f"controls={self._show_controls}, countdown={self._show_countdown}, popups={self._show_score_popups}"
            )

    def _apply_canvas_schema(self, canvas):
        c_type = getattr(canvas, "canvas_type", "InGame")
        self._canvases[c_type] = canvas
        if c_type == "MainMenu":
            if getattr(canvas, "auto_activate_on_play", True):
                self._open_main_menu()
                self._pending_menu_pause = True
        elif c_type == "PauseMenu":
            pass
        elif c_type == "SettingsMenu":
            pass
        else:
            self._apply_in_game_canvas(canvas)

    # -------------------------------------------------------------------------
    # Timeline & Frame Update Callbacks
    # -------------------------------------------------------------------------
    def _on_timeline_event(self, e):
        if not HAS_KIT:
            return
        try:
            event_type = int(e.type)
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                self._is_simulating = True
                self._is_paused = False

                if not self._game_started:
                    self._destroy_all_menus()
                    self._destroy_victory_ui()
                    self._destroy_all_popups()
                    self._discover_canvas_config()

                    # Prioritize Main Menu if authored and set to auto-activate
                    if "MainMenu" in self._canvases and getattr(self._canvases["MainMenu"], "auto_activate_on_play", True):
                        self._open_main_menu()
                        self._pending_menu_pause = True
                    elif "InGame" in self._canvases:
                        self._game_started = True
                        canvas = self._canvases["InGame"]
                        if getattr(canvas, "auto_activate_on_play", True):
                            self._apply_in_game_canvas(canvas)
                            if self._show_countdown:
                                self._start_countdown()
                            if self._show_controls:
                                self._show_controls_ui()
                    else:
                        self._game_started = True
                else:
                    # Resumed via Kit toolbar or timeline API while in-game
                    if self._active_menu == "PauseMenu":
                        self.resume_game(resume_timeline=False)

            elif event_type == int(omni.timeline.TimelineEventType.PAUSE):
                self._is_paused = True
                # Simulation paused in place: do NOT destroy active menus!
                # If in-game and no menu is active, open PauseMenu
                if self._game_started and not self.is_menu_active and "PauseMenu" in self._canvases:
                    self.open_pause_menu(pause_timeline=False)

            elif event_type == int(omni.timeline.TimelineEventType.STOP):
                self._is_simulating = False
                self._is_paused = False
                self._game_started = False
                self._pending_menu_pause = False
                self._destroy_all_menus()
                self._destroy_countdown_ui()
                self._destroy_controls_ui()
                self._destroy_all_popups()
                self._destroy_victory_ui()
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Game HUD timeline event error: {ex}")

    def _on_app_update(self, e):
        if not HAS_KIT or not self._is_simulating:
            return

        if self._pending_menu_pause:
            self._pending_menu_pause = False
            try:
                omni.timeline.get_timeline_interface().pause()
            except Exception:
                pass

        dt = e.payload.get("dt", 1.0 / 60.0) if hasattr(e, "payload") else 1.0 / 60.0

        # Maintain visibility and keep anchored relative to active viewport bounds (e.g. F7/F11 fullscreen)
        vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()

        if self._main_menu_window and getattr(self._main_menu_window, "visible", False):
            self._main_menu_window.position_x = int(vp_x + (vp_w - 440) * 0.5)
            self._main_menu_window.position_y = int(vp_y + (vp_h - 360) * 0.5)

        if self._pause_menu_window and getattr(self._pause_menu_window, "visible", False):
            self._pause_menu_window.position_x = int(vp_x + (vp_w - 420) * 0.5)
            self._pause_menu_window.position_y = int(vp_y + (vp_h - 360) * 0.5)

        if self._settings_menu_window and getattr(self._settings_menu_window, "visible", False):
            self._settings_menu_window.position_x = int(vp_x + (vp_w - 480) * 0.5)
            self._settings_menu_window.position_y = int(vp_y + (vp_h - 420) * 0.5)

        if self._controls_window:
            if not self._controls_window.visible:
                self._controls_window.visible = True
            target_cx = int(vp_x + 20)
            target_cy = int(vp_y + 20)
            if getattr(self._controls_window, "position_x", None) != target_cx:
                self._controls_window.position_x = target_cx
            if getattr(self._controls_window, "position_y", None) != target_cy:
                self._controls_window.position_y = target_cy

        if self._countdown_active and self._countdown_window:
            if not self._countdown_window.visible:
                self._countdown_window.visible = True
            target_kx = int(vp_x + (vp_w - 650) * 0.5)
            target_ky = int(vp_y + vp_h * 0.25)
            if getattr(self._countdown_window, "position_x", None) != target_kx:
                self._countdown_window.position_x = target_kx
            if getattr(self._countdown_window, "position_y", None) != target_ky:
                self._countdown_window.position_y = target_ky

        if self._victory_window:
            if not self._victory_window.visible:
                self._victory_window.visible = True
            target_vx = int(vp_x + (vp_w - 460) * 0.5)
            target_vy = int(vp_y + (vp_h - 220) * 0.5)
            if getattr(self._victory_window, "position_x", None) != target_vx:
                self._victory_window.position_x = target_vx
            if getattr(self._victory_window, "position_y", None) != target_vy:
                self._victory_window.position_y = target_vy

        # In-game countdown and popups update ONLY when unpaused and gameplay has started
        if self._game_started and not self._is_paused:
            if self._countdown_active:
                self._update_countdown(dt)
            if self._active_popups:
                self._update_popups(dt)

    # -------------------------------------------------------------------------
    # 1. Start Countdown Banner ("3... 2... 1... MARBLE GAME START!")
    # -------------------------------------------------------------------------
    def _start_countdown(self):
        """Spawns the transparent, centered countdown banner inside active viewport frame."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_countdown_ui()
        self._countdown_elapsed = 0.0
        self._countdown_active = True

        # Primary approach: inject directly into active Viewport's frame (visible in F7/F11 fullscreen)
        try:
            import omni.kit.viewport.utility as vp_util
            vp_win = vp_util.get_active_viewport_window()
            if vp_win and hasattr(vp_win, "get_frame"):
                frame = vp_win.get_frame("HydragonCountdownOverlay")
                if frame:
                    frame.clear()
                    with frame:
                        with ui.VStack():
                            ui.Spacer(height=ui.Percent(22))
                            with ui.HStack():
                                ui.Spacer()
                                self._countdown_label = ui.Label(
                                    "3...",
                                    width=650,
                                    height=90,
                                    alignment=ui.Alignment.CENTER,
                                    style={
                                        "color": 0xFF20E0FF,  # Bright Gold / Amber
                                        "font_size": 48,
                                        "alignment": ui.Alignment.CENTER,
                                    },
                                )
                                ui.Spacer()
                            ui.Spacer()
                    self._countdown_frame = frame
                    return
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to build countdown frame overlay: {e}")

        # Fallback: Floating omni.ui.Window
        try:
            vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()
            window = ui.Window(
                "HydragonCountdownOverlay",
                width=650,
                height=180,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE
                | ui.WINDOW_FLAGS_NO_BACKGROUND,
            )
            if getattr(window, "docked", False):
                try:
                    window.undock()
                except Exception:
                    pass

            window.position_x = int(vp_x + (vp_w - 650) * 0.5)
            window.position_y = int(vp_y + vp_h * 0.25)

            with window.frame:
                with ui.VStack():
                    ui.Spacer(height=20)
                    self._countdown_label = ui.Label(
                        "3...",
                        width=650,
                        height=90,
                        alignment=ui.Alignment.CENTER,
                        style={
                            "color": 0xFF20E0FF,  # Bright Gold / Amber
                            "font_size": 48,
                            "alignment": ui.Alignment.CENTER,
                        },
                    )
                    ui.Spacer(height=20)

            self._countdown_window = window
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to create countdown overlay: {e}")

    def _update_countdown(self, dt: float):
        self._countdown_elapsed += dt
        t = self._countdown_elapsed

        if not self._countdown_label:
            return

        if t < 1.0:
            self._countdown_label.text = "3..."
        elif t < 2.0:
            self._countdown_label.text = "2..."
        elif t < 3.0:
            self._countdown_label.text = "1..."
        elif t < 4.2:
            self._countdown_label.text = "MARBLE GAME START!"
            style = {
                "color": 0xFF30E050,  # Bright Green
                "font_size": 42,
            }
            if ui and hasattr(ui, "Alignment"):
                style["alignment"] = ui.Alignment.CENTER
            self._countdown_label.style = style
        else:
            # Countdown complete: dismiss
            self._countdown_active = False
            self._destroy_countdown_ui()

    def _destroy_countdown_ui(self):
        self._countdown_active = False
        if self._countdown_frame:
            try:
                self._countdown_frame.clear()
            except Exception:
                pass
            self._countdown_frame = None
        if self._countdown_window:
            try:
                self._countdown_window.visible = False
            except Exception:
                pass
            self._countdown_window = None
        self._countdown_label = None

    # -------------------------------------------------------------------------
    # 2. Controls / Hotkeys Card (Non-intrusive Semi-Transparent HUD)
    # -------------------------------------------------------------------------
    def _show_controls_ui(self):
        """Displays non-intrusive hotkeys panel anchored inside active viewport."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_controls_ui()

        # Primary approach: inject directly into active Viewport's frame.
        # Overlays attached to viewport_window.get_frame() reside in the viewport's
        # internal z_stack and persist through presentation (F7) and fullscreen (F11) modes.
        try:
            import omni.kit.viewport.utility as vp_util
            vp_win = vp_util.get_active_viewport_window()
            if vp_win and hasattr(vp_win, "get_frame"):
                frame = vp_win.get_frame("HydragonControlsOverlay")
                if frame:
                    frame.clear()
                    with frame:
                        with ui.VStack():
                            ui.Spacer(height=20)
                            with ui.HStack():
                                ui.Spacer(width=20)
                                with ui.ZStack(width=240, height=155):
                                    ui.Rectangle(
                                        style={
                                            "background_color": 0x66141822,
                                            "border_radius": 6,
                                            "border_color": 0x4488AACC,
                                            "border_width": 1,
                                        }
                                    )
                                    with ui.VStack(spacing=4):
                                        ui.Spacer(height=6)
                                        ui.Label(
                                            "  CONTROLS",
                                            style={
                                                "color": 0xFF40D0FF,
                                                "font_size": 15,
                                            },
                                        )
                                        with ui.HStack(spacing=6):
                                            ui.Label("  W, A, S, D", style={"color": 0xFFFFFFFF, "font_size": 13, "width": 85})
                                            ui.Label("Roll Marble", style={"color": 0xFFCCCCCC, "font_size": 13})
                                        with ui.HStack(spacing=6):
                                            ui.Label("  Space / C", style={"color": 0xFFFFFFFF, "font_size": 13, "width": 85})
                                            ui.Label("Jump", style={"color": 0xFFCCCCCC, "font_size": 13})
                                        with ui.HStack(spacing=6):
                                            ui.Label("  Right Drag", style={"color": 0xFFFFFFFF, "font_size": 13, "width": 85})
                                            ui.Label("Orbit Camera", style={"color": 0xFFCCCCCC, "font_size": 13})
                                        with ui.HStack(spacing=6):
                                            ui.Label("  Scroll", style={"color": 0xFFFFFFFF, "font_size": 13, "width": 85})
                                            ui.Label("Zoom Camera", style={"color": 0xFFCCCCCC, "font_size": 13})
                                        ui.Spacer(height=6)
                                ui.Spacer()
                            ui.Spacer()
                    self._controls_frame = frame
                    return
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Viewport frame overlay creation failed, falling back to window: {e}")

        # Fallback: Floating omni.ui.Window
        try:
            vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()
            window = ui.Window(
                "HydragonControlsOverlay",
                width=240,
                height=150,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE,
            )
            if getattr(window, "docked", False):
                try:
                    window.undock()
                except Exception:
                    pass

            window.position_x = int(vp_x + 20)
            window.position_y = int(vp_y + 20)

            # Semi-transparent dark slate backdrop with subtle border
            window.frame.style = {
                "background_color": 0x66141822,
                "border_radius": 6,
                "border_color": 0x4488AACC,
                "border_width": 1,
            }
            with window.frame:
                with ui.VStack(spacing=4):
                    ui.Label(
                        "CONTROLS",
                        style={
                            "color": 0xFF40D0FF,
                            "font_size": 15,
                            "margin": 4,
                        },
                    )
                    with ui.HStack(spacing=6):
                        ui.Label("  W, A, S, D", style={"color": 0xFFFFFFFF, "font_size": 13, "width": 85})
                        ui.Label("Roll Marble", style={"color": 0xFFCCCCCC, "font_size": 13})
                    with ui.HStack(spacing=6):
                        ui.Label("  Space / C", style={"color": 0xFFFFFFFF, "font_size": 13, "width": 85})
                        ui.Label("Jump", style={"color": 0xFFCCCCCC, "font_size": 13})
                    with ui.HStack(spacing=6):
                        ui.Label("  Right Drag", style={"color": 0xFFFFFFFF, "font_size": 13, "width": 85})
                        ui.Label("Orbit Camera", style={"color": 0xFFCCCCCC, "font_size": 13})
                    with ui.HStack(spacing=6):
                        ui.Label("  Scroll", style={"color": 0xFFFFFFFF, "font_size": 13, "width": 85})
                        ui.Label("Zoom Camera", style={"color": 0xFFCCCCCC, "font_size": 13})

            self._controls_window = window
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to build controls overlay: {e}")

    def _destroy_controls_ui(self):
        if self._controls_frame:
            try:
                self._controls_frame.clear()
            except Exception:
                pass
            self._controls_frame = None
        if self._controls_window:
            try:
                self._controls_window.visible = False
            except Exception:
                pass
            self._controls_window = None

    # -------------------------------------------------------------------------
    # 3. Floating Score Popups ("+100" / "+50" on Foe Destruction)
    # -------------------------------------------------------------------------
    def show_score_popup(self, world_pos: Tuple[float, float, float] = (0.0, 0.0, 0.0), points: int = 100):
        """
        Triggers a transient floating score text (+100) that floats upward and fades out.
        Anchored directly inside active viewport frame so it is always visible in F7/F11 fullscreen.
        """
        if not HAS_KIT or not self._show_score_popups:
            return

        if ui is None:
            return

        try:
            self._popup_counter += 1
            local_x, local_y = self._world_to_viewport_local(world_pos)
            popup = {
                "id": self._popup_counter,
                "points": points,
                "x": local_x,
                "initial_y": local_y,
                "current_y": local_y,
                "elapsed": 0.0,
                "duration": 1.4,
                "placer": None,
            }
            self._active_popups.append(popup)
            self._attach_popup_widget(popup)
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to spawn score popup: {e}")

    def _attach_popup_widget(self, popup: dict):
        """Builds retained Placer and Label widgets for a new popup once, avoiding per-frame rebuilds."""
        if not HAS_KIT or ui is None:
            return

        try:
            import omni.kit.viewport.utility as vp_util
            vp_win = vp_util.get_active_viewport_window()
            if not (vp_win and hasattr(vp_win, "get_frame")):
                return

            if self._popups_frame is None:
                self._popups_frame = vp_win.get_frame("HydragonScorePopupsOverlay")
                self._popups_frame.clear()

            px = int(popup.get("x", 100))
            py = int(popup.get("current_y", 100))
            pts = popup.get("points", 100)

            with self._popups_frame:
                placer = ui.Placer(offset_x=px, offset_y=py, width=180, height=50)
                with placer:
                    ui.Label(
                        f"+{pts}",
                        width=180,
                        height=50,
                        alignment=ui.Alignment.CENTER,
                        style={
                            "color": 0xFF20E0FF if pts >= 150 else 0xFF00E6FF,
                            "font_size": 36,
                            "alignment": ui.Alignment.CENTER,
                        },
                    )
                popup["placer"] = placer
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to attach score popup widget: {e}")

    def _update_popups(self, dt: float):
        """Animates floating score popups by updating placer offset_y directly with zero frame rebuilding."""
        if not self._active_popups:
            return

        remaining = []
        for popup in self._active_popups:
            popup["elapsed"] += dt
            progress = popup["elapsed"] / popup["duration"]
            if progress < 1.0:
                new_y = popup["initial_y"] - int(progress * 60.0)
                popup["current_y"] = new_y
                placer = popup.get("placer")
                if placer:
                    try:
                        placer.offset_y = new_y
                    except Exception:
                        pass
                remaining.append(popup)
            else:
                placer = popup.get("placer")
                if placer:
                    try:
                        placer.visible = False
                    except Exception:
                        pass

        self._active_popups = remaining
        if not self._active_popups and self._popups_frame:
            try:
                self._popups_frame.clear()
            except Exception:
                pass
            self._popups_frame = None

    def _destroy_all_popups(self):
        self._active_popups.clear()
        if self._popups_frame:
            try:
                self._popups_frame.clear()
            except Exception:
                pass
            self._popups_frame = None

    # -------------------------------------------------------------------------
    # 4. Polished Victory Modal ("Mission Accomplished!")
    # -------------------------------------------------------------------------
    def show_victory(self, score: int, elapsed: float, foes: int):
        """Displays a polished, high-contrast victory screen anchored in active viewport frame."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_victory_ui()

        # Primary approach: viewport frame (always visible in F7/F11 fullscreen)
        try:
            import omni.kit.viewport.utility as vp_util
            vp_win = vp_util.get_active_viewport_window()
            if vp_win and hasattr(vp_win, "get_frame"):
                frame = vp_win.get_frame("HydragonVictoryOverlay")
                if frame:
                    frame.clear()
                    with frame:
                        with ui.VStack():
                            ui.Spacer()
                            with ui.HStack():
                                ui.Spacer()
                                with ui.ZStack(width=460, height=220):
                                    ui.Rectangle(
                                        style={
                                            "background_color": 0xDD111824,
                                            "border_radius": 8,
                                            "border_color": 0xFF20D040,
                                            "border_width": 2,
                                        }
                                    )
                                    with ui.VStack(spacing=8):
                                        ui.Spacer(height=8)
                                        ui.Label(
                                            "🏆 MISSION ACCOMPLISHED!",
                                            width=440,
                                            height=40,
                                            alignment=ui.Alignment.CENTER,
                                            style={
                                                "color": 0xFF20E050,
                                                "font_size": 26,
                                                "alignment": ui.Alignment.CENTER,
                                            },
                                        )
                                        ui.Label(
                                            f"FINAL SCORE: {score}",
                                            width=440,
                                            height=35,
                                            alignment=ui.Alignment.CENTER,
                                            style={
                                                "color": 0xFFFFFFFF,
                                                "font_size": 22,
                                                "alignment": ui.Alignment.CENTER,
                                            },
                                        )
                                        ui.Label(
                                            f"Time: {elapsed:.1f}s   |   Foes Defeated: {foes}",
                                            width=440,
                                            height=25,
                                            alignment=ui.Alignment.CENTER,
                                            style={
                                                "color": 0xFFCCCCCC,
                                                "font_size": 15,
                                                "alignment": ui.Alignment.CENTER,
                                            },
                                        )
                                        ui.Spacer(height=4)
                                        ui.Label(
                                            "[ Press STOP and PLAY to restart! ]",
                                            width=440,
                                            height=25,
                                            alignment=ui.Alignment.CENTER,
                                            style={
                                                "color": 0xFF50D0FF,
                                                "font_size": 13,
                                                "alignment": ui.Alignment.CENTER,
                                            },
                                        )
                                        ui.Spacer(height=8)
                                ui.Spacer()
                            ui.Spacer()
                    self._victory_frame = frame
                    return
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to build victory frame overlay: {e}")

        # Fallback: Floating omni.ui.Window
        try:
            vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()
            window = ui.Window(
                "HydragonVictoryModal",
                width=460,
                height=220,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE,
            )
            if getattr(window, "docked", False):
                try:
                    window.undock()
                except Exception:
                    pass

            window.position_x = int(vp_x + (vp_w - 460) * 0.5)
            window.position_y = int(vp_y + (vp_h - 220) * 0.5)

            # Polished dark translucent card with gold highlight border
            window.frame.style = {
                "background_color": 0xDD111824,
                "border_radius": 8,
                "border_color": 0xFF20D040,
                "border_width": 2,
            }
            with window.frame:
                with ui.VStack(spacing=8):
                    ui.Spacer(height=8)
                    ui.Label(
                        "🏆 MISSION ACCOMPLISHED!",
                        style={
                            "color": 0xFF20E050,
                            "font_size": 26,
                            "alignment": ui.Alignment.CENTER,
                        },
                    )
                    ui.Label(
                        f"FINAL SCORE: {score}",
                        style={
                            "color": 0xFFFFFFFF,
                            "font_size": 22,
                            "alignment": ui.Alignment.CENTER,
                        },
                    )
                    ui.Label(
                        f"Time: {elapsed:.1f}s   |   Foes Defeated: {foes}",
                        style={
                            "color": 0xFFCCCCCC,
                            "font_size": 15,
                            "alignment": ui.Alignment.CENTER,
                        },
                    )
                    ui.Spacer(height=4)
                    ui.Label(
                        "[ Press STOP and PLAY to restart! ]",
                        style={
                            "color": 0xFF50D0FF,
                            "font_size": 13,
                            "alignment": ui.Alignment.CENTER,
                        },
                    )
                    ui.Spacer(height=8)

            self._victory_window = window
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to build victory modal: {e}")

    def _destroy_victory_ui(self):
        if self._victory_frame:
            try:
                self._victory_frame.clear()
            except Exception:
                pass
            self._victory_frame = None
        if self._victory_window:
            try:
                self._victory_window.visible = False
            except Exception:
                pass
            self._victory_window = None

    # -------------------------------------------------------------------------
    # 5. Main Menu, Pause Menu & Settings Menu Methods
    # -------------------------------------------------------------------------
    def start_game(self, play_timeline: bool = True):
        """Starts gameplay from Main Menu, enabling player input and triggering countdown/HUD."""
        self._mock_start_game_count += 1
        self._game_started = True
        self._is_paused = False
        self._pending_menu_pause = False
        self._destroy_main_menu_ui()
        self._active_menu = None
        self._set_player_input_enabled(True)

        if play_timeline and HAS_KIT:
            try:
                omni.timeline.get_timeline_interface().play()
            except Exception:
                pass

        if "InGame" in self._canvases:
            canvas = self._canvases["InGame"]
            self._apply_in_game_canvas(canvas)
            if self._show_countdown:
                self._start_countdown()
            if self._show_controls:
                self._show_controls_ui()
        else:
            self._show_controls = True
            self._show_countdown = True
            self._start_countdown()
            self._show_controls_ui()

        if carb:
            carb.log_info("[hydragon.editor.core] Main Menu: Start Game triggered.")

    def _open_main_menu(self):
        """Activates and displays the Main Menu, suspending player input."""
        self._destroy_all_menus()
        self._active_menu = "MainMenu"
        self._is_paused = True
        self._set_player_input_enabled(False)
        self._show_main_menu_ui()
        if carb:
            carb.log_info("[hydragon.editor.core] Main Menu opened.")

    def _show_main_menu_ui(self):
        """Displays the centered Main Menu overlay window."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_main_menu_ui()
        canvas = self._canvases.get("MainMenu")
        title_text = canvas.title if canvas and canvas.title else "HYDRAGON"
        subtitle_text = canvas.subtitle if canvas and canvas.subtitle else "A Next-Gen OpenUSD Game Engine"
        show_start = canvas.show_start_game if canvas else True
        show_settings = canvas.show_settings if canvas else True
        show_quit = canvas.show_quit if canvas else True

        try:
            vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()
            win_w, win_h = 440, 360

            window = ui.Window(
                "HydragonMainMenuWindow",
                width=win_w,
                height=win_h,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE,
            )
            if getattr(window, "docked", False):
                try:
                    window.undock()
                except Exception:
                    pass

            window.position_x = int(vp_x + (vp_w - win_w) * 0.5)
            window.position_y = int(vp_y + (vp_h - win_h) * 0.5)

            with window.frame:
                with ui.ZStack():
                    ui.Rectangle(
                        style={
                            "background_color": 0xF012151B,
                            "border_color": 0x66FFB800,
                            "border_width": 2.0,
                            "border_radius": 14.0,
                        }
                    )
                    with ui.VStack(spacing=12):
                        ui.Spacer(height=16)
                        ui.Label(
                            title_text,
                            alignment=ui.Alignment.CENTER,
                            style={
                                "color": 0xFFFFC820,
                                "font_size": 32,
                                "alignment": ui.Alignment.CENTER,
                            },
                        )
                        ui.Label(
                            subtitle_text,
                            alignment=ui.Alignment.CENTER,
                            style={
                                "color": 0xFF9EA7B4,
                                "font_size": 14,
                                "alignment": ui.Alignment.CENTER,
                            },
                        )
                        ui.Spacer(height=12)

                        btn_style = {
                            "background_color": 0xCC1F2430,
                            "border_color": 0x44FFB800,
                            "border_width": 1.0,
                            "border_radius": 8.0,
                            "color": 0xFFF0F2F5,
                            "font_size": 16,
                        }

                        if show_start:
                            ui.Button("START GAME", height=44, style=btn_style, clicked_fn=self.start_game)
                        if show_settings:
                            ui.Button("SETTINGS", height=44, style=btn_style, clicked_fn=lambda: self.open_settings_menu(from_menu="MainMenu"))
                        if show_quit:
                            ui.Button("QUIT", height=44, style=btn_style, clicked_fn=self.quit_game)
                        ui.Spacer(height=16)

            self._main_menu_window = window
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to create Main Menu: {e}")

    def _destroy_main_menu_ui(self):
        if self._main_menu_window:
            try:
                self._main_menu_window.visible = False
            except Exception:
                pass
            self._main_menu_window = None

    def open_pause_menu(self, pause_timeline: bool = True):
        """Opens the Pause Menu, suspending player input, pausing timeline/PhysX, and hiding HUD overlays."""
        self._mock_pause_count += 1
        self._is_paused = True
        self._destroy_controls_ui()
        self._active_menu = "PauseMenu"
        self._set_player_input_enabled(False)
        self._show_pause_menu_ui()
        if pause_timeline and HAS_KIT:
            try:
                omni.timeline.get_timeline_interface().pause()
            except Exception:
                pass
        if carb:
            carb.log_info("[hydragon.editor.core] Pause Menu opened and timeline paused.")

    def resume_game(self, resume_timeline: bool = True):
        """Resumes gameplay from Pause Menu, restoring player input and unpausing timeline/PhysX."""
        self._mock_resume_count += 1
        self._destroy_pause_menu_ui()
        self._active_menu = None
        self._is_paused = False
        self._set_player_input_enabled(True)
        if resume_timeline and HAS_KIT:
            try:
                omni.timeline.get_timeline_interface().play()
            except Exception:
                pass
        if self._show_controls:
            self._show_controls_ui()
        if carb:
            carb.log_info("[hydragon.editor.core] Game resumed and timeline playing.")

    def toggle_pause_menu(self):
        if self._active_menu == "PauseMenu":
            self.resume_game()
        elif self._active_menu is None and self._is_simulating and self._game_started:
            if "PauseMenu" in self._canvases:
                self.open_pause_menu()

    def _show_pause_menu_ui(self):
        """Displays the centered Pause Menu overlay window."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_pause_menu_ui()
        canvas = self._canvases.get("PauseMenu")
        title_text = canvas.title if canvas and canvas.title else "GAME PAUSED"
        subtitle_text = canvas.subtitle if canvas and canvas.subtitle else "Press ESC or click Resume to continue"
        show_resume = canvas.show_resume if canvas else True
        show_restart = canvas.show_restart if canvas else True
        show_settings = canvas.show_settings if canvas else True
        show_quit = canvas.show_quit if canvas else True

        try:
            vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()
            win_w, win_h = 420, 360

            window = ui.Window(
                "HydragonPauseMenuWindow",
                width=win_w,
                height=win_h,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE,
            )
            if getattr(window, "docked", False):
                try:
                    window.undock()
                except Exception:
                    pass

            window.position_x = int(vp_x + (vp_w - win_w) * 0.5)
            window.position_y = int(vp_y + (vp_h - win_h) * 0.5)

            with window.frame:
                with ui.ZStack():
                    ui.Rectangle(
                        style={
                            "background_color": 0xF212151B,
                            "border_color": 0x66FFB800,
                            "border_width": 2.0,
                            "border_radius": 14.0,
                        }
                    )
                    with ui.VStack(spacing=10):
                        ui.Spacer(height=16)
                        ui.Label(
                            title_text,
                            alignment=ui.Alignment.CENTER,
                            style={
                                "color": 0xFFFFC820,
                                "font_size": 28,
                                "alignment": ui.Alignment.CENTER,
                            },
                        )
                        ui.Label(
                            subtitle_text,
                            alignment=ui.Alignment.CENTER,
                            style={
                                "color": 0xFF9EA7B4,
                                "font_size": 13,
                                "alignment": ui.Alignment.CENTER,
                            },
                        )
                        ui.Spacer(height=10)

                        btn_style = {
                            "background_color": 0xCC1F2430,
                            "border_color": 0x44FFB800,
                            "border_width": 1.0,
                            "border_radius": 8.0,
                            "color": 0xFFF0F2F5,
                            "font_size": 15,
                        }

                        if show_resume:
                            ui.Button("RESUME", height=40, style=btn_style, clicked_fn=self.resume_game)
                        if show_restart:
                            ui.Button("RESTART LEVEL", height=40, style=btn_style, clicked_fn=self.restart_level)
                        if show_settings:
                            ui.Button("SETTINGS", height=40, style=btn_style, clicked_fn=lambda: self.open_settings_menu(from_menu="PauseMenu"))
                        if show_quit:
                            ui.Button("QUIT TO MAIN MENU", height=40, style=btn_style, clicked_fn=self.quit_to_main_menu)
                        ui.Spacer(height=16)

            self._pause_menu_window = window
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to create Pause Menu: {e}")

    def _destroy_pause_menu_ui(self):
        if self._pause_menu_window:
            try:
                self._pause_menu_window.visible = False
            except Exception:
                pass
            self._pause_menu_window = None

    def open_settings_menu(self, from_menu: Optional[str] = None):
        """Opens Settings Menu from either Main Menu or Pause Menu."""
        self._mock_settings_count += 1
        if from_menu:
            self._previous_menu = from_menu
        elif self._active_menu and self._active_menu != "SettingsMenu":
            self._previous_menu = self._active_menu
        self._destroy_main_menu_ui()
        self._destroy_pause_menu_ui()
        self._active_menu = "SettingsMenu"
        self._show_settings_menu_ui()
        if carb:
            carb.log_info(f"[hydragon.editor.core] Settings Menu opened from {self._previous_menu}.")

    def back_from_settings(self):
        """Closes Settings Menu and returns to previous menu."""
        self._destroy_settings_menu_ui()
        if self._previous_menu == "PauseMenu":
            self.open_pause_menu()
        else:
            self._open_main_menu()

    def _show_settings_menu_ui(self):
        """Displays the centered Settings Menu modal window."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_settings_menu_ui()
        try:
            vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()
            win_w, win_h = 480, 420

            window = ui.Window(
                "HydragonSettingsMenuWindow",
                width=win_w,
                height=win_h,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE,
            )
            if getattr(window, "docked", False):
                try:
                    window.undock()
                except Exception:
                    pass

            window.position_x = int(vp_x + (vp_w - win_w) * 0.5)
            window.position_y = int(vp_y + (vp_h - win_h) * 0.5)

            with window.frame:
                with ui.ZStack():
                    ui.Rectangle(
                        style={
                            "background_color": 0xF512151B,
                            "border_color": 0x66FFB800,
                            "border_width": 2.0,
                            "border_radius": 14.0,
                        }
                    )
                    with ui.VStack(spacing=14):
                        ui.Spacer(height=16)
                        ui.Label(
                            "SETTINGS",
                            alignment=ui.Alignment.CENTER,
                            style={
                                "color": 0xFFFFC820,
                                "font_size": 28,
                                "alignment": ui.Alignment.CENTER,
                            },
                        )
                        ui.Spacer(height=6)

                        # Soundtrack Volume Slider
                        with ui.HStack(height=28):
                            ui.Label("Music Volume", width=130, style={"color": 0xFFE0E0E0, "font_size": 14})
                            music_slider = ui.FloatSlider(min=0.0, max=1.0, step=0.05)
                            try:
                                from .soundtrack_controller import HydragonSoundtrackSystem
                                st_sys = HydragonSoundtrackSystem.get_instance()
                                if st_sys:
                                    music_slider.model.set_value(st_sys.current_volume)
                            except Exception:
                                music_slider.model.set_value(0.8)

                            def on_music_changed(model):
                                try:
                                    from .soundtrack_controller import HydragonSoundtrackSystem
                                    st_sys = HydragonSoundtrackSystem.get_instance()
                                    if st_sys:
                                        st_sys.current_volume = model.get_value_as_float()
                                except Exception:
                                    pass
                            music_slider.model.add_value_changed_fn(on_music_changed)

                        # SFX Volume Slider
                        with ui.HStack(height=28):
                            ui.Label("SFX Volume", width=130, style={"color": 0xFFE0E0E0, "font_size": 14})
                            sfx_slider = ui.FloatSlider(min=0.0, max=1.0, step=0.05)
                            try:
                                from .effects_controller import HydragonEffectsSystem
                                fx_sys = HydragonEffectsSystem.get_instance()
                                if fx_sys:
                                    sfx_slider.model.set_value(fx_sys.audio_volume)
                            except Exception:
                                sfx_slider.model.set_value(0.5)

                            def on_sfx_changed(model):
                                try:
                                    from .effects_controller import HydragonEffectsSystem
                                    fx_sys = HydragonEffectsSystem.get_instance()
                                    if fx_sys:
                                        fx_sys.audio_volume = model.get_value_as_float()
                                except Exception:
                                    pass
                            sfx_slider.model.add_value_changed_fn(on_sfx_changed)

                        ui.Spacer(height=10)

                        # Back Button
                        ui.Button(
                            "BACK",
                            height=42,
                            style={
                                "background_color": 0xCC1F2430,
                                "border_color": 0x44FFB800,
                                "border_width": 1.0,
                                "border_radius": 8.0,
                                "color": 0xFFF0F2F5,
                                "font_size": 16,
                            },
                            clicked_fn=self.back_from_settings,
                        )
                        ui.Spacer(height=16)

            self._settings_menu_window = window
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to create Settings Menu: {e}")

    def _destroy_settings_menu_ui(self):
        if self._settings_menu_window:
            try:
                self._settings_menu_window.visible = False
            except Exception:
                pass
            self._settings_menu_window = None

    def _destroy_all_menus(self):
        self._destroy_main_menu_ui()
        self._destroy_pause_menu_ui()
        self._destroy_settings_menu_ui()
        self._active_menu = None

    def quit_to_main_menu(self):
        """Returns from gameplay / pause menu back to Main Menu."""
        self._destroy_pause_menu_ui()
        self._destroy_countdown_ui()
        self._destroy_controls_ui()
        self._destroy_all_popups()
        self._destroy_victory_ui()
        self._game_started = False
        if "MainMenu" in self._canvases:
            self._open_main_menu()
            if HAS_KIT:
                try:
                    omni.timeline.get_timeline_interface().pause()
                except Exception:
                    pass
        else:
            self.quit_game()

    def quit_game(self):
        """Cleanly stops simulation timeline."""
        self._mock_quit_count += 1
        self._destroy_all_menus()
        if HAS_KIT:
            try:
                omni.timeline.get_timeline_interface().stop()
            except Exception:
                pass
        if carb:
            carb.log_info("[hydragon.editor.core] Simulation stopped via Quit.")

    def restart_level(self):
        """Restarts level from pause menu, resetting score and player position."""
        self.resume_game()
        # Reset score via GameManager
        try:
            import omni.usd
            from .schemas import HydragonGameManager
            stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
            if stage:
                for prim in stage.Traverse():
                    if HydragonGameManager.is_applied(prim):
                        gm = HydragonGameManager(prim)
                        gm.score = 0
                        gm.foes_destroyed = 0
                        break
        except Exception:
            pass

        # Trigger player respawn
        try:
            from .player_controller import HydragonPlayerControllerSystem
            pcs = HydragonPlayerControllerSystem.get_instance()
            if pcs:
                pcs._needs_respawn = True
        except Exception:
            pass

        if self._show_countdown:
            self._start_countdown()

    def _set_player_input_enabled(self, enabled: bool):
        """Safely enables or disables player controller input via OpenUSD schema."""
        if not HAS_KIT:
            return
        try:
            import omni.usd
            from .schemas import HydragonPlayerController
            stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
            if stage:
                for prim in stage.Traverse():
                    if prim.IsValid() and prim.IsActive() and HydragonPlayerController.is_applied(prim):
                        ctl = HydragonPlayerController(prim)
                        ctl.input_enabled = enabled
        except Exception:
            pass


# Unified UI System Alias
HydragonUISystem = HydragonGameHUD

