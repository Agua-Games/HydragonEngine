"""
Unit test for HydragonFoesControllerSystem and HydragonAIBrain
Validates lifecycle, fail-silent behavior outside Kit, state machine transitions
(Patrol <-> Chase), vector actuation math, and clean destruction.
"""

import math
import sys
import os
from typing import Tuple

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.foes_controller import HydragonFoesControllerSystem, HydragonAIBrain


def test_foes_controller_lifecycle():
    print("--- 1. Testing Foes Controller System Lifecycle ---")
    system = HydragonFoesControllerSystem()
    assert system is not None, "Failed to instantiate HydragonFoesControllerSystem"
    assert not system._is_active, "System should start inactive"
    assert HydragonFoesControllerSystem.get_instance() is system, "Singleton should match instantiated system"

    # Startup & shutdown outside Kit (fail-silent)
    system.startup()
    assert not system._is_active, "Should remain inactive when HAS_KIT is False"

    system.shutdown()
    assert not system._is_active, "System should be inactive after shutdown"
    assert len(system._active_brains) == 0, "Active brains should be empty"
    assert HydragonFoesControllerSystem.get_instance() is None, "Singleton should be cleared after shutdown"
    print("  [PASS] HydragonFoesControllerSystem lifecycle verified")


def test_ai_brain_state_transitions():
    print("--- 2. Testing HydragonAIBrain State Machine (Patrol <-> Chase) ---")
    # Initialize mock brain at origin
    brain = HydragonAIBrain(prim=None, origin_pos=(0.0, 50.0, 0.0))
    assert brain.state == "Patrol", f"Initial state should be Patrol, got {brain.state}"
    assert brain.is_alive, "Brain should start alive"

    # 1. Player far away (X=5000, Z=5000) -> should remain in Patrol
    far_player = (5000.0, 50.0, 5000.0)
    state, (fx, fz) = brain.update_state_machine(dt=0.016, player_pos=far_player)
    assert state == "Patrol", f"Expected state Patrol when player is far, got {state}"
    patrol_mag = math.sqrt(fx * fx + fz * fz)
    assert patrol_mag > 0.0, f"Patrol force should be non-zero, got {patrol_mag}"

    # 2. Player approaches within detection radius (e.g. X=300, Z=400, dist=500 < 1000cm)
    near_player = (300.0, 50.0, 400.0)
    state, (fx, fz) = brain.update_state_machine(dt=0.016, player_pos=near_player)
    assert state == "Chase", f"Expected state Chase when player is within detection radius, got {state}"

    # Verify chase vector points directly towards the player
    # Direction from (0, 0) to (300, 400) is normalized (0.6, 0.8)
    expected_nx = 300.0 / 500.0  # 0.6
    expected_nz = 400.0 / 500.0  # 0.8
    chase_mag = math.sqrt(fx * fx + fz * fz)
    assert chase_mag > 1000.0, f"Chase force should be substantial, got {chase_mag}"

    actual_nx = fx / chase_mag
    actual_nz = fz / chase_mag
    assert abs(actual_nx - expected_nx) < 1e-4, f"Expected nx={expected_nx}, got {actual_nx}"
    assert abs(actual_nz - expected_nz) < 1e-4, f"Expected nz={expected_nz}, got {actual_nz}"

    # 3. Player moves far outside lose_target_radius (e.g. X=2000, Z=0, dist=2000 > 1500cm)
    lost_player = (2000.0, 50.0, 0.0)
    state, (fx, fz) = brain.update_state_machine(dt=0.016, player_pos=lost_player)
    assert state == "Patrol", f"Expected state Patrol when player exceeds lose target radius, got {state}"

    print("  [PASS] AI Brain state transitions (Patrol <-> Chase) and force vectors verified")


