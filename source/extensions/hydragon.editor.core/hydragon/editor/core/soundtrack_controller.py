"""
Hydragon Engine - Background Soundtrack & Music Controller System

Component-driven background soundtrack system using OpenUSD API schemas
(HydragonSoundtrackAPI), supporting dynamic track switching (ambient, combat, victory),
smooth volume fading/crossfading, continuous looping, and timeline lifecycle integration.
"""

import os
from typing import Any, Dict, List, Optional, Tuple

try:
    import carb
    import omni.kit.app
    import omni.timeline
    import omni.usd
    from pxr import Usd, Sdf
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    Usd = None
    Sdf = None

from .schemas import HydragonSoundtrack, DEFAULT_AMBIENT_SOUNDTRACK


# Standard candidate stage prim paths for soundtrack manager
SOUNDTRACK_PRIM_CANDIDATES: Tuple[str, ...] = (
    "/hydragon_soundtrack_root/soundtrack_manager",
    "/World/SoundtrackManager",
    "/World/soundtrack_manager",
    "/World/Soundtrack",
    "/SoundtrackManager",
    "/soundtrack_manager",
    "/Soundtrack",
)

# Tracks that represent one-shot stingers/fanfares and must never continuously loop
ONE_SHOT_TRACKS = {"victory", "achievement", "win", "game_over", "death"}


class HydragonSoundtrackEntity:
    """
    Component wrapper for an OpenUSD prim with HydragonSoundtrackAPI applied.
    Manages track metadata, playback configuration, and asset paths.
    """

    def __init__(self, prim):
        self._prim = prim
        self._schema = HydragonSoundtrack(prim)

    @property
    def prim(self):
        return self._prim

    @property
    def schema(self) -> HydragonSoundtrack:
        return self._schema

    @property
    def current_track(self) -> str:
        return self._schema.current_track

    @current_track.setter
    def current_track(self, val: str):
        self._schema.current_track = val

    @property
    def track_state(self) -> str:
        return self._schema.track_state

    @track_state.setter
    def track_state(self, val: str):
        self._schema.track_state = val

    @property
    def volume(self) -> float:
        return self._schema.volume

    @volume.setter
    def volume(self, val: float):
        self._schema.volume = val

    @property
    def auto_play(self) -> bool:
        return self._schema.auto_play

    @auto_play.setter
    def auto_play(self, val: bool):
        self._schema.auto_play = val

    @property
    def is_looping(self) -> bool:
        return self._schema.is_looping

    @is_looping.setter
    def is_looping(self, val: bool):
        self._schema.is_looping = val

    @property
    def fade_duration(self) -> float:
        return self._schema.fade_duration

    @fade_duration.setter
    def fade_duration(self, val: float):
        self._schema.fade_duration = val

    @property
    def ambient_asset_path(self) -> str:
        return self._schema.ambient_asset_path

    @ambient_asset_path.setter
    def ambient_asset_path(self, val: str):
        self._schema.ambient_asset_path = val

    @property
    def combat_asset_path(self) -> str:
        return self._schema.combat_asset_path

    @combat_asset_path.setter
    def combat_asset_path(self, val: str):
        self._schema.combat_asset_path = val

    @property
    def victory_asset_path(self) -> str:
        return self._schema.victory_asset_path

    @victory_asset_path.setter
    def victory_asset_path(self, val: str):
        self._schema.victory_asset_path = val

    def get_track_asset_path(self, track_name: str) -> str:
        """
        Retrieves the asset path configured for the specified track name.
        Defaults to ambient track if track name is empty or unknown.
        """
        track_key = (track_name or "").strip().lower()
        if track_key in ("ambient", "default", "intro"):
            path = self.ambient_asset_path
            return path if path else DEFAULT_AMBIENT_SOUNDTRACK
        elif track_key == "combat":
            return self.combat_asset_path
        elif track_key in ("victory", "achievement", "win"):
            return self.victory_asset_path
        return self.ambient_asset_path or DEFAULT_AMBIENT_SOUNDTRACK

    def is_valid(self) -> bool:
        return bool(self._prim and hasattr(self._prim, "IsValid") and self._prim.IsValid())


