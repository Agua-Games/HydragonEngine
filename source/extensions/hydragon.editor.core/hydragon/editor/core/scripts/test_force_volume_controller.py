"""
Unit test for HydragonForceVolumeSystem and HydragonForceVolumeZone
Validates lifecycle, fail-silent behavior outside Kit, spatial overlap math (Box, Sphere, Cylinder),
and all 5 physical force calculation models (Linear, Radial, Turbulence, Dampening, Vortex).
"""

import math
import sys
import os

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

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
    print("--- 2. Testing Box Overlap Math ---")
    zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 100.0, 0.0))
    zone.half_extents = (100.0, 50.0, 100.0)

    # Point at center
    assert zone.check_overlap((0.0, 100.0, 0.0))

    # Point within box
    assert zone.check_overlap((50.0, 120.0, -40.0))

    # Point on boundary
    assert zone.check_overlap((100.0, 100.0, 100.0))

    # Point outside X
    assert not zone.check_overlap((110.0, 100.0, 0.0))

    # Point outside Y
    assert not zone.check_overlap((0.0, 160.0, 0.0))

    # Point outside Z
    assert not zone.check_overlap((0.0, 100.0, -105.0))
    print("  [PASS] Box overlap math verified")


def test_force_volume_overlap_sphere_and_cylinder():
    print("--- 3. Testing Sphere and Cylinder Overlap Math ---")
    # Sphere test
    zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    zone._schema.volume_shape = "Sphere"
    zone.radius = 200.0

    assert zone.check_overlap((0.0, 0.0, 0.0))
    assert zone.check_overlap((100.0, 100.0, 100.0))  # dist ~ 173.2 <= 200
    assert not zone.check_overlap((150.0, 150.0, 150.0))  # dist ~ 259.8 > 200

    # Cylinder test (radius=100, half_height=50 along Y)
    zone._schema.volume_shape = "Cylinder"
    zone.radius = 100.0
    zone.half_height = 50.0

    assert zone.check_overlap((0.0, 0.0, 0.0))
    assert zone.check_overlap((60.0, 30.0, 60.0))  # planar ~ 84.85 <= 100, y=30 <= 50
    assert not zone.check_overlap((90.0, 30.0, 90.0))  # planar ~ 127.2 > 100
    assert not zone.check_overlap((0.0, 60.0, 0.0))  # y=60 > 50
    print("  [PASS] Sphere and Cylinder overlap math verified")


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


if __name__ == "__main__":
    test_force_volume_system_lifecycle()
    test_force_volume_overlap_box()
    test_force_volume_overlap_sphere_and_cylinder()
    test_force_calculations_linear_and_radial()
    test_force_calculations_dampening_and_vortex()
    test_impulse_cooldown()
    print("\nALL FORCE VOLUME CONTROLLER TESTS PASSED! (6/6)")
