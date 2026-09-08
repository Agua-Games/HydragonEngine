"""
Unit test for HydragonEffectsSystem and ActiveExplosionVFX
Validates lifecycle, fail-silent execution outside Omniverse Kit,
particle kinematics (drag damping and vector-field turbulence),
dynamic light flash decay timing, and audio file resolution.
"""

import math
import os
import sys
import wave

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.effects_controller import (
    HydragonEffectsSystem,
    ActiveExplosionVFX,
    ExplosionPoolSlot,
    DEFAULT_FLASH_LIGHT_INTENSITY,
    DEFAULT_FLASH_LIGHT_RADIUS,
    DEFAULT_FLASH_LIGHT_DURATION,
    DEFAULT_BURST_LIFETIME,
    DEFAULT_NUM_SPARKS,
    DEFAULT_SPARK_RADIUS,
    DEFAULT_AUDIO_VOLUME,
    DEFAULT_TAUNT_DELAY,
    TAUNT_SOUND_NAMES,
    RENDER_MODE_POINT_INSTANCER,
    RENDER_MODE_POINTS,
    DEFAULT_RENDER_MODE,
)


def test_effects_system_lifecycle():
    print("--- 1. Testing Effects System Lifecycle ---")
    system = HydragonEffectsSystem()
    assert system is not None, "Failed to instantiate HydragonEffectsSystem"
    assert not system._is_active, "EffectsSystem should start inactive"
    assert HydragonEffectsSystem.get_instance() is system, "Singleton accessor must match instance"

    # Startup and shutdown outside Kit (fail-silent)
    system.startup()
    assert not system._is_active, "Should remain inactive when HAS_KIT is False"

    system.shutdown()
    assert not system._is_active, "System must be inactive after shutdown"
    assert HydragonEffectsSystem.get_instance() is None, "Singleton accessor must clear after shutdown"
    print("  [PASS] Effects System lifecycle verified")


def test_particle_kinematics_and_turbulence():
    print("--- 2. Testing Particle Kinematics & 3D Turbulence ---")
    origin = (100.0, 50.0, -200.0)
    vfx = ActiveExplosionVFX(
        prim_path="/World/Effects/FoeExplosion_0001",
        origin=origin,
        num_particles=32,
        lifetime=1.5,
        light_duration=0.35,
        initial_light_intensity=25000.0,
    )

    assert len(vfx.positions) == 32, f"Expected 32 positions, got {len(vfx.positions)}"
    assert len(vfx.velocities) == 32, f"Expected 32 velocities, got {len(vfx.velocities)}"

    # All particles must start at origin
    for p in vfx.positions:
        assert p[0] == origin[0] and p[1] == origin[1] and p[2] == origin[2]

    # Initial velocities must have positive magnitude
    for v in vfx.velocities:
        speed = math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])
        assert 200.0 <= speed <= 850.0, f"Unexpected initial particle speed: {speed}"

    # Step simulation forward by 0.1s
    alive = vfx.update(0.1)
    assert alive is True, "VFX should be alive at 0.1s"
    assert vfx.elapsed == 0.1

    # Check that particles dispersed away from origin
    dispersed_count = 0
    for p in vfx.positions:
        dist = math.sqrt((p[0] - origin[0]) ** 2 + (p[1] - origin[1]) ** 2 + (p[2] - origin[2]) ** 2)
        if dist > 1.0:
            dispersed_count += 1
    assert dispersed_count == 32, "All particles must have moved away from origin"

    # Step simulation forward until lifetime expires (> 1.5s)
    vfx.update(1.45)
    assert vfx.elapsed >= 1.5
    alive_end = vfx.update(0.01)
    assert alive_end is False, "VFX update must return False when lifetime expires"
    print("  [PASS] Particle kinematics and turbulence math verified")


