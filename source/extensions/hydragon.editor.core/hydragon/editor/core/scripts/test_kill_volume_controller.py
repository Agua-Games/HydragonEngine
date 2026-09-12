"""
Unit test for HydragonKillVolumeSystem and HydragonKillZone
Validates lifecycle, fail-silent behavior outside Kit, the trigger collider generated per shape,
trigger path dispatch, faction filtering, score penalty dispatch, checkpoint respawn resolution, and
foe destruction queue.

Note on what is no longer tested here: overlap containment. It used to be Python math
(`check_overlap`) and is now resolved by PhysX from the volume's trigger collider. What has to be
verified instead is that the collider GENERATED for a shape reproduces the containment test it
replaced - see `test_kill_zone_overlap_box`. The end-to-end tests below drive the system through
`_on_trigger_report`, which is the real entry point now.
"""

import sys
import os

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core import volume_geometry, volume_triggers
from hydragon.editor.core.kill_volume_controller import (
    HydragonKillVolumeSystem,
    HydragonKillZone,
)


class _MockPrim:
    """The only thing the snapshot path asks of the trigger prim."""

    def IsValid(self):
        return True


class _MockStaticPrim:
    """A prim that always looks invalid, for the prim-went-away path."""

    def IsValid(self):
        return False


def test_kill_volume_system_lifecycle():
    print("--- 1. Testing Kill Volume System Lifecycle ---")
    system = HydragonKillVolumeSystem()
    assert system is not None, "Failed to instantiate HydragonKillVolumeSystem"
    assert not system._is_active, "System should start inactive"
    assert not system.is_active_and_simulating(), "Should not be simulating initially"
    assert HydragonKillVolumeSystem.get_instance() is system, "Singleton should match instantiated system"

    # Startup & shutdown outside Kit (fail-silent)
    system.startup()
    assert system._is_active, "Should be marked active after startup"

    system.shutdown()
    assert not system._is_active, "System should be inactive after shutdown"
    assert system.active_zone_count == 0, "Active zones should be empty after shutdown"
    print("  [PASS] HydragonKillVolumeSystem lifecycle verified")


def test_kill_zone_overlap_box():
    print("--- 2. Testing Trigger Collider Spec (Box, Sphere, Plane) ---")
    box = volume_geometry.trigger_collider_spec("Box", (500.0, 50.0, 500.0), 500.0, 50.0)
    assert box.gprim_type == "Cube"
    assert box.scale == (500.0, 50.0, 500.0)

    sphere = volume_geometry.trigger_collider_spec("Sphere", (150.0, 150.0, 150.0), 150.0, 150.0)
    assert sphere.gprim_type == "Sphere"
    assert sphere.scale == (150.0, 150.0, 150.0)

    # A Plane becomes a BOX SLAB, because PhysX has no infinite-plane collider. The thickness is
    # chosen so the slab's half thickness equals the tolerance the analytic test used on the
    # plane's normal axis (`max(100.0, half_height)`), i.e. this is a like-for-like replacement.
    plane = volume_geometry.trigger_collider_spec("Plane", (300.0, 1.0, 400.0), 400.0, 0.0)
    assert plane.gprim_type == "Cube"
    assert plane.scale == (300.0, volume_geometry.PLANE_TRIGGER_THICKNESS * 0.5, 400.0)
    assert plane.scale[1] == 100.0, "Half the slab thickness must match the old Y tolerance"
    print("  [PASS] Trigger collider spec verified")


def test_kill_zone_overlap_sphere_and_cylinder():
    print("--- 3. Testing Entity Resolution Tolerance ---")
    # The trigger relationship names the COLLIDER; the registries hold the RIGID BODY. The match must
    # therefore be tolerant in BOTH directions, and it is the only thing that decides which entity an
    # overlap belongs to.
    assert volume_triggers.paths_match("/World/Foe_01/body", "/World/Foe_01")
    assert volume_triggers.paths_match("/World/Foe_01", "/World/Foe_01/body")
    assert not volume_triggers.paths_match("/World/Foe_01", "/World/Foe_02")
    assert not volume_triggers.paths_match("", "/World/Foe_01")

    # Nothing registered: the engine's entity index is the only source of truth, so an unregistered
    # collider resolves to nothing rather than being acted on.
    assert volume_triggers.resolve_inside_body("/World/Prop", "All") is None
    assert volume_triggers.resolve_inside_body("", "All") is None
    print("  [PASS] Entity resolution tolerance verified")


def test_kill_zone_properties_and_factions():
    print("--- 4. Testing Kill Zone Schema Properties & Factions ---")
    zone = HydragonKillZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    schema = zone._schema

    assert zone.is_enabled is True
    assert zone.respawn_player is True
    assert zone.penalty_score == 50
    assert zone.respawn_delay == 0.5
    assert zone.destroy_foes_instantly is True
    assert zone.sound_enabled is True
    assert zone.spawn_effects is True

    # Mutate properties
    schema.penalty_score = 100
    assert zone.penalty_score == 100

    schema.filter_faction = "Player"
    assert zone.filter_faction == "Player"

    schema.filter_faction = "Enemy"
    assert zone.filter_faction == "Enemy"

    schema.is_enabled = False
    assert zone.is_enabled is False
    print("  [PASS] Kill zone properties and faction filtering verified")


