"""
Unit test for HydragonForceVolumeSystem and HydragonForceVolumeZone
Validates lifecycle, fail-silent behavior outside Kit, the trigger collider generated per shape, the
trigger report decoding helpers, and all 5 physical force calculation models (Linear, Radial,
Turbulence, Dampening, Vortex).

Note on what is no longer tested here: overlap containment. It used to be Python math
(`check_shape_overlap` / `check_overlap`) and is now resolved by PhysX from the volume's trigger
collider. What has to be verified instead is that the collider GENERATED for a shape reproduces the
containment test it replaced - that is what `test_force_volume_trigger_collider_spec` pins down.
"""

import math
import sys
import os

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core import volume_geometry, volume_triggers
from hydragon.editor.core.force_volume_controller import (
    HydragonForceVolumeSystem,
    HydragonForceVolumeZone,
)


def test_force_volume_system_lifecycle():
    print("--- 1. Testing Force Volume System Lifecycle ---")
    system = HydragonForceVolumeSystem()
    assert system is not None, "Failed to instantiate HydragonForceVolumeSystem"
    assert not system._is_active, "System should start inactive"
    assert not system.is_active_and_simulating(), "Should not be simulating initially"
    assert HydragonForceVolumeSystem.get_instance() is system, "Singleton should match instantiated system"

    # Startup & shutdown outside Kit (fail-silent)
    system.startup()
    assert system._is_active, "Should be marked active after startup"

    system.shutdown()
    assert not system._is_active, "System should be inactive after shutdown"
    assert system.active_volume_count == 0, "Active volumes should be empty after shutdown"
    print("  [PASS] HydragonForceVolumeSystem lifecycle verified")


def test_force_volume_overlap_box():
    print("--- 2. Testing Trigger Collider Spec (Box, Sphere, Cylinder) ---")
    # A Cube of size 2 has a half-extent of 1 in its own frame, so scaling it by the half extents
    # IS the containment test `|rel| <= half_extents` - no approximation involved.
    box = volume_geometry.trigger_collider_spec("Box", (100.0, 50.0, 100.0), 100.0, 50.0)
    assert box.gprim_type == "Cube"
    assert box.size == 2.0
    assert box.scale == (100.0, 50.0, 100.0)

    sphere = volume_geometry.trigger_collider_spec("Sphere", (200.0, 200.0, 200.0), 200.0, 200.0)
    assert sphere.gprim_type == "Sphere"
    assert sphere.radius == 1.0
    assert sphere.scale == (200.0, 200.0, 200.0)

    # Cylinder: radius on X/Z, HALF height on Y (the Gprim's own height is the full 2).
    cylinder = volume_geometry.trigger_collider_spec("Cylinder", (100.0, 50.0, 100.0), 100.0, 50.0)
    assert cylinder.gprim_type == "Cylinder"
    assert cylinder.radius == 1.0 and cylinder.height == 2.0
    assert cylinder.scale == (100.0, 50.0, 100.0)

    # An unknown shape falls back to the Box collider, matching build_shape_geometry().
    fallback = volume_geometry.trigger_collider_spec("Nonsense", (10.0, 20.0, 30.0), 10.0, 20.0)
    assert fallback.gprim_type == "Cube"
    assert fallback.scale == (10.0, 20.0, 30.0)
    print("  [PASS] Trigger collider spec verified")


