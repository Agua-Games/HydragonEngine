"""
Hydragon Engine - Trigger and Level Goal Physics Controller System

Component-driven trigger volume system using OpenUSD API schemas
(HydragonTriggerAPI, HydragonGameAPI), PhysX physics simulation, and
agnostic entity tracking for victory and gameplay events.
"""

import math
from typing import Dict, List, Optional, Tuple

try:
    import carb
    import omni.kit.app
    import omni.timeline
    import omni.usd
    import omni.physx
    from omni.physx import get_physx_interface
    from pxr import Usd, UsdGeom, Sdf, Gf
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None

from .schemas import HydragonTrigger, HydragonGameManager, HydragonPlayerController


def _is_rigid_body(prim) -> bool:
    """Checks whether a prim has rigid body physics enabled using multiple schema inspection techniques."""
    if not prim or not hasattr(prim, "IsValid") or not prim.IsValid():
        return False

    # 1. Attribute check (fastest and most standard in OpenUSD physics)
    if prim.HasAttribute("physics:rigidBodyEnabled"):
        val = prim.GetAttribute("physics:rigidBodyEnabled").Get()
        return bool(val) if val is not None else True

    # 2. Applied schemas check
    if hasattr(prim, "GetAppliedSchemas"):
        applied = prim.GetAppliedSchemas() or []
        if "PhysicsRigidBodyAPI" in applied or "PhysxRigidBodyAPI" in applied:
            return True

    # 3. String token HasAPI check
    try:
        if prim.HasAPI("PhysicsRigidBodyAPI") or prim.HasAPI("PhysxRigidBodyAPI"):
            return True
    except Exception:
        pass

    return False


def _find_rigid_body_descendant(prim) -> Optional[object]:
    """Locates the prim with rigid body physics, checking root, standard subpaths, and descendants."""
    if not prim or not hasattr(prim, "IsValid") or not prim.IsValid():
        return None
    if _is_rigid_body(prim):
        return prim
    try:
        mesh_prim = prim.GetPrimAtPath("geometry/ball_mesh")
        if mesh_prim and mesh_prim.IsValid() and _is_rigid_body(mesh_prim):
            return mesh_prim
    except Exception:
        pass
    for child in prim.GetChildren():
        if _is_rigid_body(child):
            return child
    for child in prim.GetAllChildren():
        rb = _find_rigid_body_descendant(child)
        if rb:
            return rb
    return None


class HydragonTriggerZone:
    """
    Component class representing an individual trigger volume entity.
    Encapsulates spatial bounds, faction filtering, and one-shot state.
    """

    def __init__(
        self,
        prim,
        world_pos: Tuple[float, float, float] = (0.0, 0.0, 0.0),
        radius: float = 75.0,
        half_height: float = 50.0,
    ):
        self._prim = prim
        self._prim_path: str = prim.GetPath().pathString if prim and hasattr(prim, "GetPath") else ""
        self._schema: Optional[HydragonTrigger] = HydragonTrigger(prim) if prim else None

        self._world_pos: Tuple[float, float, float] = world_pos
        self._radius: float = radius
        self._half_height: float = half_height
        self._triggered: bool = False

    @property
    def prim_path(self) -> str:
        return self._prim_path

    @property
    def world_pos(self) -> Tuple[float, float, float]:
        return self._world_pos

    @world_pos.setter
    def world_pos(self, pos: Tuple[float, float, float]):
        self._world_pos = pos

    @property
    def radius(self) -> float:
        return self._radius

    @radius.setter
    def radius(self, val: float):
        self._radius = val

    @property
    def half_height(self) -> float:
        return self._half_height

    @property
    def is_triggered(self) -> bool:
        return self._triggered

    @property
    def is_enabled(self) -> bool:
        if self._schema:
            return self._schema.is_enabled
        return not self._triggered

    @property
    def is_one_shot(self) -> bool:
        if self._schema:
            return self._schema.is_one_shot
        return True

    @property
    def event_type(self) -> str:
        if self._schema:
            return self._schema.event_type
        return "OnLevelComplete"

    @property
    def filter_faction(self) -> str:
        if self._schema:
            return self._schema.filter_faction
        return "Player"

    def check_overlap(self, pos: Tuple[float, float, float]) -> bool:
        """
        Evaluates whether a point is within the trigger volume cylindrical envelope.
        horiz_dist <= radius and abs(dy) <= half_height.
        """
        if not self.is_enabled:
            return False

        dx = pos[0] - self._world_pos[0]
        dy = pos[1] - self._world_pos[1]
        dz = pos[2] - self._world_pos[2]

        horiz_dist = math.sqrt(dx * dx + dz * dz)
        if horiz_dist <= self._radius and abs(dy) <= self._half_height:
            return True
        return False

    def on_trigger_entered(self):
        """Marks the trigger as activated and deactivates if one-shot."""
        self._triggered = True
        if self._schema and self._schema.is_one_shot:
            self._schema.is_enabled = False