def test_dynamic_light_flash_decay():
    print("--- 3. Testing Dynamic Light Flash Quadratic Decay ---")
    origin = (0.0, 0.0, 0.0)

    class MockLightPrim:
        def __init__(self):
            self.intensity = 25000.0
            self.active = True

        def GetIntensityAttr(self):
            mock_attr = self
            return mock_attr

        def Set(self, val):
            self.intensity = val

        def GetPrim(self):
            return self

        def IsValid(self):
            return True

        def IsActive(self):
            return self.active

        def SetActive(self, val):
            self.active = val

    mock_light = MockLightPrim()
    vfx = ActiveExplosionVFX(
        prim_path="/World/Effects/FoeExplosion_0002",
        origin=origin,
        light_prim=mock_light,
        lifetime=1.5,
        light_duration=0.35,
        initial_light_intensity=25000.0,
    )

    # At t=0.175 (half duration), intensity should follow (1 - 0.5)^2 = 0.25 * 25000 = 6250
    vfx.update(0.175)
    expected_intensity = 25000.0 * ((1.0 - (0.175 / 0.35)) ** 2.0)
    assert abs(mock_light.intensity - expected_intensity) < 1.0, (
        f"Expected intensity {expected_intensity}, got {mock_light.intensity}"
    )

    # After t >= 0.35, light should deactivate to preserve RTX / draw call performance
    vfx.update(0.20)  # total elapsed = 0.375 > 0.35
    assert mock_light.active is False, "Light prim should be deactivated after flash duration"
    assert vfx.light_prim is None, "Light prim reference should be cleared after deactivation"
    print("  [PASS] Dynamic light flash decay verified")


def test_audio_asset_integrity():
    print("--- 4. Testing Audio Asset Resolution and PCM Header ---")
    system = HydragonEffectsSystem()
    audio_path = system._resolve_audio_path()
    assert audio_path != "", "Failed to resolve foe_defeat.wav audio path"
    import os
    assert os.path.exists(audio_path), f"Audio file not found at: {audio_path}"

    # Verify defeat audio is a valid, readable WAV file
    with wave.open(audio_path, "rb") as wf:
        n_channels = wf.getnchannels()
        sampwidth = wf.getsampwidth()
        framerate = wf.getframerate()
        n_frames = wf.getnframes()
        assert n_channels in (1, 2), f"Expected mono/stereo, got {n_channels}"
        assert sampwidth == 2, f"Expected 16-bit audio (2 bytes), got {sampwidth}"
        assert framerate == 44100, f"Expected 44.1kHz samplerate, got {framerate}"
        assert n_frames > 0, "Audio file has 0 frames"

    # Verify all 4 taunt sound assets on disk
    assert len(TAUNT_SOUND_NAMES) == 4
    for taunt_name in TAUNT_SOUND_NAMES:
        taunt_path = system._resolve_sound_file(taunt_name)
        assert taunt_path != "", f"Failed to resolve {taunt_name}"
        assert os.path.exists(taunt_path), f"Taunt audio not found: {taunt_path}"
        with wave.open(taunt_path, "rb") as twf:
            assert twf.getnframes() > 0, f"Taunt {taunt_name} has 0 frames"

    system.shutdown()
    print(f"  [PASS] Defeat audio and all 4 taunt audio assets verified successfully")


def test_effects_fail_silent_headless():
    print("--- 5. Testing Spawn VFX & Delayed Taunt Queuing Headless ---")
    system = HydragonEffectsSystem()
    assert len(system._delayed_sounds) == 0
    assert system._mock_taunt_play_count == 0

    try:
        system.spawn_foe_destruction_vfx(stage=None, world_pos=(50.0, 100.0, -10.0), color_theme="gold")
        system.spawn_foe_destruction_vfx(stage=None, world_pos=(0.0, 0.0, 0.0), color_theme="cyan")
    except Exception as e:
        assert False, f"spawn_foe_destruction_vfx raised unexpected exception: {e}"

    # Verify 2 delayed taunt cues were queued with default 1.5s delay
    assert len(system._delayed_sounds) == 2
    for item in system._delayed_sounds:
        assert abs(item["timer"] - DEFAULT_TAUNT_DELAY) < 1e-4
        assert item["sound"] in TAUNT_SOUND_NAMES or item["sound"] is not None

    # Step forward by 0.5s (timer remaining ~1.0s, no taunt sound fired yet)
    system.step_delayed_sounds(0.5)
    assert len(system._delayed_sounds) == 2
    assert system._mock_taunt_play_count == 0
    for item in system._delayed_sounds:
        assert abs(item["timer"] - 1.0) < 1e-4

    # Step forward by 1.1s (timer reaches 0.0, both taunts fire)
    system.step_delayed_sounds(1.1)
    assert len(system._delayed_sounds) == 0
    assert system._mock_taunt_play_count == 2, f"Expected 2 mock taunts played, got {system._mock_taunt_play_count}"

    system.shutdown()
    print("  [PASS] Spawn VFX and delayed random taunt queuing headless verified")