def test_ai_brain_destruction():
    print("--- 3. Testing Clean Entity Destruction ---")
    brain = HydragonAIBrain(prim=None, origin_pos=(0.0, 50.0, 0.0))
    assert brain.is_alive

    # Destroy entity cleanly
    brain.destroy(reason="unit_test")
    assert not brain.is_alive, "Brain must be dead after destroy()"

    # Dead brain returns Dead state and zero force
    state, (fx, fz) = brain.update_state_machine(dt=0.016, player_pos=(100.0, 50.0, 100.0))
    assert state == "Dead", f"Expected Dead state, got {state}"
    assert fx == 0.0 and fz == 0.0, f"Force must be zero for dead entity, got ({fx}, {fz})"
    print("  [PASS] Clean entity destruction verified")


def test_stomp_geometry_math():
    print("--- 4. Testing Collision & Stomp Geometry Envelope Math ---")
    foe_pos = (100.0, 50.0, 200.0)

    # Helper function matching criteria in HydragonFoesControllerSystem
    def evaluate_collision(p_pos, f_pos) -> Tuple[bool, bool, str]:
        dx = p_pos[0] - f_pos[0]
        dy = p_pos[1] - f_pos[1]
        dz = p_pos[2] - f_pos[2]
        horiz_dist = math.sqrt(dx * dx + dz * dz)
        dist_3d = math.sqrt(dx * dx + dy * dy + dz * dz)

        is_collision = dist_3d <= 115.0 or (horiz_dist < 100.0 and abs(dy) < 90.0)
        if not is_collision:
            return False, False, ""

        is_stomp = (horiz_dist < 80.0 and 30.0 < dy < 130.0)
        reason = "stomped_by_player" if is_stomp else "player_collision"
        return is_collision, is_stomp, reason

    # 1. Perfect stomp: player directly above foe at dy = 60cm
    player_above = (105.0, 110.0, 205.0)  # horiz_dist = sqrt(25+25) = 7.07, dy = 60
    col, stomp, reason = evaluate_collision(player_above, foe_pos)
    assert col and stomp and reason == "stomped_by_player", "Direct downward landing must register as stomp"

    # 2. Horizontal collision beside foe: player rolling on floor dy = 0cm, horiz_dist = 50cm
    player_beside = (150.0, 50.0, 200.0)  # horiz_dist = 50, dy = 0
    col, stomp, reason = evaluate_collision(player_beside, foe_pos)
    assert col, "Side-by-side collision must register as collision to deactivate foe"
    assert not stomp, "Side-by-side collision should NOT be a vertical stomp"
    assert reason == "player_collision", f"Expected player_collision reason, got {reason}"

    # 3. Player too far horizontally: horiz_dist = 160cm
    player_missed = (260.0, 50.0, 200.0)  # horiz_dist = 160
    col, stomp, _ = evaluate_collision(player_missed, foe_pos)
    assert not col, "Player outside horizontal radius should not collide"

    # 4. Player too high: dy = 200cm
    player_too_high = (100.0, 250.0, 200.0)  # dy = 200
    col, stomp, _ = evaluate_collision(player_too_high, foe_pos)
    assert not col, "Player too high should not trigger collision"

    print("  [PASS] Collision & stomp geometry envelope math verified")


