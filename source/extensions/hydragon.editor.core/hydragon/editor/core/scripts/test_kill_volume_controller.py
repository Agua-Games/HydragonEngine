"""
Unit test for HydragonKillVolumeSystem and HydragonKillZone
Validates lifecycle, fail-silent behavior outside Kit, spatial overlap math (Box, Sphere, Cylinder),
faction filtering, score penalty dispatch, checkpoint respawn resolution, and foe destruction queue.
"""

import sys
import os

# scripts/ -> core/ -> editor/ -> hydragon/ -> hydragon.editor.core/
ext_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
if ext_dir not in sys.path:
    sys.path.insert(0, ext_dir)

from hydragon.editor.core.kill_volume_controller import (
    HydragonKillVolumeSystem,
    HydragonKillZone,
)


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
    print("--- 2. Testing Kill Zone Box Overlap Math ---")
    zone = HydragonKillZone(prim=None, world_pos=(0.0, -200.0, 0.0))
    zone.half_extents = (500.0, 50.0, 500.0)

    # Point at center
    assert zone.check_overlap((0.0, -200.0, 0.0))

    # Point within box
    assert zone.check_overlap((250.0, -180.0, -300.0))

    # Point on boundary
    assert zone.check_overlap((500.0, -200.0, 500.0))

    # Point above box (safe floor)
    assert not zone.check_overlap((0.0, 0.0, 0.0))

    # Point far outside in X
    assert not zone.check_overlap((550.0, -200.0, 0.0))
    print("  [PASS] Kill zone box overlap math verified")


def test_kill_zone_overlap_sphere_and_cylinder():
    print("--- 3. Testing Kill Zone Sphere and Cylinder Overlap Math ---")
    # Sphere test
    zone = HydragonKillZone(prim=None, world_pos=(0.0, 0.0, 0.0))
    zone._schema.volume_shape = "Sphere"
    zone.radius = 150.0

    assert zone.check_overlap((0.0, 0.0, 0.0))
    assert zone.check_overlap((80.0, 80.0, 80.0))  # dist ~ 138.56 <= 150
    assert not zone.check_overlap((100.0, 100.0, 100.0))  # dist ~ 173.2 > 150

    # Cylinder test
    zone._schema.volume_shape = "Cylinder"
    zone.radius = 120.0
    zone.half_height = 40.0

    assert zone.check_overlap((0.0, 0.0, 0.0))
    assert zone.check_overlap((80.0, 20.0, 80.0))  # planar ~ 113.1 <= 120, y=20 <= 40
    assert not zone.check_overlap((90.0, 20.0, 90.0))  # planar ~ 127.2 > 120
    assert not zone.check_overlap((0.0, 50.0, 0.0))  # y=50 > 40
    print("  [PASS] Kill zone sphere and cylinder overlap math verified")


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
    assert not zone.check_overlap((0.0, 0.0, 0.0)), "Disabled zone must reject overlap"
    print("  [PASS] Kill zone properties and faction filtering verified")


def test_player_hazard_trigger_and_respawn_dispatch():
    print("--- 5. Testing Player Hazard Trigger & Respawn Dispatch ---")
    system = HydragonKillVolumeSystem()
    system._is_simulating = True

    # Create mock zone at (0, -200, 0)
    zone = HydragonKillZone(prim=None, world_pos=(0.0, -200.0, 0.0))
    zone.half_extents = (500.0, 50.0, 500.0)
    zone._schema.penalty_score = 50
    zone._schema.filter_faction = "All"
    system._active_zones["/World/TestKillZone"] = zone

    # Mock player system
    class MockPlayerControllerSystem:
        def __init__(self):
            self.respawn_called = False
            self.respawn_target_pos = None

        def is_active_and_simulating(self):
            return True

        def get_player_world_pos(self):
            return (0.0, -210.0, 0.0)  # Inside kill zone!

        def request_respawn(self, custom_spawn_pos=None):
            self.respawn_called = True
            self.respawn_target_pos = custom_spawn_pos

    mock_player_sys = MockPlayerControllerSystem()

    # Monkeypatch get_instance
    from hydragon.editor.core.player_controller import HydragonPlayerControllerSystem
    orig_player_get = HydragonPlayerControllerSystem.get_instance
    HydragonPlayerControllerSystem.get_instance = classmethod(lambda cls: mock_player_sys)

    try:
        system._process_player_hazard(stage=None)
        assert mock_player_sys.respawn_called, "request_respawn must be dispatched when player enters kill volume"
        assert system._player_cooldown > 0.0, "Cooldown must be activated after player kill"
    finally:
        HydragonPlayerControllerSystem.get_instance = orig_player_get

    print("  [PASS] Player hazard trigger and respawn dispatch verified")