class HydragonTriggerSystem:
    """
    Manages trigger zones, win-condition evaluation, and game session state.
    Strictly follows ECS guidelines: discovers triggers once on PLAY and indexes in memory.
    """

    _instance: Optional["HydragonTriggerSystem"] = None

    def __init__(self):
        HydragonTriggerSystem._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False
        self._physics_step_sub = None
        self._timeline_sub = None
        self._app_update_sub = None

        self._active_triggers: Dict[str, HydragonTriggerZone] = {}
        self._cached_game_manager_path: Optional[str] = None
        self._level_completed: bool = False

    @classmethod
    def get_instance(cls) -> Optional["HydragonTriggerSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

    @property
    def is_level_completed(self) -> bool:
        return self._level_completed

    def startup(self):
        """Initializes subscriptions to timeline, physics step, and app updates."""
        if not HAS_KIT:
            return

        self._subscribe_timeline()
        self._subscribe_app_update()
        self._subscribe_physics()
        self._is_active = True

        if carb:
            carb.log_info("[hydragon.editor.core] HydragonTriggerSystem started.")

    def shutdown(self):
        """Releases subscriptions and clears cached trigger data."""
        self._is_active = False
        self._is_simulating = False
        self._active_triggers.clear()
        self._cached_game_manager_path = None
        self._level_completed = False

        if not HAS_KIT:
            HydragonTriggerSystem._instance = None
            return

        if self._physics_step_sub:
            self._physics_step_sub = None
        if self._app_update_sub:
            self._app_update_sub = None
        if self._timeline_sub:
            self._timeline_sub = None

        HydragonTriggerSystem._instance = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonTriggerSystem shutdown.")

    # -------------------------------------------------------------------------
    # Subscriptions
    # -------------------------------------------------------------------------
    def _subscribe_timeline(self):
        try:
            timeline = omni.timeline.get_timeline_interface()
            stream = timeline.get_timeline_event_stream()
            self._timeline_sub = stream.create_subscription_to_pop(self._on_timeline_event)
            self._is_simulating = timeline.is_playing()
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Trigger system timeline subscription failed: {e}")

    def _subscribe_app_update(self):
        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                self._on_app_update
            )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Trigger system app update subscription failed: {e}")

    def _subscribe_physics(self) -> bool:
        if self._physics_step_sub is not None:
            return True
        try:
            physx_iface = get_physx_interface()
            if physx_iface:
                self._physics_step_sub = physx_iface.subscribe_physics_step_events(self._on_physics_step)
                if carb:
                    carb.log_info("[hydragon.editor.core] Trigger system subscribed to PhysX steps.")
                return True
        except Exception:
            pass
        return False

    # -------------------------------------------------------------------------
    # Event Callbacks
    # -------------------------------------------------------------------------
    def _on_timeline_event(self, e):
        if not HAS_KIT:
            return
        try:
            event_type = int(e.type)
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                self._is_simulating = True
                self._level_completed = False
                self._subscribe_physics()
                stage = omni.usd.get_context().get_stage()
                if stage:
                    self._discover_entities_once(stage)
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Play mode started. Registered {len(self._active_triggers)} triggers."
                    )
            elif event_type in (int(omni.timeline.TimelineEventType.STOP), int(omni.timeline.TimelineEventType.PAUSE)):
                self._is_simulating = False
                self._active_triggers.clear()
                self._cached_game_manager_path = None
                self._level_completed = False
                if carb:
                    carb.log_info("[hydragon.editor.core] Play mode stopped. Trigger registry cleared.")
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Trigger system timeline event error: {ex}")

    def _on_app_update(self, e):
        """Syncs simulation state and ensures PhysX step subscription is healthy."""
        if not HAS_KIT:
            return

        try:
            timeline = omni.timeline.get_timeline_interface()
            self._is_simulating = timeline.is_playing()
        except Exception:
            pass

        if not self._is_simulating:
            return

        if self._physics_step_sub is None:
            if not self._subscribe_physics():
                dt = e.payload.get("dt", 1.0 / 60.0) if hasattr(e, "payload") else 1.0 / 60.0
                self._on_physics_step(dt)

    # -------------------------------------------------------------------------
    # One-Time Discovery on Simulation Start
    # -------------------------------------------------------------------------
    def _discover_entities_once(self, stage):
        """
        Discovers all trigger prims and game managers once on simulation start.
        Avoids per-frame traversals entirely.
        """
        self._active_triggers.clear()
        self._cached_game_manager_path = None
        self._level_completed = False

        if not stage:
            return

        for prim in stage.Traverse():
            if not prim.IsValid() or not prim.IsActive():
                continue

            # Check for Trigger schema
            if HydragonTrigger.is_applied(prim):
                prim_path = prim.GetPath().pathString
                world_pos = self._compute_prim_world_pos(prim)
                radius, half_height = self._extract_trigger_geometry_bounds(prim)

                zone = HydragonTriggerZone(
                    prim=prim,
                    world_pos=world_pos,
                    radius=radius,
                    half_height=half_height,
                )
                self._active_triggers[prim_path] = zone

            # Discover Game Manager
            if self._cached_game_manager_path is None:
                if HydragonGameManager.is_applied(prim):
                    self._cached_game_manager_path = prim.GetPath().pathString

    def _compute_prim_world_pos(self, prim) -> Tuple[float, float, float]:
        """Calculates world translation of a USD prim."""
        if not HAS_KIT or not prim or not prim.IsValid():
            return 0.0, 0.0, 0.0

        try:
            xformable = UsdGeom.Xformable(prim)
            tf = xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
            t = (
                tf.ExtractTranslation()
                if hasattr(tf, "ExtractTranslation")
                else (tf.GetTranslation() if hasattr(tf, "GetTranslation") else Gf.Vec3d(0.0))
            )
            return (float(t[0]), float(t[1]), float(t[2]))
        except Exception:
            return 0.0, 0.0, 0.0

    def _extract_trigger_geometry_bounds(self, trigger_prim) -> Tuple[float, float]:
        """
        Extracts radius and half-height from trigger volume geometry descendants
        or provides robust defaults for centimeter-scale scene.
        """
        default_radius = 75.0
        default_half_height = 50.0

        if not HAS_KIT or not trigger_prim or not trigger_prim.IsValid():
            return default_radius, default_half_height

        try:
            # Check cylinder detector child (e.g. goal_hole asset)
            for child in trigger_prim.GetAllChildren():
                if UsdGeom and hasattr(UsdGeom, "Cylinder"):
                    cyl = UsdGeom.Cylinder(child)
                    if cyl:
                        rad_attr = cyl.GetRadiusAttr()
                        ht_attr = cyl.GetHeightAttr()
                        r = float(rad_attr.Get()) if rad_attr and rad_attr.IsValid() else default_radius
                        h = float(ht_attr.Get()) if ht_attr and ht_attr.IsValid() else (default_half_height * 2.0)
                        return r, h * 0.5

                for grandchild in child.GetAllChildren():
                    if UsdGeom and hasattr(UsdGeom, "Cylinder"):
                        cyl = UsdGeom.Cylinder(grandchild)
                        if cyl:
                            rad_attr = cyl.GetRadiusAttr()
                            ht_attr = cyl.GetHeightAttr()
                            r = float(rad_attr.Get()) if rad_attr and rad_attr.IsValid() else default_radius
                            h = float(ht_attr.Get()) if ht_attr and ht_attr.IsValid() else (default_half_height * 2.0)
                            return r, h * 0.5
        except Exception:
            pass

        return default_radius, default_half_height

    # -------------------------------------------------------------------------
    # Physics Update Step (O(N) iteration - ZERO per-frame traversals)
    # -------------------------------------------------------------------------
    def _on_physics_step(self, dt: float):
        if not self._is_simulating or not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage()
        if not stage:
            return

        # 1. Update Game Manager session time
        if self._cached_game_manager_path and not self._level_completed:
            gm_prim = stage.GetPrimAtPath(self._cached_game_manager_path)
            if gm_prim and gm_prim.IsValid() and HydragonGameManager.is_applied(gm_prim):
                gm = HydragonGameManager(gm_prim)
                gm.elapsed_time += dt

        if not self._active_triggers:
            return

        # 2. Get Player real-time simulated world position
        player_pos: Optional[Tuple[float, float, float]] = None
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys:
                player_pos = p_sys.get_player_world_pos()
        except Exception:
            pass

        if player_pos is None or (player_pos[0] == 0.0 and player_pos[1] == 0.0 and player_pos[2] == 0.0):
            # Check player rigid body directly if player_sys pos is default
            player_rb = self._resolve_player_rb_path(stage)
            if player_rb:
                player_prim = stage.GetPrimAtPath(player_rb)
                player_pos = self._get_prim_realtime_pos(player_prim, player_rb)

        if player_pos is None:
            return

        # 3. Check triggers overlap
        triggered_paths: List[str] = []

        for prim_path, zone in list(self._active_triggers.items()):
            if not zone.is_enabled:
                continue

            if zone.check_overlap(player_pos):
                zone.on_trigger_entered()
                triggered_paths.append(prim_path)

                if zone.event_type == "OnLevelComplete":
                    self._handle_level_complete(stage)

        # Remove deactivated one-shot triggers
        for path in triggered_paths:
            zone = self._active_triggers.get(path)
            if zone and not zone.is_enabled:
                self._active_triggers.pop(path, None)

    def _handle_level_complete(self, stage):
        """Triggers victory state on Game Manager, awards score, and disables player input."""
        if self._level_completed:
            return

        self._level_completed = True

        score = 0
        elapsed = 0.0

        # Update Game Manager
        if self._cached_game_manager_path and stage:
            try:
                gm_prim = stage.GetPrimAtPath(self._cached_game_manager_path)
                if gm_prim and gm_prim.IsValid() and HydragonGameManager.is_applied(gm_prim):
                    gm = HydragonGameManager(gm_prim)
                    gm.trigger_victory()
                    gm.score += 500  # Bonus for completing level
                    score = gm.score
                    elapsed = gm.elapsed_time
            except Exception as e:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Error updating game manager victory: {e}")

        # Disable player controller input on victory
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys and stage:
                player_prim = p_sys.find_player_prim(stage)
                if player_prim:
                    ctl = HydragonPlayerController(player_prim)
                    ctl.input_enabled = False
        except Exception:
            pass

        if carb:
            carb.log_info("================================================================")
            carb.log_info(f"*** HYDRAGON VICTORY! Level completed in {elapsed:.2f}s! Final Score: {score} ***")
            carb.log_info("================================================================")

    def _resolve_player_rb_path(self, stage) -> Optional[str]:
        """Finds player rigid body path agnostically without hardcoded paths."""
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys:
                rb = p_sys.get_player_rb_path()
                if rb:
                    return rb
        except Exception:
            pass

        if stage:
            for prim in stage.Traverse():
                if prim.IsValid() and prim.IsActive() and HydragonPlayerController.is_applied(prim):
                    rb = _find_rigid_body_descendant(prim)
                    if rb:
                        return rb.GetPath().pathString
        return None

    def _get_prim_realtime_pos(self, prim, path_str: str) -> Optional[Tuple[float, float, float]]:
        """Queries PhysX live transform or USD fallback."""
        if prim and hasattr(prim, "IsValid") and prim.IsValid() and prim.IsActive() and _is_rigid_body(prim):
            try:
                physx_iface = get_physx_interface()
                if physx_iface and hasattr(physx_iface, "get_rigidbody_transformation"):
                    rb_data = physx_iface.get_rigidbody_transformation(path_str)
                    if rb_data and rb_data.get("ret_val", False):
                        p = rb_data.get("position")
                        if p is not None:
                            return (float(p[0]), float(p[1]), float(p[2]))
            except Exception:
                pass

        return self._compute_prim_world_pos(prim)