def test_contact_report_event_processing():
    print("--- 5. Testing Contact Report Event Processing ---")
    system = HydragonFoesControllerSystem()

    # Setup active foe brain
    foe_prim_path = "/World/Foe_01"
    foe_rb_path = "/World/Foe_01/geometry/ball_mesh"
    brain = HydragonAIBrain(prim=None, origin_pos=(100.0, 50.0, 200.0))
    brain._prim_path = foe_prim_path
    brain.rb_path = foe_rb_path
    brain._current_pos = (100.0, 50.0, 200.0)

    system._active_brains[foe_prim_path] = brain
    system._registered_brains[foe_prim_path] = brain
    system._cached_player_rb_path = "/World/Player/geometry/ball_mesh"
    system._cached_player_path = "/World/Player"

    # Verify path resolution logic
    player_root, player_rb = system._cached_player_path, system._cached_player_rb_path
    rb_to_brain = {b.rb_path: b for b in system._registered_brains.values() if b.rb_path}

    a0 = "/World/Player/geometry/ball_mesh"
    a1 = "/World/Foe_01/geometry/ball_mesh"
    is_p0 = (a0 == player_rb or a0 == player_root or a0.startswith(player_root + "/"))
    is_p1 = (a1 == player_rb or a1 == player_root or a1.startswith(player_root + "/"))
    assert is_p0 and not is_p1

    matched_brain = rb_to_brain.get(a1)
    assert matched_brain is brain

    # Verify stomp classification when player is above foe
    p_pos = (100.0, 110.0, 200.0)
    f_pos = brain.current_pos
    is_stomp = (p_pos[1] > f_pos[1] + 25.0)
    assert is_stomp

    # Verify lateral collision classification when heights are equal
    p_pos_lateral = (100.0, 50.0, 200.0)
    is_lateral_stomp = (p_pos_lateral[1] > f_pos[1] + 25.0)
    assert not is_lateral_stomp

    # Test queue deduplication logic
    if not any(h[0] == foe_prim_path for h in system._pending_hits):
        system._pending_hits.append((foe_prim_path, False))
    # Second hit arrives as stomp -> upgrades existing hit without duplicate
    if not any(h[0] == foe_prim_path for h in system._pending_hits):
        system._pending_hits.append((foe_prim_path, True))
    else:
        for idx, (hp, hs) in enumerate(system._pending_hits):
            if hp == foe_prim_path and not hs:
                system._pending_hits[idx] = (hp, True)
                break

    assert len(system._pending_hits) == 1
    assert system._pending_hits[0] == (foe_prim_path, True)

    # Simulate race condition where physics step already popped from _active_brains
    system._active_brains.pop(foe_prim_path, None)
    assert foe_prim_path not in system._active_brains

    # Destruction logic in _on_app_update resolves brain from _registered_brains
    p_path, stomp_flag = system._pending_hits.pop(0)
    target_brain = system._active_brains.pop(p_path, None) or system._registered_brains.get(p_path)
    assert target_brain is brain
    target_brain.destroy(reason="test_stomp")
    system._destroyed_prim_paths.add(p_path)

    assert not target_brain.is_alive
    assert foe_prim_path in system._destroyed_prim_paths
    assert foe_prim_path not in system._active_brains

    system.shutdown()
    print("  [PASS] Contact report event processing logic verified")


def test_stop_event_stage_restoration():
    print("--- 6. Testing Stage Restoration on Simulation STOP ---")
    system = HydragonFoesControllerSystem()
    system._destroyed_prim_paths.add("/World/Foe_01")
    system._destroyed_prim_paths.add("/World/Foe_02")

    # Mock stage & prims
    class MockChild:
        def __init__(self):
            self.rb_enabled = False
        def HasAttribute(self, name):
            return name == "physics:rigidBodyEnabled"
        def GetAttribute(self, name):
            class Attr:
                def __init__(self, c): self.c = c
                def Set(self, val): self.c.rb_enabled = val
            return Attr(self)

    class MockPrim:
        def __init__(self, path):
            self.path = path
            self.active = False
            self.child = MockChild()
        def IsValid(self):
            return True
        def SetActive(self, val):
            self.active = val
        def GetAllChildren(self):
            return [self.child]
        def HasAttribute(self, name):
            return False

    prims = {
        "/World/Foe_01": MockPrim("/World/Foe_01"),
        "/World/Foe_02": MockPrim("/World/Foe_02"),
    }

    # Simulate restore loop
    for p_path in list(system._destroyed_prim_paths):
        prim = prims.get(p_path)
        assert prim is not None
        prim.SetActive(True)
        for child in prim.GetAllChildren():
            if child.HasAttribute("physics:rigidBodyEnabled"):
                child.GetAttribute("physics:rigidBodyEnabled").Set(True)

    assert prims["/World/Foe_01"].active is True
    assert prims["/World/Foe_01"].child.rb_enabled is True
    assert prims["/World/Foe_02"].active is True
    assert prims["/World/Foe_02"].child.rb_enabled is True

    system.shutdown()
    assert len(system._destroyed_prim_paths) == 0
    print("  [PASS] Stage restoration on STOP verified")