class HydragonSoundtrackSystem:
    """
    Singleton system that manages background music playback, track switching,
    looping, volume fading, and timeline lifecycle integration.
    """

    _instance: Optional["HydragonSoundtrackSystem"] = None

    DEFAULT_VOLUME: float = 0.8
    DEFAULT_FADE_DURATION: float = 1.5

    def __init__(self):
        HydragonSoundtrackSystem._instance = self
        self._is_active: bool = False
        self._entities: Dict[str, HydragonSoundtrackEntity] = {}
        self._active_entity: Optional[HydragonSoundtrackEntity] = None

        # Playback state
        self._current_track_name: str = "ambient"
        self._is_playing: bool = False
        self._is_paused: bool = False
        self._current_sound: Any = None
        self._cached_sounds: Dict[str, Any] = {}

        # Fading state
        self._target_volume: float = self.DEFAULT_VOLUME
        self._current_volume: float = self.DEFAULT_VOLUME
        self._fade_duration: float = self.DEFAULT_FADE_DURATION
        self._fade_direction: int = 0  # +1: in, -1: out, 0: idle
        self._fade_rate: float = 0.0
        self._pending_track_change: Optional[str] = None

        # Subscriptions
        self._timeline_sub = None
        self._app_update_sub = None

        # Headless / Mock counters for testing
        self._mock_play_count: int = 0
        self._mock_stop_count: int = 0
        self._mock_loop_restart_count: int = 0
        self._mock_sound_playing: bool = False

    @classmethod
    def get_instance(cls) -> Optional["HydragonSoundtrackSystem"]:
        return cls._instance

    # -------------------------------------------------------------------------
    # Lifecycle
    # -------------------------------------------------------------------------
    def startup(self):
        """Initializes subscriptions for timeline events and app update loop."""
        self._is_active = True
        self._subscribe_timeline()
        self._subscribe_app_update()
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonSoundtrackSystem initialized.")

    def shutdown(self):
        """Releases subscriptions, stops active soundtrack, and clears cache."""
        self.stop_track(fade=False)
        self._is_active = False
        self._timeline_sub = None
        self._app_update_sub = None
        self._entities.clear()
        self._active_entity = None
        self._cached_sounds.clear()
        self._current_sound = None
        HydragonSoundtrackSystem._instance = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonSoundtrackSystem shutdown.")

    # -------------------------------------------------------------------------
    # Subscriptions
    # -------------------------------------------------------------------------
    def _subscribe_timeline(self):
        if not HAS_KIT:
            return
        try:
            stream = omni.timeline.get_timeline_interface().get_timeline_event_stream()
            if stream:
                self._timeline_sub = stream.create_subscription_to_pop(
                    self._on_timeline_event, name="HydragonSoundtrackTimelineSub"
                )
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to subscribe to timeline for soundtrack: {ex}")

    def _subscribe_app_update(self):
        if not HAS_KIT:
            return
        try:
            app = omni.kit.app.get_app()
            if app:
                self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                    self._on_app_update, name="HydragonSoundtrackAppUpdateSub"
                )
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to subscribe to app update for soundtrack: {ex}")

    def _on_timeline_event(self, e):
        """Handles timeline play, stop, and pause events."""
        if not self._is_active:
            return
        try:
            event_type = int(e.type)
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                if stage:
                    self._discover_entities(stage)
                self._handle_timeline_play()
            elif event_type == int(omni.timeline.TimelineEventType.STOP):
                self._handle_timeline_stop()
            elif event_type == int(omni.timeline.TimelineEventType.PAUSE):
                self._handle_timeline_pause()
        except Exception as ex:
            if carb:
                carb.log_error(f"[hydragon.editor.core] Error in soundtrack timeline event: {ex}")

    def _on_app_update(self, e):
        """Per-frame update callback for volume fading and looping checks."""
        if not self._is_active:
            return
        dt = 1.0 / 60.0
        if hasattr(e, "payload") and isinstance(e.payload, dict):
            dt = float(e.payload.get("dt", 1.0 / 60.0))
        self.step_soundtrack(dt)

    # -------------------------------------------------------------------------
    # Stage Discovery
    # -------------------------------------------------------------------------
    def _discover_entities(self, stage):
        """Discovers soundtrack entities authored with HydragonSoundtrackAPI."""
        self._entities.clear()
        self._active_entity = None
        if not stage:
            return

        # 1. Check well-known candidate paths first (fastest O(1) checks)
        for candidate_path in SOUNDTRACK_PRIM_CANDIDATES:
            prim = stage.GetPrimAtPath(candidate_path)
            if prim and prim.IsValid() and HydragonSoundtrack.is_applied(prim):
                entity = HydragonSoundtrackEntity(prim)
                self._entities[candidate_path] = entity
                if self._active_entity is None:
                    self._active_entity = entity

        # 2. Check /World children if candidate paths didn't match
        if not self._active_entity:
            world = stage.GetPrimAtPath("/World")
            if world and world.IsValid():
                for child in world.GetChildren():
                    if HydragonSoundtrack.is_applied(child):
                        entity = HydragonSoundtrackEntity(child)
                        self._entities[str(child.GetPath())] = entity
                        if self._active_entity is None:
                            self._active_entity = entity

        # 3. Check root prims as fallback
        if not self._active_entity:
            for prim in stage.GetPseudoRoot().GetChildren():
                if HydragonSoundtrack.is_applied(prim):
                    entity = HydragonSoundtrackEntity(prim)
                    self._entities[str(prim.GetPath())] = entity
                    if self._active_entity is None:
                        self._active_entity = entity

        if carb and self._active_entity:
            carb.log_info(f"[hydragon.editor.core] Discovered soundtrack entity at: {self._active_entity.prim.GetPath()}")

    # -------------------------------------------------------------------------
    # Timeline Handlers
    # -------------------------------------------------------------------------
    def _handle_timeline_play(self):
        """Starts soundtrack playback when simulation begins, respecting auto_play setting."""
        if self._is_paused:
            self.resume_track()
            return

        if not self._active_entity or not self._active_entity.is_valid():
            # Strict Opt-In Architecture: Do not play audio unless a HydragonSoundtrackAPI prim is authored.
            return

        if self._active_entity.auto_play:
            target_track = self._active_entity.current_track or "ambient"
            # Sanitize track: simulation start must always begin on ambient, never on victory/stingers
            if target_track.lower() in ONE_SHOT_TRACKS:
                target_track = "ambient"
                self._active_entity.current_track = "ambient"
            fade_duration = self._active_entity.fade_duration
            vol = self._active_entity.volume
            self.play_track(target_track, fade=True, fade_duration=fade_duration, target_volume=vol)

    def _handle_timeline_stop(self):
        """Stops soundtrack playback on simulation end and resets track to ambient."""
        self.stop_track(fade=False)
        self._current_track_name = "ambient"
        if self._active_entity and self._active_entity.is_valid():
            try:
                self._active_entity.current_track = "ambient"
                self._active_entity.track_state = "Stopped"
            except Exception:
                pass

    def _handle_timeline_pause(self):
        """Pauses soundtrack playback on simulation pause."""
        try:
            from .game_hud import HydragonGameHUD
            hud = HydragonGameHUD.get_instance()
            if hud and hud.is_menu_active:
                return
        except Exception:
            pass
        self.pause_track()

    # -------------------------------------------------------------------------
    # File Resolution
    # -------------------------------------------------------------------------
    def _resolve_sound_file(self, filename_or_path: str) -> str:
        """Resolves audio asset paths dynamically relative to extension root or working directory."""
        if not filename_or_path:
            return ""
        clean_raw = str(filename_or_path).strip("@").replace("\\", "/")
        clean_name = os.path.basename(clean_raw)
        candidates = []

        # If already an existing absolute path
        if os.path.isabs(clean_raw) and os.path.exists(clean_raw):
            return os.path.abspath(clean_raw).replace("\\", "/")

        # 1. Preferred: Via Kit Extension Manager if available
        if HAS_KIT:
            try:
                em = omni.kit.app.get_app().get_extension_manager()
                ext_id_path = em.get_extension_path_by_pkg_id("hydragon.editor.core")
                if ext_id_path:
                    candidates.append(os.path.join(ext_id_path, clean_raw))
                    candidates.append(os.path.join(ext_id_path, "data", "assets", "audio", "sound_fx_samples", clean_name))
            except Exception:
                pass

        # 2. Relative to extension root via __file__ (development fallback)
        ext_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
        candidates.append(os.path.join(ext_root, clean_raw))
        candidates.append(os.path.join(ext_root, "data", "assets", "audio", "sound_fx_samples", clean_name))

        # Via Current Working Directory fallback
        candidates.append(os.path.abspath(os.path.join(os.getcwd(), clean_raw)))
        candidates.append(os.path.abspath(os.path.join(os.getcwd(), "source", "extensions", "hydragon.editor.core", "data", "assets", "audio", "sound_fx_samples", clean_name)))

        for c in candidates:
            if os.path.exists(c):
                return os.path.abspath(c).replace("\\", "/")

        return ""

    # -------------------------------------------------------------------------
    # Audio Interface & Sound Caching
    # -------------------------------------------------------------------------
    def _get_ui_audio(self):
        """Retrieves omni.kit.uiaudio interface safely."""
        if not HAS_KIT:
            return None
        try:
            import omni.kit.uiaudio
            return omni.kit.uiaudio.get_ui_audio_interface()
        except Exception:
            return None

    def _get_or_load_sound(self, resolved_path: str):
        """Retrieves or loads a UiSound instance into cache."""
        if not resolved_path:
            return None
        if resolved_path in self._cached_sounds:
            return self._cached_sounds[resolved_path]

        audio = self._get_ui_audio()
        if not audio:
            return None

        try:
            snd = audio.create_sound(resolved_path)
            if snd:
                self._cached_sounds[resolved_path] = snd
                if carb:
                    carb.log_info(f"[hydragon.editor.core] Loaded soundtrack asset: {resolved_path}")
                return snd
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to load soundtrack sound '{resolved_path}': {ex}")
        return None

    # -------------------------------------------------------------------------
    # Volume Control
    # -------------------------------------------------------------------------
    def _apply_volume(self, volume: float):
        """Sets playback volume level (0.0 to 1.0) in carb.settings."""
        target_vol = max(0.0, min(1.0, float(volume)))
        self._current_volume = target_vol
        if carb and hasattr(carb, "settings"):
            try:
                settings = carb.settings.get_settings()
                if settings:
                    for key in ("/persistent/audio/context/uiVolume", "/persistent/audio/context/masterVolume"):
                        try:
                            current = settings.get_as_float(key) if hasattr(settings, "get_as_float") else None
                            if current is None or abs(current - self._current_volume) > 0.005 or self._current_volume in (0.0, 1.0):
                                settings.set_float(key, self._current_volume)
                        except AttributeError:
                            settings.set(key, self._current_volume)
            except Exception:
                pass

    def set_volume(self, volume: float):
        """Manually sets soundtrack target and current volume."""
        self._target_volume = max(0.0, min(1.0, float(volume)))
        self._apply_volume(self._target_volume)
        if self._active_entity and self._active_entity.is_valid():
            self._active_entity.volume = self._target_volume

    # -------------------------------------------------------------------------
    # Playback Control
    # -------------------------------------------------------------------------
    def play_track(
        self,
        track_name: str,
        fade: bool = True,
        fade_duration: Optional[float] = None,
        target_volume: Optional[float] = None,
    ):
        """
        Plays or switches to the specified track (ambient, combat, victory).
        Supports smooth fading and crossfading between tracks.
        """
        track_key = (track_name or "ambient").strip().lower()

        if target_volume is not None:
            self._target_volume = max(0.0, min(1.0, float(target_volume)))
        elif self._active_entity and self._active_entity.is_valid():
            self._target_volume = self._active_entity.volume
        else:
            self._target_volume = self.DEFAULT_VOLUME

        duration = fade_duration if fade_duration is not None else (
            self._active_entity.fade_duration if (self._active_entity and self._active_entity.is_valid()) else self.DEFAULT_FADE_DURATION
        )
        self._fade_duration = max(0.01, float(duration))

        # If already playing this track
        if self._is_playing and self._current_track_name == track_key and not self._is_paused:
            if self._fade_direction < 0:
                # Reverse fade out into fade in
                self._fade_direction = 1
                self._fade_rate = (self._target_volume - self._current_volume) / self._fade_duration if self._fade_duration > 0 else 1.0
            return

        # If playing another track and fade is requested: fade out first, then switch
        if self._is_playing and fade and self._current_volume > 0.05:
            self._pending_track_change = track_key
            self._fade_direction = -1
            self._fade_rate = self._current_volume / self._fade_duration if self._fade_duration > 0 else 1.0
            return

        # Immediate start
        self._start_track_playback(track_key, fade=fade)

    def _start_track_playback(self, track_key: str, fade: bool = True):
        """Internal worker to start playing a sound asset for track_key."""
        self._pending_track_change = None
        self._current_track_name = track_key

        # Resolve asset path
        asset_path = ""
        if self._active_entity and self._active_entity.is_valid():
            asset_path = self._active_entity.get_track_asset_path(track_key)
            self._active_entity.current_track = track_key
            self._active_entity.track_state = "Playing"
        else:
            asset_path = DEFAULT_AMBIENT_SOUNDTRACK

        resolved = self._resolve_sound_file(asset_path)
        if not resolved:
            resolved = self._resolve_sound_file(DEFAULT_AMBIENT_SOUNDTRACK)

        audio = self._get_ui_audio()
        if audio and resolved:
            sound = self._get_or_load_sound(resolved)
            if sound:
                if self._current_sound and self._current_sound != sound:
                    try:
                        audio.stop_sound(self._current_sound)
                    except Exception:
                        pass

                self._current_sound = sound
                try:
                    audio.play_sound(sound)
                    self._is_playing = True
                    self._is_paused = False
                except Exception as ex:
                    if carb:
                        carb.log_warn(f"[hydragon.editor.core] Failed to play soundtrack '{resolved}': {ex}")
        else:
            # Mock / Headless mode
            self._mock_play_count += 1
            self._mock_sound_playing = True
            self._is_playing = True
            self._is_paused = False

        if fade:
            self._current_volume = 0.0
            self._apply_volume(0.0)
            self._fade_direction = 1
            self._fade_rate = self._target_volume / self._fade_duration if self._fade_duration > 0 else 1.0
        else:
            self._current_volume = self._target_volume
            self._apply_volume(self._target_volume)
            self._fade_direction = 0

        if carb:
            carb.log_info(f"[hydragon.editor.core] Soundtrack playing track '{track_key}' (asset: {resolved}).")

    def stop_track(self, fade: bool = True):
        """Stops playback of the currently active soundtrack."""
        if not self._is_playing and not self._mock_sound_playing:
            return

        if fade and self._current_volume > 0.05:
            self._fade_direction = -1
            self._pending_track_change = "__STOP__"
            self._fade_rate = self._current_volume / self._fade_duration if self._fade_duration > 0 else 1.0
            return

        self._finalize_stop()

    def _finalize_stop(self):
        """Halts the audio engine playback immediately and resets flags."""
        audio = self._get_ui_audio()
        if audio and self._current_sound:
            try:
                audio.stop_sound(self._current_sound)
            except Exception:
                pass

        self._is_playing = False
        self._is_paused = False
        self._fade_direction = 0
        self._pending_track_change = None
        self._mock_sound_playing = False
        self._mock_stop_count += 1

        if self._active_entity and self._active_entity.is_valid():
            self._active_entity.track_state = "Stopped"

        if carb:
            carb.log_info("[hydragon.editor.core] Soundtrack stopped.")

    def pause_track(self):
        """Pauses active soundtrack."""
        if not self._is_playing or self._is_paused:
            return
        audio = self._get_ui_audio()
        if audio and self._current_sound:
            try:
                audio.stop_sound(self._current_sound)
            except Exception:
                pass
        self._is_paused = True
        if self._active_entity and self._active_entity.is_valid():
            self._active_entity.track_state = "Paused"

    def resume_track(self):
        """Resumes paused soundtrack."""
        if not self._is_paused:
            return
        audio = self._get_ui_audio()
        if audio and self._current_sound:
            try:
                audio.play_sound(self._current_sound)
            except Exception:
                pass
        self._is_paused = False
        self._is_playing = True
        if self._active_entity and self._active_entity.is_valid():
            self._active_entity.track_state = "Playing"

    # -------------------------------------------------------------------------
    # Frame Stepping (Fading & Looping)
    # -------------------------------------------------------------------------
    def step_soundtrack(self, dt: float):
        """
        Updates smooth volume fading and checks for loop restarting every frame.
        """
        if not self._is_active:
            return

        # 1. Process volume fading
        if self._fade_direction != 0:
            step_vol = self._fade_rate * dt
            if self._fade_direction > 0:
                # Fade in
                self._current_volume = min(self._target_volume, self._current_volume + step_vol)
                self._apply_volume(self._current_volume)
                if self._current_volume >= self._target_volume:
                    self._fade_direction = 0
            elif self._fade_direction < 0:
                # Fade out
                self._current_volume = max(0.0, self._current_volume - step_vol)
                self._apply_volume(self._current_volume)
                if self._current_volume <= 0.001:
                    self._fade_direction = 0
                    if self._pending_track_change == "__STOP__":
                        self._finalize_stop()
                    elif self._pending_track_change:
                        next_track = self._pending_track_change
                        self._pending_track_change = None
                        self._start_track_playback(next_track, fade=True)

        # 2. Check continuous looping
        is_looping = True
        if self._active_entity and self._active_entity.is_valid():
            is_looping = self._active_entity.is_looping

        # One-shot stingers and fanfares must NEVER loop indefinitely
        if self._current_track_name and self._current_track_name.lower() in ONE_SHOT_TRACKS:
            is_looping = False

        if self._is_playing and not self._is_paused and is_looping:
            audio = self._get_ui_audio()
            if audio and self._current_sound:
                try:
                    if not audio.is_sound_playing(self._current_sound):
                        audio.play_sound(self._current_sound)
                        self._mock_loop_restart_count += 1
                except Exception:
                    pass
            elif not audio:
                if not self._mock_sound_playing:
                    self._mock_loop_restart_count += 1
        elif self._is_playing and not self._is_paused and not is_looping:
            audio = self._get_ui_audio()
            if audio and self._current_sound:
                try:
                    if not audio.is_sound_playing(self._current_sound):
                        self._is_playing = False
                        if self._active_entity and self._active_entity.is_valid():
                            self._active_entity.track_state = "Stopped"
                except Exception:
                    pass
            elif not audio:
                if not self._mock_sound_playing:
                    self._is_playing = False
                    if self._active_entity and self._active_entity.is_valid():
                        self._active_entity.track_state = "Stopped"

    # -------------------------------------------------------------------------
    # State Inspection
    # -------------------------------------------------------------------------
    @property
    def is_playing(self) -> bool:
        return self._is_playing or self._mock_sound_playing

    @property
    def is_paused(self) -> bool:
        return self._is_paused

    @property
    def current_track_name(self) -> str:
        return self._current_track_name

    @property
    def current_volume(self) -> float:
        return self._current_volume

    @property
    def target_volume(self) -> float:
        return self._target_volume

    @property
    def entities(self) -> Dict[str, HydragonSoundtrackEntity]:
        return self._entities

    @property
    def active_entity(self) -> Optional[HydragonSoundtrackEntity]:
        return self._active_entity