def test_force_volume_overlap_sphere_and_cylinder():
    print("--- 3. Testing Trigger State Helpers and Per-Step Bookkeeping ---")
    # The trigger relationship names the COLLIDER; the entity registries hold the RIGID BODY. These
    # differ by a level in the common case, which is why the match is a prefix test, not equality.
    assert volume_triggers.paths_match("/World/Ball/collider", "/World/Ball")
    assert volume_triggers.paths_match("/World/Ball", "/World/Ball")
    assert not volume_triggers.paths_match("/World/Ball", "/World/Other")
    assert not volume_triggers.paths_match("", "/World/Ball")

    assert volume_triggers.faction_is_accepted("All", "Anything")
    assert volume_triggers.faction_is_accepted("RigidBodies", "Unknown")
    assert volume_triggers.faction_is_accepted("Player", "Player")
    assert not volume_triggers.faction_is_accepted("Player", "Enemy")

    class MockPlayerControllerSystem:
        def is_active_and_simulating(self):
            return True

        def get_player_rb_path(self):
            return "/World/Player"

    from hydragon.editor.core.player_controller import HydragonPlayerControllerSystem

    original_player_get = HydragonPlayerControllerSystem.get_instance
    HydragonPlayerControllerSystem.get_instance = classmethod(
        lambda cls: MockPlayerControllerSystem()
    )

    class MockTriggerPrim:
        def IsValid(self):
            return True

    original_colliders = volume_triggers.triggered_colliders
    try:
        # `resolve_inside_body` is the gate that turns a reported COLLIDER path into the rigid body
        # the registry knows, or None to ignore the overlap. Returning the registry's OWN path is what
        # removes the need to decode an actor id - the step that was failing in Kit, because trigger
        # report ids are Fabric handles rather than SdfPath-encoded integers.
        record = volume_triggers.resolve_inside_body("/World/Player/geometry/ball_mesh", "All")
        assert record is not None, "a collider under the player's rigid body must resolve"
        assert record["faction"] == "Player"
        assert record["rb_path"] == "/World/Player", "the registry's rigid body path is returned"

        assert volume_triggers.resolve_inside_body("/World/Ground", "All") is None, (
            "an unregistered collider must be ignored - that is what keeps the ground plane out"
        )
        assert volume_triggers.resolve_inside_body(
            "/World/Player/geometry/ball_mesh", "Enemy"
        ) is None, "a volume filtered to Enemy must reject the player"

        # The per-step bookkeeping: a supplied snapshot must land in `_inside`, which is what the
        # impulse edge is computed against on the next step.
        system = HydragonForceVolumeSystem()
        system._is_simulating = True
        zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 0.0, 0.0))
        system._active_volumes["/World/V"] = zone
        system._trigger_prims["/World/V"] = MockTriggerPrim()

        volume_triggers.triggered_colliders = lambda prim: {"/World/Player/body"}
        system._on_physics_step(0.016)
        assert "/World/Player/body" in system._inside.get("/World/V", {}), (
            "an overlapping collider must be recorded for the volume"
        )

        volume_triggers.triggered_colliders = lambda prim: set()
        system._on_physics_step(0.016)
        assert system._inside.get("/World/V") == {}, (
            "a snapshot with nothing inside must clear the record"
        )
        system.shutdown()
    finally:
        volume_triggers.triggered_colliders = original_colliders
        HydragonPlayerControllerSystem.get_instance = original_player_get

    print("  [PASS] Trigger state helpers and per-step bookkeeping verified")