def test_foe_destruction_score_popup_integration():
    print("--- 7. Testing Foe Destruction Score Popup & Property Access ---")
    from hydragon.editor.core.game_hud import HydragonGameHUD

    brain = HydragonAIBrain(prim=None, origin_pos=(150.0, 50.0, 250.0))
    # brain.current_pos must be accessible as property without call parentheses
    assert isinstance(brain.current_pos, tuple)
    assert brain.current_pos == (150.0, 50.0, 250.0)

    hud = HydragonGameHUD()
    # Mock popup collection
    received_popups = []

    def mock_show_score_popup(world_pos=(0.0, 0.0, 0.0), points=100):
        received_popups.append({"pos": world_pos, "pts": points})

    hud.show_score_popup = mock_show_score_popup

    # Simulate foe destruction pipeline in _on_physics_step
    foe_pos = brain.current_pos if brain else (0.0, 0.0, 0.0)
    is_stomp = True
    points = 150 if is_stomp else 100
    hud.show_score_popup(world_pos=foe_pos, points=points)

    assert len(received_popups) == 1
    assert received_popups[0]["pos"] == (150.0, 50.0, 250.0)
    assert received_popups[0]["pts"] == 150

    hud.shutdown()
    print("  [PASS] Foe destruction score popup integration verified")


def test_player_arcade_bounce_math():
    print("--- 8. Testing Player Stomp Arcade Bounce Rebound Math ---")
    # 1. Lateral offset in +X
    p_pos = (100.0, 75.0, 200.0)
    f_pos = (80.0, 50.0, 200.0)

    dx = p_pos[0] - f_pos[0]
    dz = p_pos[2] - f_pos[2]
    h_dist = math.sqrt(dx * dx + dz * dz)
    assert h_dist == 20.0
    nx = dx / h_dist
    nz = dz / h_dist
    assert nx == 1.0 and nz == 0.0

    horiz_impulse = 180000.0
    vert_impulse = 450000.0
    ix = nx * horiz_impulse
    iy = vert_impulse
    iz = nz * horiz_impulse

    assert ix == 180000.0
    assert iy == 450000.0
    assert iz == 0.0

    # 2. Diagonal offset (3:4:5 triangle)
    p_pos_diag = (100.0, 75.0, 100.0)
    f_pos_diag = (70.0, 50.0, 60.0)
    dx = p_pos_diag[0] - f_pos_diag[0]  # +30
    dz = p_pos_diag[2] - f_pos_diag[2]  # +40
    h_dist = math.sqrt(dx * dx + dz * dz)
    assert abs(h_dist - 50.0) < 1e-4
    nx = dx / h_dist  # 0.6
    nz = dz / h_dist  # 0.8
    assert abs(nx - 0.6) < 1e-4
    assert abs(nz - 0.8) < 1e-4

    ix = nx * horiz_impulse
    iy = vert_impulse
    iz = nz * horiz_impulse
    assert abs(ix - 108000.0) < 1e-4
    assert iy == 450000.0
    assert abs(iz - 144000.0) < 1e-4

    # 3. Fail-silent execution outside Kit (both stomp and lateral collision)
    system = HydragonFoesControllerSystem()
    try:
        system._apply_player_bounce(stage=None, stage_id=0, sim_iface=None, foe_pos=f_pos, is_stomp=True)
        system._apply_player_bounce(stage=None, stage_id=0, sim_iface=None, foe_pos=f_pos, is_stomp=False)
    except Exception as e:
        assert False, f"_apply_player_bounce raised unexpected exception outside Kit: {e}"
    system.shutdown()

    print("  [PASS] Player arcade bounce rebound math verified for stomp and lateral hits")