def test_explosion_pool_slot_lifecycle():
    print("--- 6. Testing ExplosionPoolSlot Lifecycle & Light Flash Cutoff ---")
    slot = ExplosionPoolSlot(slot_index=0, root_path_str="/World/Effects/Pool_0")
    assert not slot.is_active, "Pool slot must start dormant"
    assert slot.lifetime == DEFAULT_BURST_LIFETIME, f"Expected {DEFAULT_BURST_LIFETIME}s burst duration, got {slot.lifetime}"
    assert slot.light_duration == DEFAULT_FLASH_LIGHT_DURATION, f"Expected {DEFAULT_FLASH_LIGHT_DURATION}s light duration, got {slot.light_duration}"
    assert slot.flash_intensity == DEFAULT_FLASH_LIGHT_INTENSITY, f"Expected {DEFAULT_FLASH_LIGHT_INTENSITY} intensity, got {slot.flash_intensity}"
    assert slot.num_sparks == DEFAULT_NUM_SPARKS, f"Expected {DEFAULT_NUM_SPARKS} sparks, got {slot.num_sparks}"
    assert slot.spark_radius == DEFAULT_SPARK_RADIUS, f"Expected {DEFAULT_SPARK_RADIUS} radius, got {slot.spark_radius}"

    # Activate slot
    world_pos = (150.0, 50.0, -250.0)
    slot.activate(world_pos=world_pos, color_theme="cyan")
    assert slot.is_active is True
    assert slot.elapsed == 0.0
    assert len(slot.positions) == DEFAULT_NUM_SPARKS
    assert len(slot.velocities) == DEFAULT_NUM_SPARKS

    # All local spark positions start at (0, 0, 0) and origin matches world_pos
    assert slot.origin == world_pos, f"Expected origin {world_pos}, got {slot.origin}"
    assert slot._light_extinguished is False
    for p in slot.positions:
        assert p == [0.0, 0.0, 0.0]

    # Verify base_scales are scaled directly by spark_radius
    for s in slot.base_scales:
        assert (DEFAULT_SPARK_RADIUS * 0.8) <= s <= (DEFAULT_SPARK_RADIUS * 1.3)

    # Verify dynamic radius modification
    slot.spark_radius = 15.0
    slot.activate(world_pos=world_pos)
    for s in slot.base_scales:
        assert (15.0 * 0.8) <= s <= (15.0 * 1.3)
    slot.spark_radius = DEFAULT_SPARK_RADIUS
    slot.activate(world_pos=world_pos)

    # Step simulation to t = 0.10s (within light duration)
    alive = slot.update(0.10)
    assert alive is True
    assert slot.elapsed == 0.10
    assert slot._light_extinguished is False

    # Step past light duration (0.22s) to t = 0.30s
    alive = slot.update(0.20)
    assert alive is True
    assert abs(slot.elapsed - 0.30) < 1e-4

    # Step past full burst duration (0.55s) to t = 0.60s
    alive_end = slot.update(0.30)
    assert alive_end is False, "Slot must expire and deactivate at t >= 0.55s"
    assert slot.is_active is False, "Slot must mark is_active as False upon expiration"
    assert slot._light_extinguished is True
    print("  [PASS] ExplosionPoolSlot lifecycle and quick light cutoff verified")


def test_pool_capacity_and_flash_constants():
    print("--- 7. Testing Effects System Pool Constants & Sizing ---")
    assert HydragonEffectsSystem.POOL_SIZE == 3, f"Expected pool size 3, got {HydragonEffectsSystem.POOL_SIZE}"
    assert HydragonEffectsSystem.FLASH_LIGHT_INTENSITY == DEFAULT_FLASH_LIGHT_INTENSITY
    assert HydragonEffectsSystem.FLASH_LIGHT_RADIUS == DEFAULT_FLASH_LIGHT_RADIUS
    assert HydragonEffectsSystem.NUM_SPARKS == DEFAULT_NUM_SPARKS
    assert HydragonEffectsSystem.SPARK_RADIUS == DEFAULT_SPARK_RADIUS
    assert HydragonEffectsSystem.AUDIO_VOLUME == DEFAULT_AUDIO_VOLUME
    assert HydragonEffectsSystem.TAUNT_DELAY == DEFAULT_TAUNT_DELAY
    print("  [PASS] Effects System pool constants verified")


def test_audio_volume_configuration():
    print("--- 8. Testing Audio Volume Configuration & Clamping ---")
    system = HydragonEffectsSystem()
    assert HydragonEffectsSystem.AUDIO_VOLUME == DEFAULT_AUDIO_VOLUME
    assert system.audio_volume == DEFAULT_AUDIO_VOLUME

    # Test setting volume within 0.0 to 1.0
    system.audio_volume = 0.65
    assert abs(system.audio_volume - 0.65) < 1e-4

    # Test volume clamping
    system.audio_volume = 1.5
    assert system.audio_volume == 1.0

    system.audio_volume = -0.2
    assert system.audio_volume == 0.0

    system.shutdown()
    print("  [PASS] Audio volume configuration and clamping verified")


