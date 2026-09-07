"""
Hydragon Engine - Game HUD & In-Game Visual Overlays

Manages non-intrusive on-screen visual feedback for gameplay:
1. Start Countdown Banner ("3... 2... 1... MARBLE GAME START!") with transparent background.
2. In-Game Controls / Hotkeys Card (WASD, Jump, Camera Orbit).
3. Floating Score Popups ("+100" / "+50") upon foe sphere destruction.
4. Polished Victory Modal with final score, time, and restart prompt.
"""

from typing import List, Optional, Tuple

try:
    import carb
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

        # Countdown State
        self._countdown_elapsed: float = 0.0
        self._countdown_active: bool = False
        self._countdown_window = None
        self._countdown_label = None

        # Controls HUD State
        self._controls_window = None

        # Floating Score Popups
        self._active_popups: List[dict] = []
        self._popup_counter: int = 0

        # Victory Modal
        self._victory_window = None

    @classmethod
    def get_instance(cls) -> Optional["HydragonGameHUD"]:
        return cls._instance

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

    def startup(self):
        """Initializes HUD subscriptions for timeline and per-frame app updates."""
        if not HAS_KIT:
            return
        self._is_active = True
        self._subscribe_timeline()
        self._subscribe_app_update()
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonGameHUD initialized.")

    def shutdown(self):
        """Releases all subscriptions and cleans up all active HUD windows."""
        self._is_active = False
        self._is_simulating = False
        self._timeline_sub = None
        self._app_update_sub = None

        self._destroy_countdown_ui()
        self._destroy_controls_ui()
        self._destroy_all_popups()
        self._destroy_victory_ui()

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
    # Timeline & Frame Update Callbacks
    # -------------------------------------------------------------------------
    def _on_timeline_event(self, e):
        if not HAS_KIT:
            return
        try:
            event_type = int(e.type)
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                self._is_simulating = True
                self._destroy_victory_ui()
                self._destroy_all_popups()
                self._start_countdown()
                self._show_controls_ui()
            elif event_type in (int(omni.timeline.TimelineEventType.STOP), int(omni.timeline.TimelineEventType.PAUSE)):
                self._is_simulating = False
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

        dt = e.payload.get("dt", 1.0 / 60.0) if hasattr(e, "payload") else 1.0 / 60.0

        # Maintain visibility and keep anchored relative to active viewport bounds (e.g. F7/F11 fullscreen)
        vp_x, vp_y, vp_w, vp_h = self._get_viewport_bounds()

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

        # 1. Update Countdown Banner
        if self._countdown_active:
            self._update_countdown(dt)

        # 2. Update Floating Score Popups
        if self._active_popups:
            self._update_popups(dt)

    # -------------------------------------------------------------------------
    # 1. Start Countdown Banner ("3... 2... 1... MARBLE GAME START!")
    # -------------------------------------------------------------------------
    def _start_countdown(self):
        """Spawns the transparent, centered countdown banner on simulation start."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_countdown_ui()
        self._countdown_elapsed = 0.0
        self._countdown_active = True

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
            if hasattr(ui, "DockPreference"):
                window.dock_preference = ui.DockPreference.DISABLED

            window.position_x = int(vp_x + (vp_w - 650) * 0.5)
            window.position_y = int(vp_y + vp_h * 0.25)

            with window.frame:
                with ui.VStack(alignment=ui.Alignment.CENTER):
                    ui.Spacer(height=20)
                    self._countdown_label = ui.Label(
                        "3...",
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
        """Displays non-intrusive hotkeys panel in viewport."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_controls_ui()

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
            if hasattr(ui, "DockPreference"):
                window.dock_preference = ui.DockPreference.DISABLED

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
                with ui.VStack(alignment=ui.Alignment.LEFT, spacing=4):
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
        Also triggers Kit notification feedback.
        """
        if not HAS_KIT:
            return

        try:
            # Post lightweight feedback notification in Kit Notification Manager
            import omni.kit.notification_manager as nm
            if nm and hasattr(nm, "post_notification"):
                nm.post_notification(
                    f"💥 Foe Defeated! +{points} pts",
                    duration=3,
                    status=nm.NotificationStatus.INFO,
                )
        except Exception:
            pass

        if ui is None:
            return

        try:
            self._popup_counter += 1
            screen_x, screen_y = self._world_to_screen(world_pos)
            badge_window = ui.Window(
                f"HydragonScorePopup_{self._popup_counter}",
                width=160,
                height=60,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE
                | ui.WINDOW_FLAGS_NO_BACKGROUND,
            )
            if hasattr(ui, "DockPreference"):
                badge_window.dock_preference = ui.DockPreference.DISABLED

            badge_window.position_x = screen_x
            badge_window.position_y = screen_y

            with badge_window.frame:
                with ui.VStack(alignment=ui.Alignment.CENTER):
                    ui.Label(
                        f"+{points}",
                        style={
                            "color": 0xFF00E6FF,  # Glowing gold
                            "font_size": 34,
                            "alignment": ui.Alignment.CENTER,
                        },
                    )

            self._active_popups.append({
                "window": badge_window,
                "elapsed": 0.0,
                "duration": 1.4,
                "initial_y": screen_y,
            })
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to spawn score popup: {e}")

    def _update_popups(self, dt: float):
        remaining = []
        for popup in self._active_popups:
            popup["elapsed"] += dt
            progress = popup["elapsed"] / popup["duration"]
            window = popup.get("window")

            if progress < 1.0:
                if window:
                    if not window.visible:
                        window.visible = True
                    if hasattr(window, "position_y"):
                        try:
                            window.position_y = popup["initial_y"] - int(progress * 50.0)
                        except Exception:
                            pass
                remaining.append(popup)
            else:
                if window:
                    try:
                        window.visible = False
                    except Exception:
                        pass

        self._active_popups = remaining

    def _destroy_all_popups(self):
        for popup in self._active_popups:
            w = popup.get("window")
            if w:
                try:
                    w.visible = False
                except Exception:
                    pass
        self._active_popups.clear()

    # -------------------------------------------------------------------------
    # 4. Polished Victory Modal ("Mission Accomplished!")
    # -------------------------------------------------------------------------
    def show_victory(self, score: int, elapsed: float, foes: int):
        """Displays a polished, high-contrast victory screen with final statistics."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_victory_ui()

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
            if hasattr(ui, "DockPreference"):
                window.dock_preference = ui.DockPreference.DISABLED

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
                with ui.VStack(alignment=ui.Alignment.CENTER, spacing=8):
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
        if self._victory_window:
            try:
                self._victory_window.visible = False
            except Exception:
                pass
            self._victory_window = None