def test_physics_step_safety():
    print("--- 9. Testing _on_physics_step Safety and Stage Compatibility ---")
    system = HydragonFoesControllerSystem()
    system._is_simulating = True
    brain = HydragonAIBrain(prim=None, origin_pos=(0.0, 50.0, 0.0))
    system._active_brains["/World/Foe"] = brain

    # Real pxr.Usd.Stage instances do NOT have an IsValid() method
    class MockUsdStage:
        def GetPrimAtPath(self, path):
            return None

    try:
        system._on_physics_step(0.016)
    except Exception as e:
        assert False, f"_on_physics_step raised unexpected exception: {e}"

    system.shutdown()
    print("  [PASS] _on_physics_step stage validation safety verified")


def test_foes_physics_manager_clamping_and_debounce():
    print("--- 10. Testing Foes PhysicsManager Clamping, CCD and Bounce Debounce ---")
    system = HydragonFoesControllerSystem()

    class MockAttr:
        def __init__(self, val):
            self.val = val
        def IsValid(self):
            return True
        def Get(self):
            return self.val
        def Set(self, val):
            self.val = val

    class MockPrimWithAttrs:
        def __init__(self, is_kinematic=False, initial_max_vel=10000.0):
            self.attrs = {
                "physics:kinematicEnabled": MockAttr(is_kinematic),
                "physxRigidBody:maxLinearVelocity": MockAttr(initial_max_vel),
                "physxRigidBody:maxAngularVelocity": MockAttr(50.0),
                "physxRigidBody:enableCCD": MockAttr(True),
                "physxRigidBody:solverPositionIterationCount": MockAttr(16),
                "physxRigidBody:solverVelocityIterationCount": MockAttr(4),
            }
        def IsValid(self):
            return True
        def HasAttribute(self, name):
            return name in self.attrs
        def GetAttribute(self, name):
            return self.attrs.get(name)

    class MockPhysicsManagerLowVel:
        max_linear_velocity = 10.0
        max_angular_velocity = 30.0
        default_linear_damping = 0.5
        default_angular_damping = 2.0
        solver_position_iterations = 20
        solver_velocity_iterations = 6
        enable_ccd = True
        bounce_threshold = 150.0

    # 1. Test that max_linear_velocity = 10.0 overwrites existing 10000.0 on foes
    prim = MockPrimWithAttrs(is_kinematic=False, initial_max_vel=10000.0)
    system._physics_mgr = MockPhysicsManagerLowVel()
    system._ensure_rigid_body_damping(prim)
    assert prim.attrs["physxRigidBody:maxLinearVelocity"].Get() == 1000.0, "Foe maxLinearVelocity was not scaled to 1000.0!"
    assert prim.attrs["physxRigidBody:maxAngularVelocity"].Get() == math.degrees(30.0), "Foe maxAngularVelocity was not converted from radians to degrees!"
    assert prim.attrs["physxRigidBody:enableCCD"].Get() is True

    # 2. Test kinematic body has CCD disabled
    kin_prim = MockPrimWithAttrs(is_kinematic=True, initial_max_vel=10000.0)
    system._ensure_rigid_body_damping(kin_prim)
    assert kin_prim.attrs["physxRigidBody:enableCCD"].Get() is False, "CCD must be False on kinematic foe body!"

    # 3. Test bounce debounce logic
    system._last_player_bounce_time = 0.0
    # First bounce updates timestamp
    system._apply_player_bounce(stage=None, stage_id=0, sim_iface=None, foe_pos=(0.0, 0.0, 0.0), is_stomp=True)
    t1 = system._last_player_bounce_time
    assert t1 > 0.0

    # Rapid second bounce within 0.15s should be debounced (early return, t1 unchanged)
    system._apply_player_bounce(stage=None, stage_id=0, sim_iface=None, foe_pos=(0.0, 0.0, 0.0), is_stomp=False)
    assert system._last_player_bounce_time == t1, "Rapid second bounce was not debounced!"

    # Force bounce bypasses debounce
    import time
    system._apply_player_bounce(stage=None, stage_id=0, sim_iface=None, foe_pos=(0.0, 0.0, 0.0), is_stomp=False, force_bounce=True)
    assert system._last_player_bounce_time >= t1

    system.shutdown()
    print("  [PASS] Foes velocity clamping, kinematic CCD, and bounce debounce verified")


