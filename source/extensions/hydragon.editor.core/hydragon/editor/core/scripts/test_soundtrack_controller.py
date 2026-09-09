"""
Unit test for HydragonSoundtrackSystem and HydragonSoundtrackEntity
Validates lifecycle, track switching (ambient, combat, victory), smooth volume fading,
crossfading between tracks, dynamic audio file resolution, and fail-silent execution outside Kit.
"""

import os
import sys
import wave

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.schemas import (
    HydragonSoundtrack,
    DEFAULT_AMBIENT_SOUNDTRACK,
)
from hydragon.editor.core.soundtrack_controller import (
    HydragonSoundtrackSystem,
    HydragonSoundtrackEntity,
)


class MockPrim:
    """Mock OpenUSD Prim for testing schema attributes outside Omniverse Kit."""

    def __init__(self, path: str = "/World/soundtrack_manager"):
        self._path = path
        self._attrs = {}

    def IsValid(self) -> bool:
        return True

    def GetPath(self):
        return self._path

    def HasAttribute(self, name: str) -> bool:
        return name in self._attrs

    def GetAttribute(self, name: str):
        if name not in self._attrs:
            return None
        class Attr:
            def __init__(self, outer, n):
                self._outer = outer
                self._n = n
            def IsValid(self):
                return True
            def Get(self):
                return self._outer._attrs.get(self._n)
            def Set(self, new_v):
                self._outer._attrs[self._n] = new_v
        return Attr(self, name)

    def CreateAttribute(self, name: str, type_name):
        class Attr:
            def __init__(self, outer, n):
                self._outer = outer
                self._n = n
            def IsValid(self):
                return True
            def Set(self, v):
                self._outer._attrs[self._n] = v
            def Get(self):
                return self._outer._attrs.get(self._n)
        return Attr(self, name)


def test_soundtrack_schema_and_entity():
    print("--- 1. Testing HydragonSoundtrack Schema & Entity ---")
    mock_prim = MockPrim()
    entity = HydragonSoundtrackEntity(mock_prim)
    assert entity.is_valid(), "Entity should be valid with MockPrim"

    # Default fail-silent values
    assert entity.current_track == "ambient"
    assert entity.track_state == "Playing"
    assert entity.volume == 0.8
    assert entity.auto_play is True
    assert entity.is_looping is True
    assert entity.fade_duration == 1.5
    assert "lounge_soundtrack_01.wav" in entity.ambient_asset_path
    assert entity.combat_asset_path == ""
    assert entity.victory_asset_path == ""

    # Test property mutations
    entity.current_track = "combat"
    assert entity.current_track == "combat"
    entity.track_state = "Paused"
    assert entity.track_state == "Paused"
    entity.volume = 0.5
    assert entity.volume == 0.5
    entity.auto_play = False
    assert entity.auto_play is False
    entity.is_looping = False
    assert entity.is_looping is False
    entity.fade_duration = 2.0
    assert entity.fade_duration == 2.0
    entity.combat_asset_path = "combat_01.wav"
    assert entity.combat_asset_path == "combat_01.wav"
    entity.victory_asset_path = "victory_01.wav"
    assert entity.victory_asset_path == "victory_01.wav"

    # Test get_track_asset_path
    assert entity.get_track_asset_path("ambient") == entity.ambient_asset_path
    assert entity.get_track_asset_path("combat") == "combat_01.wav"
    assert entity.get_track_asset_path("victory") == "victory_01.wav"
    assert entity.get_track_asset_path("unknown") == entity.ambient_asset_path
    print("  [PASS] HydragonSoundtrack Schema & Entity verified")


def test_soundtrack_system_lifecycle():
    print("--- 2. Testing Soundtrack System Lifecycle ---")
    system = HydragonSoundtrackSystem()
    assert system is not None
    assert HydragonSoundtrackSystem.get_instance() is system

    system.startup()
    assert system._is_active is True

    system.shutdown()
    assert system._is_active is False
    assert HydragonSoundtrackSystem.get_instance() is None
    print("  [PASS] Soundtrack System lifecycle verified")


def test_soundtrack_playback_and_states():
    print("--- 3. Testing Soundtrack Playback & State Transitions ---")
    system = HydragonSoundtrackSystem()
    system.startup()

    # Play immediate without fade
    system.play_track("ambient", fade=False, target_volume=0.8)
    assert system.is_playing is True
    assert system.is_paused is False
    assert system.current_track_name == "ambient"
    assert system.current_volume == 0.8
    assert system._mock_play_count == 1

    # Pause and resume
    system.pause_track()
    assert system.is_paused is True

    system.resume_track()
    assert system.is_paused is False
    assert system.is_playing is True

    # Immediate stop
    system.stop_track(fade=False)
    assert system.is_playing is False
    assert system._mock_stop_count == 1

    system.shutdown()
    print("  [PASS] Soundtrack playback and states verified")