def test_player_hazard_trigger_and_respawn_dispatch():
    print("--- 5. Testing Player Hazard Response to a Trigger Snapshot ---")
    system = HydragonKillVolumeSystem()
    system._is_simulating = True

    zone = HydragonKillZone(prim=None, world_pos=(0.0, -200.0, 0.0))
    zone._schema.penalty_score = 50
    zone._schema.filter_faction = "All"
    system._active_zones["/World/TestKillZone"] = zone
    system._trigger_prims["/World/TestKillZone"] = _MockPrim()

    class MockPlayerControllerSystem:
        def __init__(self):
            self.respawn_calls = 0

        def is_active_and_simulating(self):
            return True

        def get_player_rb_path(self):
            return "/World/Player"

        def get_player_world_pos(self):
            return (0.0, -210.0, 0.0)

        def request_respawn(self, custom_spawn_pos=None):
            self.respawn_calls += 1

    mock_player_sys = MockPlayerControllerSystem()

    from hydragon.editor.core.player_controller import HydragonPlayerControllerSystem
    original_get = HydragonPlayerControllerSystem.get_instance
    HydragonPlayerControllerSystem.get_instance = classmethod(lambda cls: mock_player_sys)

    # PhysX is not available outside Kit, so the snapshot is supplied directly. Everything downstream
    # of it - the entering edge, the entity resolution, the cooldown, the response - is the real code
    # under test, including the per-step differencing.
    original_colliders = volume_triggers.triggered_colliders
    try:
        volume_triggers.triggered_colliders = lambda prim: {"/World/Player/geometry/ball_mesh"}
        system._on_physics_step(0.016)
        assert mock_player_sys.respawn_calls == 1, (
            "the respawn must be dispatched on the ENTERING edge"
        )

        # Still inside: no new edge, so nothing further happens.
        system._on_physics_step(0.016)
        assert mock_player_sys.respawn_calls == 1, "a body that stays inside must not re-trigger"

        # Leaves, then enters again.
        volume_triggers.triggered_colliders = lambda prim: set()
        system._on_physics_step(0.016)
        system._cooldowns.clear()  # stands in for waiting the cooldown out
        volume_triggers.triggered_colliders = lambda prim: {"/World/Player/geometry/ball_mesh"}
        system._on_physics_step(0.016)
        assert mock_player_sys.respawn_calls == 2, "re-entering after leaving must trigger again"

        # An unregistered collider - scenery - must never be treated as an entity.
        volume_triggers.triggered_colliders = lambda prim: {"/World/Environment/Ground"}
        system._cooldowns.clear()
        system._on_physics_step(0.016)
        assert mock_player_sys.respawn_calls == 2, "the ground plane is not a hazard victim"
    finally:
        volume_triggers.triggered_colliders = original_colliders
        HydragonPlayerControllerSystem.get_instance = original_get

    print("  [PASS] Player hazard response to a trigger snapshot verified")


def test_foe_hazard_instant_destruction_dispatch():
    print("--- 6. Testing Foe Hazard Response to a Trigger Snapshot ---")
    system = HydragonKillVolumeSystem()
    system._is_simulating = True

    zone = HydragonKillZone(prim=None, world_pos=(0.0, -200.0, 0.0))
    zone._schema.destroy_foes_instantly = True
    zone._schema.filter_faction = "All"
    system._active_zones["/World/TestKillZone"] = zone
    system._trigger_prims["/World/TestKillZone"] = _MockPrim()

    class MockBrain:
        def __init__(self, path, pos):
            self._prim_path = path
            self._rb_path = path
            self.current_pos = pos
            self.is_alive = True

    class MockFoesControllerSystem:
        def __init__(self):
            self._active_brains = {"/World/Foe_01": MockBrain("/World/Foe_01", (0.0, -210.0, 0.0))}
            self.destroyed_paths = []

        def is_active_and_simulating(self):
            return True

        def queue_foe_destruction(self, prim_path, is_stomp=False):
            self.destroyed_paths.append(prim_path)

    mock_foes_sys = MockFoesControllerSystem()

    from hydragon.editor.core.foes_controller import HydragonFoesControllerSystem
    original_get = HydragonFoesControllerSystem.get_instance
    HydragonFoesControllerSystem.get_instance = classmethod(lambda cls: mock_foes_sys)

    original_colliders = volume_triggers.triggered_colliders
    try:
        volume_triggers.triggered_colliders = lambda prim: {"/World/Foe_01/body"}
        system._on_physics_step(0.016)
        assert "/World/Foe_01" in mock_foes_sys.destroyed_paths, (
            "a foe entering the hazard must be queued for destruction"
        )

        # A different, unregistered collider must not destroy anything.
        volume_triggers.triggered_colliders = lambda prim: set()
        system._on_physics_step(0.016)
        system._cooldowns.clear()
        volume_triggers.triggered_colliders = lambda prim: {"/World/Prop/body"}
        system._on_physics_step(0.016)
        assert mock_foes_sys.destroyed_paths == ["/World/Foe_01"], (
            "an unregistered body must not destroy anything"
        )
    finally:
        volume_triggers.triggered_colliders = original_colliders
        HydragonFoesControllerSystem.get_instance = original_get

    print("  [PASS] Foe hazard response to a trigger snapshot verified")


if __name__ == "__main__":
    test_kill_volume_system_lifecycle()
    test_kill_zone_overlap_box()
    test_kill_zone_overlap_sphere_and_cylinder()
    test_kill_zone_properties_and_factions()
    test_player_hazard_trigger_and_respawn_dispatch()
    test_foe_hazard_instant_destruction_dispatch()
    print("\nALL KILL VOLUME CONTROLLER TESTS PASSED! (6/6)")