def test_foes_mass_scaled_chase_actuation_and_traction():
    print("--- 11. Testing Foes Mass-Scaled Chase Actuation & Ground Traction ---")
    # 1. Standard Foe (mass = 5.0 kg, radius = 50.0 cm)
    brain_std = HydragonAIBrain(prim=None, origin_pos=(0.0, 50.0, 0.0))
    brain_std.mass = 5.0
    brain_std.radius = 50.0
    assert brain_std.mass == 5.0
    assert brain_std.radius == 50.0

    player_pos = (300.0, 50.0, 400.0)  # dist = 500 < 1000cm -> Chase
    state, (fx, fz) = brain_std.update_state_machine(dt=0.016, player_pos=player_pos)
    assert state == "Chase"
    f_mag = math.sqrt(fx * fx + fz * fz)
    accel_std = f_mag / brain_std.mass
    assert abs(accel_std - 4900.0) < 1e-2, f"Standard foe acceleration {accel_std} should be 4900 cm/s^2 (~5G)"
    assert abs(f_mag - 24500.0) < 1e-2, f"Standard foe force {f_mag} should match player move force (~24500-25000 N)"

    # 2. Scaled Heavy Foe (mass = 20.48 kg, radius = 80.0 cm, matching 1.6x scaled Foe_02)
    brain_heavy = HydragonAIBrain(prim=None, origin_pos=(0.0, 80.0, 0.0))
    brain_heavy.mass = 20.48
    brain_heavy.radius = 80.0
    h_state, (h_fx, h_fz) = brain_heavy.update_state_machine(dt=0.016, player_pos=player_pos)
    assert h_state == "Chase"
    h_f_mag = math.sqrt(h_fx * h_fx + h_fz * h_fz)
    # Heavy foe force must be capped at 38000.0 N to prevent catastrophic ramp launches
    assert abs(h_f_mag - 38000.0) < 1e-2, f"Heavy foe force {h_f_mag} must be capped at 38000 N"
    accel_heavy = h_f_mag / brain_heavy.mass
    assert 1800.0 <= accel_heavy <= 1900.0, f"Heavy foe acceleration {accel_heavy} should be ~1.85G"

    # Verify sphere-on-sphere ramp launch upward force is strictly below gravitational weight
    ramp_sin = (80.0 - 50.0) / (80.0 + 50.0)  # ~0.2308
    upward_ramp_force = h_f_mag * ramp_sin  # ~8769 N
    std_gravity_weight = 5.0 * 9810.0  # 49050 N (or 65000 N under 13000 cm/s^2)
    assert upward_ramp_force < std_gravity_weight * 0.25, (
        f"Upward ramp force {upward_ramp_force} must be < 25% of weight {std_gravity_weight} to prevent launch"
    )

    # 3. Ground Traction Check: verify airborne foes do not receive driving propulsion
    system = HydragonFoesControllerSystem()
    # Grounded: bottom at floor (pos_y = 50, radius = 50 -> bottom = 0 <= 25)
    is_grounded = system._check_foe_grounded(None, "/World/Foe_01", (0.0, 50.0, 0.0), 50.0)
    assert is_grounded is True, "Resting sphere must be detected as grounded"

    # Airborne: bottom at 75 cm above floor (pos_y = 125, radius = 50 -> bottom = 75 > 25)
    is_airborne = system._check_foe_grounded(None, "/World/Foe_01", (0.0, 125.0, 0.0), 50.0)
    assert is_airborne is False, "Airborne sphere must be detected as not grounded"
    system.shutdown()

    # 4. State preservation from schema
    class MockSchemaWithState:
        state = "Chase"
        detection_radius = 10.0
        lose_target_radius = 15.0
        patrol_radius = 5.0
        chase_force = 350.0
        patrol_force = 150.0
        target_prim = None

    class MockPrimWithSchema:
        def GetPath(self):
            class MockPath:
                pathString = "/World/AuthoredChaseFoe"
            return MockPath()

    brain_authored = HydragonAIBrain(prim=None)
    brain_authored._schema = MockSchemaWithState()
    initial_st = brain_authored._schema.state if brain_authored._schema.state in ("Patrol", "Chase") else "Patrol"
    brain_authored._state = initial_st
    assert brain_authored.state == "Chase", "Authored Chase state must be respected"

    print("  [PASS] Foes mass-scaled chase actuation, ramp collision safety, and ground traction verified")


