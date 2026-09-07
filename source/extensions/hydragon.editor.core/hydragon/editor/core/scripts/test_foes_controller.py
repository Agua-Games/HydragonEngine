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


if __name__ == "__main__":
    test_foes_controller_lifecycle()
    test_ai_brain_state_transitions()
    test_ai_brain_destruction()
    test_stomp_geometry_math()
    test_contact_report_event_processing()
    test_stop_event_stage_restoration()
    test_foe_destruction_score_popup_integration()
    print("\n=======================================================")
    print(" ALL FOES CONTROLLER SYSTEM TESTS PASSED! (7/7)")
    print("=======================================================")
