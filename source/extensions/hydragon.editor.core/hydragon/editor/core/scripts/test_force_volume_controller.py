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
    print("--- 8. Testing Uncoupled Radial Force Reach ---")
    # Box is small: half extents = 50 (extends from -50 to +50)
    zone = HydragonForceVolumeZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    zone.half_extents = (50.0, 50.0, 50.0)
    schema = zone._schema

    # Both linear and radial enabled
    schema.linear_enabled = True
    schema.linear_direction = (0.0, 1.0, 0.0)
    schema.linear_magnitude = 500.0

    schema.radial_enabled = True
    schema.radial_radius = 500.0
    schema.radial_magnitude = 1000.0
    schema.radial_falloff = "None"

    # Point at (200, 0, 0) is OUTSIDE the box, but INSIDE the radial radius (500)
    outside_box_point = (200.0, 0.0, 0.0)
    assert not zone.check_shape_overlap(outside_box_point), "Point should be outside the box shape"
    assert zone.check_overlap(outside_box_point), "Point must be inside the uncoupled radial zone"

    force_vec, _, _ = zone.compute_forces(
        body_pos=outside_box_point,
        body_vel=(0.0, 0.0, 0.0),
        body_ang_vel=(0.0, 0.0, 0.0),
        sim_time=0.0,
        dt=0.016,
    )
    # Radial force pulls toward origin (-X): fx = -1000
    assert abs(force_vec[0] - (-1000.0)) < 1e-3, f"Expected radial fx=-1000, got {force_vec[0]}"
    # Linear force is shape-confined to the box, so fy should be 0 outside the box!
    assert abs(force_vec[1]) < 1e-3, f"Linear force should not leak outside the box, got fy={force_vec[1]}"

    # Point at (600, 0, 0) is outside BOTH the box and radial reach
    way_outside = (600.0, 0.0, 0.0)
    assert not zone.check_overlap(way_outside)
    print("  [PASS] Uncoupled radial reach and shape-confined forces verified")