def test_soundtrack_fading_in_and_out():
    print("--- 4. Testing Soundtrack Volume Fading ---")
    system = HydragonSoundtrackSystem()
    system.startup()

    # Start track with fade in over 1.0s to volume 0.8
    system.play_track("ambient", fade=True, fade_duration=1.0, target_volume=0.8)
    assert system.is_playing is True
    assert system._fade_direction == 1
    assert system.current_volume == 0.0

    # Step forward 0.5s -> should be halfway (0.4)
    system.step_soundtrack(0.5)
    assert abs(system.current_volume - 0.4) < 0.01, f"Expected ~0.4, got {system.current_volume}"
    assert system._fade_direction == 1

    # Step forward another 0.6s -> reaches target 0.8 and finishes fading
    system.step_soundtrack(0.6)
    assert system.current_volume == 0.8
    assert system._fade_direction == 0

    # Trigger fade-out stop over 1.0s
    system.stop_track(fade=True)
    assert system._fade_direction == -1

    # Step 0.5s -> volume drops to 0.4
    system.step_soundtrack(0.5)
    assert abs(system.current_volume - 0.4) < 0.01

    # Step 0.6s -> volume reaches 0 and stops
    system.step_soundtrack(0.6)
    assert system.current_volume == 0.0
    assert system.is_playing is False
    assert system._mock_stop_count >= 1

    system.shutdown()
    print("  [PASS] Soundtrack volume fading verified")


def test_soundtrack_crossfading():
    print("--- 5. Testing Track Crossfading ---")
    system = HydragonSoundtrackSystem()
    system.startup()

    # Start ambient track at full volume
    system.play_track("ambient", fade=False, target_volume=1.0)
    assert system.current_track_name == "ambient"
    assert system.current_volume == 1.0

    # Switch to combat track with fade
    system.play_track("combat", fade=True, fade_duration=1.0, target_volume=1.0)
    assert system._pending_track_change == "combat"
    assert system._fade_direction == -1

    # Step halfway down
    system.step_soundtrack(0.5)
    assert abs(system.current_volume - 0.5) < 0.01
    assert system.current_track_name == "ambient"

    # Step remaining down to 0 -> triggers start of combat track and starts fading in
    system.step_soundtrack(0.6)
    assert system.current_track_name == "combat"
    assert system._fade_direction == 1

    # Step to complete combat fade-in
    system.step_soundtrack(1.0)
    assert system.current_volume == 1.0
    assert system._fade_direction == 0

    system.shutdown()
    print("  [PASS] Track crossfading verified")


def test_soundtrack_asset_file_resolution():
    print("--- 6. Testing Soundtrack Asset Resolution ---")
    system = HydragonSoundtrackSystem()

    # Test resolving ambient track
    ambient_path = system._resolve_sound_file(DEFAULT_AMBIENT_SOUNDTRACK)
    assert ambient_path != "", f"Failed to resolve default ambient soundtrack: {DEFAULT_AMBIENT_SOUNDTRACK}"
    assert os.path.exists(ambient_path), f"Resolved ambient path does not exist on disk: {ambient_path}"
    assert ambient_path.endswith("lounge_soundtrack_01.wav")

    # Validate WAV file structure
    with wave.open(ambient_path, "rb") as wf:
        channels = wf.getnchannels()
        sampwidth = wf.getsampwidth()
        framerate = wf.getframerate()
        nframes = wf.getnframes()
        duration = nframes / float(framerate)
        assert channels in (1, 2), f"Expected 1 or 2 channels, got {channels}"
        assert framerate >= 22050, f"Expected standard sample rate, got {framerate}"
        assert duration > 1.0, f"Soundtrack duration should be > 1s, got {duration}s"
        print(f"  [PASS] Ambient soundtrack validated: {channels} channels, {framerate} Hz, {duration:.2f}s")


def test_soundtrack_dormant_without_prim():
    print("--- 7. Testing Strict Opt-In Dormancy on Bare Stages ---")
    system = HydragonSoundtrackSystem()
    system.startup()
    assert system._active_entity is None

    # Simulate timeline PLAY event without any soundtrack prim authored
    system._handle_timeline_play()
    assert system.is_playing is False
    assert system._mock_play_count == 0
    print("  [PASS] Soundtrack remains completely dormant when no soundtrack prim exists on stage")

    # Now bind a mock entity with auto_play=True
    mock_prim = MockPrim()
    entity = HydragonSoundtrackEntity(mock_prim)
    system._active_entity = entity
    system._handle_timeline_play()
    assert system.is_playing is True
    assert system.current_track_name == "ambient"
    print("  [PASS] Soundtrack plays when HydragonSoundtrack entity is authored and auto_play is True")

    system.shutdown()