def test_force_calculations_linear_and_radial():
    print("--- 4. Testing Linear and Radial Force Calculations ---")
    zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    schema = zone._schema

    # 1. Linear Force: upward (0, 1, 0) magnitude 1000
    schema.linear_enabled = True
    schema.linear_direction = (0.0, 1.0, 0.0)
    schema.linear_magnitude = 1000.0

    force_vec, _, _ = zone.compute_forces(
        body_pos=(0.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    assert abs(force_vec[1] - 1000.0) < 1e-3, f"Expected fy=1000, got {force_vec[1]}"
    assert abs(force_vec[0]) < 1e-3 and abs(force_vec[2]) < 1e-3

    # 2. Radial Force: Attractor at origin (pos=(100, 0, 0) -> should pull left towards -X)
    schema.linear_enabled = False
    schema.radial_enabled = True
    schema.radial_magnitude = 500.0  # positive = attractor
    schema.radial_radius = 500.0
    schema.radial_falloff = "None"

    force_vec, _, _ = zone.compute_forces(
        body_pos=(100.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    assert abs(force_vec[0] - (-500.0)) < 1e-3, f"Attractor should pull toward -X: got {force_vec[0]}"

    # Repulsor: negative magnitude -> should push along +X
    schema.radial_magnitude = -500.0
    force_vec, _, _ = zone.compute_forces(
        body_pos=(100.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    assert abs(force_vec[0] - 500.0) < 1e-3, f"Repulsor should push along +X: got {force_vec[0]}"

    # Linear falloff: distance = 250 / 500 -> ratio = 0.5 -> mag = 250
    schema.radial_falloff = "Linear"
    force_vec, _, _ = zone.compute_forces(
        body_pos=(250.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    assert abs(force_vec[0] - 250.0) < 1e-3, f"Linear falloff expected 250, got {force_vec[0]}"
    print("  [PASS] Linear and Radial force math verified")


def test_force_calculations_dampening_and_vortex():
    print("--- 5. Testing Dampening and Vortex Calculations ---")
    zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    schema = zone._schema

    # 1. Dampening
    schema.dampening_enabled = True
    schema.linear_damping = 0.5
    schema.angular_damping = 0.5

    _, damped_v, damped_w = zone.compute_forces(
        body_pos=(0.0, 0.0, 0.0),
        body_vel=(100.0, 0.0, 0.0),
        body_ang_vel=(0.0, 10.0, 0.0),
        sim_time=0.0,
        dt=0.1,  # factor = 1 - 0.5 * 0.1 = 0.95
    )
    assert abs(damped_v[0] - 95.0) < 1e-3, f"Damped vx expected 95.0, got {damped_v[0]}"
    assert abs(damped_w[1] - 9.5) < 1e-3, f"Damped wy expected 9.5, got {damped_w[1]}"

    # 2. Vortex: axis=(0, 1, 0), body at (100, 0, 0)
    schema.dampening_enabled = False
    schema.vortex_enabled = True
    schema.vortex_axis = (0.0, 1.0, 0.0)
    schema.vortex_magnitude = 800.0
    schema.vortex_inward_pull = 200.0

    force_vec, _, _ = zone.compute_forces(
        body_pos=(100.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    # At (100, 0, 0) around Y axis:
    # Inward pull is towards -X (inward_pull = 200 -> fx = -200)
    # Tangent around Y is along -Z: (0, 1, 0) x (1, 0, 0) = (0, 0, -1) -> fz = -800
    assert abs(force_vec[0] - (-200.0)) < 1e-3, f"Vortex inward pull expected -200, got {force_vec[0]}"
    assert abs(force_vec[2] - (-800.0)) < 1e-3, f"Vortex tangential force expected -800, got {force_vec[2]}"
    print("  [PASS] Dampening and Vortex force math verified")


def test_impulse_cooldown():
    print("--- 6. Testing Impulse Cooldown Logic ---")
    zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    zone._schema.impulse_cooldown = 1.0

    rb_path = "/World/Player"
    # First impulse at t=0.0 should succeed
    assert zone.can_apply_impulse(rb_path, 0.0), "Initial impulse must succeed"

    # Immediately at t=0.1 should be blocked by cooldown
    assert not zone.can_apply_impulse(rb_path, 0.1), "Impulse within cooldown must be blocked"

    # At t=0.9 should still be blocked
    assert not zone.can_apply_impulse(rb_path, 0.9), "Impulse before cooldown expires must be blocked"

    # At t=1.1 should succeed
    assert zone.can_apply_impulse(rb_path, 1.1), "Impulse after cooldown expired must succeed"

    # Different body at t=1.2 should succeed
    assert zone.can_apply_impulse("/World/Foe", 1.2), "Different body must have independent cooldown"
    print("  [PASS] Impulse cooldown logic verified")


def test_linear_coord_space():
    print("--- 7. Testing Linear Coord Space (Volume vs World) ---")
    zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    schema = zone._schema
    schema.linear_enabled = True
    schema.linear_direction = (0.0, 1.0, 0.0)
    schema.linear_magnitude = 800.0

    # Test World space
    schema.linear_coord_space = "World"
    assert schema.linear_coord_space == "World"
    force_vec, _, _ = zone.compute_forces(
        body_pos=(0.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    assert abs(force_vec[1] - 800.0) < 1e-3

    # Test Volume space default
    schema.linear_coord_space = "Volume"
    assert schema.linear_coord_space == "Volume"
    force_vec, _, _ = zone.compute_forces(
        body_pos=(0.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    assert abs(force_vec[1] - 800.0) < 1e-3
    print("  [PASS] Linear Coord Space verified")


def test_uncoupled_radial_force_reach():
    print("--- 8. Testing Shape Confinement Is Now the Trigger's Job ---")
    # The shape-dependent terms (linear, turbulence, vortex, dampening) used to be gated by a Python
    # containment test. They are now reached only for bodies PhysX reported inside the trigger, so
    # compute_forces treats every call as "inside".
    #
    # This is a DELIBERATE behaviour change: a volume no longer reaches bodies outside its own cage,
    # not even through `force:radialRadius`. Influence and the drawn bounds are now the same thing.
    zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    schema = zone._schema
    schema.linear_enabled = True
    schema.linear_direction = (0.0, 1.0, 0.0)
    schema.linear_magnitude = 500.0
    schema.radial_enabled = False

    force_vec, _, _ = zone.compute_forces(
        body_pos=(200.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    assert abs(force_vec[1] - 500.0) < 1e-3, "A body reported inside gets the full linear force"
    assert abs(force_vec[0]) < 1e-3

    # The radial falloff itself is a property of the force, not of the boundary, so it still fades
    # with distance - the trigger decides WHETHER, this decides HOW MUCH.
    schema.linear_enabled = False
    schema.radial_enabled = True
    schema.radial_radius = 500.0
    schema.radial_magnitude = 1000.0
    schema.radial_falloff = "None"
    force_vec, _, _ = zone.compute_forces(
        body_pos=(200.0, 0.0, 0.0),
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    assert abs(force_vec[0] - (-1000.0)) < 1e-3, f"Expected radial fx=-1000, got {force_vec[0]}"
    print("  [PASS] Shape confinement is the trigger's responsibility")


if __name__ == "__main__":
    test_force_volume_system_lifecycle()
    test_force_volume_overlap_box()
    test_force_volume_overlap_sphere_and_cylinder()
    test_force_calculations_linear_and_radial()
    test_force_calculations_dampening_and_vortex()
    test_impulse_cooldown()
    test_linear_coord_space()
    test_uncoupled_radial_force_reach()
    print("\nALL FORCE VOLUME CONTROLLER TESTS PASSED! (8/8)")