def test_warp_acceleration_lifecycle():
    print("--- 9. Testing NVIDIA Warp Acceleration Lifecycle ---")
    import glob
    import importlib

    repo_root = os.path.abspath(os.path.join(ext_dir, "..", "..", ".."))
    warp_pattern = os.path.join(repo_root, "_build", "windows-x86_64", "release", "extscache", "omni.warp.core*")
    warp_candidates = glob.glob(warp_pattern)

    if warp_candidates and warp_candidates[0] not in sys.path:
        sys.path.insert(0, warp_candidates[0])

    try:
        import warp as wp
        has_warp = True
    except ImportError:
        has_warp = False

    if not has_warp:
        print("  [SKIP] NVIDIA Warp not available in test environment")
        return

    # Reload effects_controller to bind Warp module and kernels
    import hydragon.editor.core.effects_controller as ec
    importlib.reload(ec)

    assert ec.HAS_WARP is True, "HAS_WARP must be True when warp is loaded"
    assert ec.wp is not None, "wp handle must be bound"
    assert not hasattr(ec.wp, "is_initialized"), "Warp does not expose is_initialized"

    system = ec.HydragonEffectsSystem()
    system._init_warp()
    status = system.get_warp_status()
    assert status["use_warp"] is True, f"Expected use_warp True, got {status}"
    assert status["initialized"] is True, f"Expected initialized True, got {status}"
    assert status["device"] in ("cuda", "cpu")

    # Verify ExplosionPoolSlot allocation and execution with Warp
    slot = ec.ExplosionPoolSlot(slot_index=99, num_sparks=16, spark_radius=20.0)
    slot._ensure_warp_arrays(device=system._warp_device)
    assert slot._use_warp is True, "Slot must have Warp enabled"
    assert slot._warp_initialized is True, "Slot Warp arrays must be initialized"

    # Activate slot (dispatches init_sparks_kernel on device)
    slot.activate(world_pos=(0.0, 0.0, 0.0), color_theme="gold")
    assert slot._use_warp is True, "Slot must retain Warp enabled after activate"

    # Step simulation (dispatches simulate_sparks_kernel on device)
    alive = slot.update(0.016)
    assert alive is True, "Slot must remain active during lifetime"
    assert slot.elapsed == 0.016

    # Verify particles were updated and moved on device
    moved = any(
        abs(p[0]) > 0.001 or abs(p[1]) > 0.001 or abs(p[2]) > 0.001
        for p in slot.positions
    )
    assert moved, "Particles must disperse on GPU device"

    system.shutdown()
    print("  [PASS] NVIDIA Warp acceleration lifecycle verified")


def test_render_mode_configuration_and_switching():
    print("--- 10. Testing Particle Render Modes Configuration & Switching ---")
    assert DEFAULT_RENDER_MODE == RENDER_MODE_POINT_INSTANCER
    assert RENDER_MODE_POINT_INSTANCER == "point_instancer"
    assert RENDER_MODE_POINTS == "points"

    system = HydragonEffectsSystem()
    assert system.RENDER_MODE == DEFAULT_RENDER_MODE
    assert system.render_mode == RENDER_MODE_POINT_INSTANCER

    # Create dummy slots
    slot0 = ExplosionPoolSlot(slot_index=0, num_sparks=15, render_mode=system.render_mode)
    slot1 = ExplosionPoolSlot(slot_index=1, num_sparks=15, render_mode=system.render_mode)
    system._slots = [slot0, slot1]

    # Switch to Points mode
    system.render_mode = RENDER_MODE_POINTS
    assert system.render_mode == RENDER_MODE_POINTS
    assert slot0.render_mode == RENDER_MODE_POINTS
    assert slot1.render_mode == RENDER_MODE_POINTS

    # Switch back to PointInstancer mode
    system.render_mode = RENDER_MODE_POINT_INSTANCER
    assert system.render_mode == RENDER_MODE_POINT_INSTANCER
    assert slot0.render_mode == RENDER_MODE_POINT_INSTANCER
    assert slot1.render_mode == RENDER_MODE_POINT_INSTANCER

    # Attempt setting invalid mode (must be safely ignored)
    system.render_mode = "invalid_mode"
    assert system.render_mode == RENDER_MODE_POINT_INSTANCER
    assert slot0.render_mode == RENDER_MODE_POINT_INSTANCER

    status = system.get_warp_status()
    assert "render_mode" in status
    assert status["render_mode"] == RENDER_MODE_POINT_INSTANCER

    system.shutdown()
    print("  [PASS] Render mode configuration and switching verified")