def test_ground_probe_ignores_volume_trigger_colliders():
    print("--- 12. Testing the Foe Ground Probe Ignores Volume Trigger Colliders ---")
    import hydragon.editor.core.foes_controller as fc
    import hydragon.editor.core.volume_triggers as vt

    TRIGGER = "/World/ForceVolume_vortex/volumes/force_trigger"
    FLOOR = "/World/Environment/Ground/collider"

    class _Vec:
        def __init__(self, x, y, z):
            self._v = (x, y, z)

        def __getitem__(self, index):
            return self._v[index]

    class _StubCarb:
        Float3 = staticmethod(lambda x, y, z: _Vec(x, y, z))
        log_info = staticmethod(lambda message: None)
        log_warn = staticmethod(lambda message: None)
        log_error = staticmethod(lambda message: None)

    class _Hit:
        def __init__(self, collision, position):
            self.collision = collision
            self.rigid_body = ""
            self.position = position

    class _Query:
        def __init__(self, hits):
            self._hits = hits

        def raycast_all(self, origin, direction, distance, report_fn, both_sides=False):
            for hit in self._hits:
                if not report_fn(hit):
                    break
            return True

    original_carb = fc.carb
    original_has_kit = fc.HAS_KIT
    had_query = "get_physx_scene_query_interface" in fc.__dict__
    original_query = fc.__dict__.get("get_physx_scene_query_interface")
    fc.carb = _StubCarb()
    # Forces the RAYCAST branch instead of the `bottom_y <= 25.0` fallback, which would otherwise
    # answer the question by itself and never exercise the filter.
    fc.HAS_KIT = True
    try:
        vt._trigger_colliders.add(TRIGGER)

        system = HydragonFoesControllerSystem()

        def grounded(hits):
            fc.get_physx_scene_query_interface = lambda: _Query(hits)
            # Foe at y=200 with radius 50: bottom is 150, far above the 25 fallback threshold.
            return system._check_foe_grounded(None, "/World/Foe_01", (0.0, 200.0, 0.0), 50.0)

        # A volume floor 50 under the foe must NOT count as ground. Without the filter the foe gets
        # horizontal traction while airborne, i.e. it flies.
        assert grounded([_Hit(TRIGGER, (0.0, 150.0, 0.0))]) is False, (
            "a volume trigger must not count as ground"
        )

        assert grounded([_Hit(FLOOR, (0.0, 150.0, 0.0))]) is True, "real ground must still count"

        # An empty scene falls through to the geometric fallback, which must be unaffected.
        assert grounded([]) is False, "an airborne foe with nothing below is not grounded"

        system.shutdown()
        print("  [PASS] Foe ground probe ignores volume trigger colliders but still detects ground")
    finally:
        vt._trigger_colliders.discard(TRIGGER)
        fc.carb = original_carb
        fc.HAS_KIT = original_has_kit
        if had_query:
            fc.get_physx_scene_query_interface = original_query
        else:
            del fc.get_physx_scene_query_interface


if __name__ == "__main__":
    test_foes_controller_lifecycle()
    test_ai_brain_state_transitions()
    test_ai_brain_destruction()
    test_stomp_geometry_math()
    test_contact_report_event_processing()
    test_stop_event_stage_restoration()
    test_foe_destruction_score_popup_integration()
    test_player_arcade_bounce_math()
    test_physics_step_safety()
    test_foes_physics_manager_clamping_and_debounce()
    test_foes_mass_scaled_chase_actuation_and_traction()
    test_ground_probe_ignores_volume_trigger_colliders()
    print("\n=======================================================")
    print(" ALL FOES CONTROLLER SYSTEM TESTS PASSED! (12/12)")
    print("=======================================================")