def test_wireframe_synchronization():
    print("--- 9. Testing Dynamic Wireframe Guide Synchronization ---")
    from hydragon.editor.core.force_volume_controller import (
        update_wireframe_guide,
        sync_force_volume_wireframe,
    )
    from hydragon.editor.core.kill_volume_controller import (
        sync_kill_volume_wireframe,
    )
    from hydragon.editor.core.schemas import (
        HydragonForceVolume,
        HydragonKillVolume,
    )

    class MockAttr:
        def __init__(self, val=None):
            self._val = val
        def IsValid(self):
            return True
        def Get(self):
            return self._val
        def Set(self, val):
            self._val = val
        def SetMetadata(self, k, v):
            pass

    class MockCurves:
        def __init__(self):
            self.type_attr = MockAttr()
            self.wrap_attr = MockAttr()
            self.counts_attr = MockAttr()
            self.points_attr = MockAttr()
            self.extent_attr = MockAttr()
            self.widths_attr = MockAttr()
            self.color_attr = MockAttr()

        def GetTypeAttr(self): return self.type_attr
        def CreateTypeAttr(self): return self.type_attr
        def GetWrapAttr(self): return self.wrap_attr
        def CreateWrapAttr(self): return self.wrap_attr
        def GetCurveVertexCountsAttr(self): return self.counts_attr
        def CreateCurveVertexCountsAttr(self): return self.counts_attr
        def GetPointsAttr(self): return self.points_attr
        def CreatePointsAttr(self): return self.points_attr
        def GetExtentAttr(self): return self.extent_attr
        def CreateExtentAttr(self): return self.extent_attr
        def GetWidthsAttr(self): return self.widths_attr
        def CreateWidthsAttr(self): return self.widths_attr
        def GetDisplayColorAttr(self): return self.color_attr
        def CreateDisplayColorAttr(self): return self.color_attr

    class MockPrimPath:
        def __init__(self, path: str):
            self.pathString = path
            self.name = path.split("/")[-1]
        def GetPrimPath(self):
            return self
        def IsPrimPath(self):
            return True
        def __str__(self):
            return self.pathString

    class MockPrim:
        def __init__(self, path="/World/ForceVolume", type_name="BasisCurves"):
            self._path = path
            self._type_name = type_name
            self._attrs = {}
            self.curves = MockCurves()
            self._children = {}

        def IsValid(self):
            return True
        def GetTypeName(self):
            return self._type_name
        def SetTypeName(self, t):
            self._type_name = t
        def HasAttribute(self, name):
            return name in self._attrs
        def GetAttribute(self, name):
            return self._attrs.get(name)
        def CreateAttribute(self, name, *args):
            attr = MockAttr()
            self._attrs[name] = attr
            return attr
        def GetPrimAtPath(self, rel_path):
            return self._children.get(rel_path)
        def GetPath(self):
            return MockPrimPath(self._path)

    # 1. Test update_wireframe_guide with all 4 supported shapes
    bounds = MockPrim(path="/World/ForceVolume/volumes/force_bounds")
    
    # Sphere wireframe: 3 circles (XY, XZ, YZ), 33 points each
    update_wireframe_guide(bounds, shape="Sphere", radius=100.0)
    counts = bounds.curves.counts_attr.Get()
    points = bounds.curves.points_attr.Get()
    assert len(counts) == 3, f"Expected 3 curve counts for Sphere, got {len(counts)}"
    assert all(c == 33 for c in counts), "Each circle should have 33 vertices"
    assert len(points) == 99, f"Expected 99 points for Sphere, got {len(points)}"

    # Cylinder wireframe: 2 rings (33 points each) + 4 struts (2 points each) = 6 curves, 74 points
    update_wireframe_guide(bounds, shape="Cylinder", radius=80.0, half_height=50.0)
    counts = bounds.curves.counts_attr.Get()
    points = bounds.curves.points_attr.Get()
    assert len(counts) == 6, f"Expected 6 curve counts for Cylinder, got {len(counts)}"
    assert counts == [33, 33, 2, 2, 2, 2]
    assert len(points) == 74, f"Expected 74 points for Cylinder, got {len(points)}"

    # Box wireframe: 12 edges, 2 points each = 24 points
    update_wireframe_guide(bounds, shape="Box", half_extents=(100.0, 50.0, 100.0))
    counts = bounds.curves.counts_attr.Get()
    points = bounds.curves.points_attr.Get()
    assert len(counts) == 12, f"Expected 12 curve counts for Box, got {len(counts)}"
    assert all(c == 2 for c in counts)
    assert len(points) == 24, f"Expected 24 points for Box, got {len(points)}"

    # Plane wireframe: 1 perimeter (5 points) + 2 diagonals (2 points each) = 3 curves, 9 points
    update_wireframe_guide(bounds, shape="Plane", half_extents=(200.0, 0.0, 200.0))
    counts = bounds.curves.counts_attr.Get()
    points = bounds.curves.points_attr.Get()
    assert len(counts) == 3, f"Expected 3 curve counts for Plane, got {len(counts)}"
    assert counts == [5, 2, 2]
    assert len(points) == 9, f"Expected 9 points for Plane, got {len(points)}"

    # 2. Test sync_force_volume_wireframe hierarchy traversal
    vol_prim = MockPrim(path="/World/ForceVolume", type_name="Xform")
    vol_bounds = MockPrim(path="/World/ForceVolume/volumes/force_bounds", type_name="BasisCurves")
    vol_prim._children["volumes/force_bounds"] = vol_bounds
    shape_attr = vol_prim.CreateAttribute("force:volumeShape")
    
    shape_attr.Set("Sphere")
    sync_force_volume_wireframe(vol_prim)
    assert len(vol_bounds.curves.counts_attr.Get()) == 3, "sync_force_volume_wireframe must update to Sphere"

    shape_attr.Set("Box")
    sync_force_volume_wireframe(vol_prim)
    assert len(vol_bounds.curves.counts_attr.Get()) == 12, "sync_force_volume_wireframe must update to Box"

    # 3. Test HydragonForceVolume schema property setter triggering sync
    force_vol = HydragonForceVolume(vol_prim)
    force_vol.volume_shape = "Cylinder"
    assert len(vol_bounds.curves.counts_attr.Get()) == 6, "Setting schema.volume_shape must trigger wireframe sync"

    # 4. Test stage mutation notice handler _on_objects_changed
    system = HydragonForceVolumeSystem()
    class MockNotice:
        def GetChangedInfoOnlyPaths(self):
            class MockPropPath:
                name = "force:volumeShape"
                def GetPrimPath(self):
                    return MockPrimPath("/World/ForceVolume")
            return [MockPropPath()]
        def GetResyncedPaths(self):
            return []

    class MockStage:
        def GetPrimAtPath(self, path):
            if str(path) == "/World/ForceVolume":
                return vol_prim
            return None

    shape_attr.Set("Sphere")
    system._on_objects_changed(MockNotice(), MockStage())
    assert len(vol_bounds.curves.counts_attr.Get()) == 3, "Stage notice must trigger wireframe sync to Sphere"

    # 5. Test sync_kill_volume_wireframe and schema setter
    kill_prim = MockPrim(path="/World/KillVolume", type_name="Xform")
    kill_bounds = MockPrim(path="/World/KillVolume/volumes/kill_bounds", type_name="BasisCurves")
    kill_prim._children["volumes/kill_bounds"] = kill_bounds
    kill_shape_attr = kill_prim.CreateAttribute("kill:volumeShape")
    
    kill_shape_attr.Set("Sphere")
    sync_kill_volume_wireframe(kill_prim)
    assert len(kill_bounds.curves.counts_attr.Get()) == 3, "Kill volume sync must update to Sphere"

    kill_vol = HydragonKillVolume(kill_prim)
    kill_vol.volume_shape = "Plane"
    assert len(kill_bounds.curves.counts_attr.Get()) == 3, "Setting HydragonKillVolume.volume_shape must trigger wireframe sync"
    assert kill_bounds.curves.counts_attr.Get() == [5, 2, 2]

    print("  [PASS] Dynamic wireframe guide synchronization verified")


if __name__ == "__main__":
    test_force_volume_system_lifecycle()
    test_force_volume_overlap_box()
    test_force_volume_overlap_sphere_and_cylinder()
    test_force_calculations_linear_and_radial()
    test_force_calculations_dampening_and_vortex()
    test_impulse_cooldown()
    test_linear_coord_space()
    test_uncoupled_radial_force_reach()
    test_wireframe_synchronization()
    print("\nALL FORCE VOLUME CONTROLLER TESTS PASSED! (9/9)")