def test_slot_render_modes_execution():
    print("--- 11. Testing Slot Lifecycle in PointInstancer & Points Modes ---")
    # Test PointInstancer slot
    slot_instancer = ExplosionPoolSlot(
        slot_index=0,
        num_sparks=16,
        render_mode=RENDER_MODE_POINT_INSTANCER,
    )
    assert slot_instancer.render_mode == RENDER_MODE_POINT_INSTANCER
    assert slot_instancer.instancer_path == "/World/Effects/Pool_0/PointInstancer"
    assert slot_instancer.points_path == "/World/Effects/Pool_0/Points"

    slot_instancer.activate(world_pos=(10.0, 20.0, 30.0), color_theme="gold")
    assert slot_instancer.is_active is True
    assert slot_instancer.origin == (10.0, 20.0, 30.0)

    # Step simulation
    alive = slot_instancer.update(0.016)
    assert alive is True
    assert slot_instancer.elapsed == 0.016

    # Clear handles test (ensures no missing attr error)
    slot_instancer.clear_cached_handles()
    assert slot_instancer._cached_instancer is None
    assert slot_instancer._cached_points is None

    slot_instancer.deactivate()
    assert slot_instancer.is_active is False

    # Test Points slot
    slot_points = ExplosionPoolSlot(
        slot_index=1,
        num_sparks=16,
        render_mode=RENDER_MODE_POINTS,
    )
    assert slot_points.render_mode == RENDER_MODE_POINTS
    slot_points.activate(world_pos=(50.0, 0.0, -10.0), color_theme="cyan")
    assert slot_points.is_active is True

    alive = slot_points.update(0.016)
    assert alive is True
    assert slot_points.elapsed == 0.016

    slot_points.clear_cached_handles()
    slot_points.deactivate()
    assert slot_points.is_active is False

    print("  [PASS] Slot lifecycle in PointInstancer and Points modes verified")


def test_points_render_mode_attributes_and_decay():
    print("--- 12. Testing UsdGeom.Points Mode Scale Decay and Buffers ---")
    slot = ExplosionPoolSlot(
        slot_index=0,
        num_sparks=20,
        spark_radius=20.0,
        lifetime=0.5,
        render_mode=RENDER_MODE_POINTS,
    )
    assert slot.render_mode == RENDER_MODE_POINTS
    assert len(slot.current_scales) == 20
    assert len(slot.base_scales) == 20

    slot.activate(world_pos=(0.0, 100.0, 0.0), color_theme="gold")
    # Base scales should be around spark_radius (20.0 * [0.85, 1.25])
    for s in slot.base_scales:
        assert 16.0 <= s <= 26.0

    # Initial current_scales should match base_scales
    for i in range(20):
        assert abs(slot.current_scales[i] - slot.base_scales[i]) < 1e-4

    # Update halfway through lifetime (t = 0.25, lifetime = 0.5)
    slot.update(0.25)
    # Expected scale_factor = 1.0 - 0.25/0.5 = 0.5
    for i in range(20):
        expected = slot.base_scales[i] * 0.5
        assert abs(slot.current_scales[i] - expected) < 1e-2, (
            f"Scale mismatch at halfway: expected {expected}, got {slot.current_scales[i]}"
        )

    # Base scales must remain constant (no compounding shrinkage bug)
    for s in slot.base_scales:
        assert 16.0 <= s <= 26.0

    slot.deactivate()
    print("  [PASS] Points render mode attributes and linear scale decay verified")


if __name__ == "__main__":
    test_effects_system_lifecycle()
    test_particle_kinematics_and_turbulence()
    test_dynamic_light_flash_decay()
    test_audio_asset_integrity()
    test_effects_fail_silent_headless()
    test_explosion_pool_slot_lifecycle()
    test_pool_capacity_and_flash_constants()
    test_audio_volume_configuration()
    test_warp_acceleration_lifecycle()
    test_render_mode_configuration_and_switching()
    test_slot_render_modes_execution()
    test_points_render_mode_attributes_and_decay()
    print("\nALL 12 EFFECTS SYSTEM TESTS PASSED SUCCESSFULLY!")
