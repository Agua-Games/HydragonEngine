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
        self._countdown_model = None

        # Controls HUD State
        self._controls_window = None

        # Floating Score Popups
        self._active_popups: List[dict] = []

        # Victory Modal
        self._victory_window = None

    @classmethod
    def get_instance(cls) -> Optional["HydragonGameHUD"]:
        return cls._instance

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
            self._countdown_model = ui.SimpleStringModel("3...")
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
            with window.frame:
                with ui.VStack(alignment=ui.Alignment.CENTER):
                    ui.Spacer(height=20)
                    ui.Label(
                        model=self._countdown_model,
                        style={
                            "color": 0xFF20E0FF,  # Bright Gold / Amber
                            "font_size": 46,
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

        if not self._countdown_model:
            return

        if t < 1.0:
            self._countdown_model.as_string = "3..."
        elif t < 2.0:
            self._countdown_model.as_string = "2..."
        elif t < 3.0:
            self._countdown_model.as_string = "1..."
        elif t < 4.2:
            self._countdown_model.as_string = "MARBLE GAME START!"
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
        self._countdown_model = None

    # -------------------------------------------------------------------------
    # 2. Controls / Hotkeys Card (Non-intrusive Semi-Transparent HUD)
    # -------------------------------------------------------------------------
    def _show_controls_ui(self):
        """Displays non-intrusive hotkeys panel in viewport."""
        if not HAS_KIT or ui is None:
            return

        self._destroy_controls_ui()

        try:
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
            # Create transient floating score overlay badge
            badge_window = ui.Window(
                f"HydragonScorePopup_{len(self._active_popups)}",
                width=160,
                height=60,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE
                | ui.WINDOW_FLAGS_NO_BACKGROUND,
            )
            with badge_window.frame:
                with ui.VStack(alignment=ui.Alignment.CENTER):
                    ui.Label(
                        f"+{points}",
                        style={
                            "color": 0xFF00E6FF,  # Glowing gold
                            "font_size": 32,
                            "alignment": ui.Alignment.CENTER,
                        },
                    )

            self._active_popups.append({
                "window": badge_window,
                "elapsed": 0.0,
                "duration": 1.4,
                "initial_y": badge_window.position_y if hasattr(badge_window, "position_y") else 0,
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
                # Float upward
                if window and hasattr(window, "position_y"):
                    try:
                        window.position_y = popup["initial_y"] - int(progress * 40.0)
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
            window = ui.Window(
                "HydragonVictoryModal",
                width=460,
                height=200,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE,
            )
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