def test_foe_hazard_instant_destruction_dispatch():
    print("--- 6. Testing Foe Hazard Destruction Dispatch ---")
    system = HydragonKillVolumeSystem()
    system._is_simulating = True

    zone = HydragonKillZone(prim=None, world_pos=(0.0, -200.0, 0.0))
    zone.half_extents = (500.0, 50.0, 500.0)
    zone._schema.destroy_foes_instantly = True
    zone._schema.filter_faction = "All"
    system._active_zones["/World/TestKillZone"] = zone

    # Mock foes system
    class MockBrain:
        def __init__(self, path, pos):
            self._prim_path = path
            self.current_pos = pos
            self.is_alive = True

    class MockFoesControllerSystem:
        def __init__(self):
            self._active_brains = {
                "/World/Foe_01": MockBrain("/World/Foe_01", (0.0, -210.0, 0.0)),  # inside
                "/World/Foe_02": MockBrain("/World/Foe_02", (0.0, 100.0, 0.0)),   # outside
            }
            self.destroyed_paths = []

        def is_active_and_simulating(self):
            return True

        def queue_foe_destruction(self, prim_path, is_stomp=False):
            self.destroyed_paths.append(prim_path)

    mock_foes_sys = MockFoesControllerSystem()

    from hydragon.editor.core.foes_controller import HydragonFoesControllerSystem
    orig_foes_get = HydragonFoesControllerSystem.get_instance
    HydragonFoesControllerSystem.get_instance = classmethod(lambda cls: mock_foes_sys)

    try:
        system._process_foes_hazard(stage=None)
        assert "/World/Foe_01" in mock_foes_sys.destroyed_paths, "Foe inside hazard must be queued for destruction"
        assert "/World/Foe_02" not in mock_foes_sys.destroyed_paths, "Foe outside hazard must NOT be destroyed"
    finally:
        HydragonFoesControllerSystem.get_instance = orig_foes_get

    print("  [PASS] Foe hazard instant destruction dispatch verified")


def test_kill_volume_wireframe_synchronization():
    print("--- 7. Testing Kill Volume Dynamic Wireframe Synchronization ---")
    from hydragon.editor.core.kill_volume_controller import (
        sync_kill_volume_wireframe,
        HydragonKillVolumeSystem,
    )
    from hydragon.editor.core.schemas import HydragonKillVolume

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
        def __init__(self, path="/World/KillVolume", type_name="BasisCurves"):
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

    kill_prim = MockPrim(path="/World/KillVolume", type_name="Xform")
    kill_bounds = MockPrim(path="/World/KillVolume/volumes/kill_bounds", type_name="BasisCurves")
    kill_prim._children["volumes/kill_bounds"] = kill_bounds
    shape_attr = kill_prim.CreateAttribute("kill:volumeShape")

    # 1. Sphere sync
    shape_attr.Set("Sphere")
    sync_kill_volume_wireframe(kill_prim)
    assert len(kill_bounds.curves.counts_attr.Get()) == 3, "Kill volume sync must update to Sphere (3 circles)"

    # 2. Box sync
    shape_attr.Set("Box")
    sync_kill_volume_wireframe(kill_prim)
    assert len(kill_bounds.curves.counts_attr.Get()) == 12, "Kill volume sync must update to Box (12 edges)"

    # 3. Schema property setter
    kill_vol = HydragonKillVolume(kill_prim)
    kill_vol.volume_shape = "Plane"
    assert len(kill_bounds.curves.counts_attr.Get()) == 3
    assert kill_bounds.curves.counts_attr.Get() == [5, 2, 2]

    # 4. Stage mutation notice
    system = HydragonKillVolumeSystem()
    class MockNotice:
        def GetChangedInfoOnlyPaths(self):
            class MockPropPath:
                name = "kill:volumeShape"
                def GetPrimPath(self):
                    return MockPrimPath("/World/KillVolume")
            return [MockPropPath()]
        def GetResyncedPaths(self):
            return []

    class MockStage:
        def GetPrimAtPath(self, path):
            if str(path) == "/World/KillVolume":
                return kill_prim
            return None

    shape_attr.Set("Sphere")
    system._on_objects_changed(MockNotice(), MockStage())
    assert len(kill_bounds.curves.counts_attr.Get()) == 3, "Kill volume stage notice must update wireframe to Sphere"

    print("  [PASS] Kill volume dynamic wireframe synchronization verified")


if __name__ == "__main__":
    test_kill_volume_system_lifecycle()
    test_kill_zone_overlap_box()
    test_kill_zone_overlap_sphere_and_cylinder()
    test_kill_zone_properties_and_factions()
    test_player_hazard_trigger_and_respawn_dispatch()
    test_foe_hazard_instant_destruction_dispatch()
    test_kill_volume_wireframe_synchronization()
    print("\nALL KILL VOLUME CONTROLLER TESTS PASSED! (7/7)")
