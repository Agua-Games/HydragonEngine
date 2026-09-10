"""
Hydragon Engine - Foes AI Physics Controller System

Component-driven AI system for adversary actors using OpenUSD API schemas
(HydragonChaserAIAPI, HydragonActorAPI), PhysX physics simulation, and
an ECS state machine (Patrol <-> Chase).
"""

import math
import random
import threading
import time
from typing import Dict, List, Optional, Tuple

try:
    import carb
    import omni.kit.app
    import omni.timeline
    import omni.usd
    import omni.physx
    from omni.physx import (
        get_physx_interface,
        get_physx_simulation_interface,
        get_physx_scene_query_interface,
    )
    from pxr import Usd, UsdGeom, Sdf, Gf, UsdUtils, UsdPhysics, PhysicsSchemaTools
    try:
        from pxr import PhysxSchema
    except ImportError:
        PhysxSchema = None
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    UsdPhysics = None
    PhysxSchema = None

from .schemas import HydragonActor, HydragonChaserAI, HydragonGameManager, HydragonPlayerController, HydragonPhysicsManager


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

    # 4. UsdPhysics wrapper boolean check
    try:
        if UsdPhysics and hasattr(UsdPhysics, "RigidBodyAPI"):
            if bool(UsdPhysics.RigidBodyAPI(prim)):
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



class HydragonAIBrain:
    """
    Component class representing an individual AI entity's state machine and behavior.
    Encapsulates patrol, chase, blackboard state, and clean entity destruction.
    """

    def __init__(self, prim, origin_pos: Optional[Tuple[float, float, float]] = None):
        self._prim = prim
        self._prim_path: str = prim.GetPath().pathString if prim and hasattr(prim, "GetPath") else ""
        self._rb_path: Optional[str] = None
        self._actor: Optional[HydragonActor] = HydragonActor(prim) if prim else None
        self._schema: Optional[HydragonChaserAI] = HydragonChaserAI(prim) if prim else None

        self._origin_pos: Tuple[float, float, float] = origin_pos or (0.0, 0.0, 0.0)
        self._current_pos: Tuple[float, float, float] = self._origin_pos
        self._patrol_waypoint: Tuple[float, float, float] = self._origin_pos

        initial_state = "Patrol"
        if self._schema and hasattr(self._schema, "state") and self._schema.state in ("Patrol", "Chase"):
            initial_state = self._schema.state
        self._state: str = initial_state
        self._patrol_timer: float = 0.0
        self._patrol_duration: float = random.uniform(2.5, 4.5)
        self._is_alive: bool = True
        self._last_stomp_check_time: float = 0.0
        self._mass: float = 5.0
        self._radius: float = 50.0

        if self._schema and hasattr(self._schema, "state"):
            self._schema.state = self._state

    @property
    def prim_path(self) -> str:
        return self._prim_path

    @property
    def rb_path(self) -> Optional[str]:
        return self._rb_path

    @rb_path.setter
    def rb_path(self, path: Optional[str]):
        self._rb_path = path

    @property
    def mass(self) -> float:
        return self._mass

    @mass.setter
    def mass(self, val: float):
        self._mass = max(0.1, float(val))

    @property
    def radius(self) -> float:
        return self._radius

    @radius.setter
    def radius(self, val: float):
        self._radius = max(1.0, float(val))

    @property
    def target_prim_path(self) -> Optional[str]:
        if self._schema and self._schema.target_prim:
            return str(self._schema.target_prim)
        return None

    @property
    def is_alive(self) -> bool:
        return self._is_alive

    @property
    def state(self) -> str:
        return self._state

    @property
    def current_pos(self) -> Tuple[float, float, float]:
        return self._current_pos

    def set_current_pos(self, pos: Tuple[float, float, float]):
        self._current_pos = pos

    def set_origin_pos(self, pos: Tuple[float, float, float]):
        self._origin_pos = pos
        self._current_pos = pos
        self._pick_new_patrol_waypoint()

    def _get_effective_radius(self, radius_val: float) -> float:
        """Translates meter-scale schema radius to centimeter scene units if needed."""
        if radius_val < 100.0:
            return radius_val * 100.0
        return radius_val

    def _get_effective_force(
        self,
        force_val: float,
        default_mag: float = 24500.0,
        accel_multiplier: float = 14.0,
        min_accel: float = 2000.0,
        max_force: float = 38000.0,
    ) -> float:
        """
        Ensures force magnitude is sufficient for centimeter-scale PhysX rigid bodies,
        scales proportionally with mass, and enforces an upper bound to prevent vertical
        collision ramp launch during sphere-on-sphere impacts.
        """
        if force_val >= 1000.0:
            calc_force = max(force_val, self._mass * min_accel)
        elif force_val > 0.0:
            target_accel = max(force_val * accel_multiplier, min_accel)
            calc_force = self._mass * target_accel
        else:
            calc_force = max(default_mag, self._mass * min_accel)

        return min(calc_force, max_force)

    def _pick_new_patrol_waypoint(self):
        """Picks a random 2D navigation target within patrol radius around origin position."""
        patrol_rad = 500.0
        if self._schema:
            patrol_rad = self._get_effective_radius(self._schema.patrol_radius)

        angle = random.uniform(0.0, 2.0 * math.pi)
        dist = random.uniform(0.2 * patrol_rad, patrol_rad)
        wx = self._origin_pos[0] + dist * math.cos(angle)
        wz = self._origin_pos[2] + dist * math.sin(angle)
        wy = self._origin_pos[1]
        self._patrol_waypoint = (wx, wy, wz)
        self._patrol_timer = 0.0
        self._patrol_duration = random.uniform(2.5, 4.5)

    def update_state_machine(
        self,
        dt: float,
        player_pos: Optional[Tuple[float, float, float]],
    ) -> Tuple[str, Tuple[float, float]]:
        """
        Evaluates state transitions and returns the computed (state, (horizontal_force_x, horizontal_force_z)).
        """
        if not self._is_alive:
            return "Dead", (0.0, 0.0)

        # 1. Calculate distance to player if available
        dist_to_player = float("inf")
        p_dx = 0.0
        p_dz = 0.0
        if player_pos is not None:
            p_dx = player_pos[0] - self._current_pos[0]
            p_dz = player_pos[2] - self._current_pos[2]
            dist_to_player = math.sqrt(p_dx * p_dx + p_dz * p_dz)

        detection_rad = 1000.0
        lose_rad = 1500.0
        if self._schema:
            detection_rad = self._get_effective_radius(self._schema.detection_radius)
            lose_rad = self._get_effective_radius(self._schema.lose_target_radius)

        # 2. State Transitions
        if self._state == "Patrol":
            if dist_to_player <= detection_rad:
                self._state = "Chase"
                if self._schema:
                    self._schema.state = "Chase"
        elif self._state == "Chase":
            if dist_to_player > lose_rad:
                self._state = "Patrol"
                if self._schema:
                    self._schema.state = "Patrol"
                self._pick_new_patrol_waypoint()

        # 3. State Behavior Execution
        force_x = 0.0
        force_z = 0.0

        if self._state == "Chase":
            if dist_to_player > 1e-4:
                nx = p_dx / dist_to_player
                nz = p_dz / dist_to_player
                chase_val = self._schema.chase_force if self._schema and hasattr(self._schema, "chase_force") else 350.0
                chase_mag = self._get_effective_force(
                    chase_val, default_mag=24500.0, accel_multiplier=14.0, min_accel=2000.0, max_force=38000.0
                )
                force_x = nx * chase_mag
                force_z = nz * chase_mag

        elif self._state == "Patrol":
            self._patrol_timer += dt
            w_dx = self._patrol_waypoint[0] - self._current_pos[0]
            w_dz = self._patrol_waypoint[2] - self._current_pos[2]
            dist_to_waypoint = math.sqrt(w_dx * w_dx + w_dz * w_dz)

            # If reached waypoint or time expired, select new patrol target
            if dist_to_waypoint < 50.0 or self._patrol_timer >= self._patrol_duration:
                self._pick_new_patrol_waypoint()
                w_dx = self._patrol_waypoint[0] - self._current_pos[0]
                w_dz = self._patrol_waypoint[2] - self._current_pos[2]
                dist_to_waypoint = math.sqrt(w_dx * w_dx + w_dz * w_dz)

            if dist_to_waypoint > 1e-4:
                wx_norm = w_dx / dist_to_waypoint
                wz_norm = w_dz / dist_to_waypoint
                patrol_val = self._schema.patrol_force if self._schema and hasattr(self._schema, "patrol_force") else 150.0
                patrol_mag = self._get_effective_force(
                    patrol_val, default_mag=10500.0, accel_multiplier=14.0, min_accel=1500.0, max_force=20000.0
                )
                force_x = wx_norm * patrol_mag
                force_z = wz_norm * patrol_mag

        return self._state, (force_x, force_z)

        return self._state, (force_x, force_z)

    def destroy(self, reason: str = ""):
        """
        Deactivates this entity cleanly according to OpenUSD ECS guidelines.
        Sets prim.SetActive(False) to immediately cease PhysX simulation and Hydra draw calls.
        """
        self._is_alive = False
        if self._actor:
            self._actor.is_alive = False

        if HAS_KIT and self._prim and hasattr(self._prim, "IsValid") and self._prim.IsValid():
            try:
                if self._prim.IsActive():
                    self._prim.SetActive(False)
                if hasattr(UsdGeom, "Imageable"):
                    UsdGeom.Imageable(self._prim).MakeInvisible()
            except Exception as e:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to deactivate prim {self._prim_path}: {e}")

        if carb:
            carb.log_info(f"[hydragon.editor.core] Foe '{self._prim_path}' destroyed ({reason}). Prim deactivated.")


class HydragonFoesControllerSystem:
    """
    Manages discovery, batch physics updates, and lifecycle for all adversary actors.
    Adheres strictly to the avoid-stage-traversal-anti-pattern convention:
    entities are discovered once on simulation start and indexed in memory.
    """

    KILL_FLOOR_Y: float = -300.0  # Fall boundary in stage units (cm)

    _instance: Optional["HydragonFoesControllerSystem"] = None

    def __init__(self):
        HydragonFoesControllerSystem._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False
        self._physics_step_sub = None
        self._timeline_sub = None
        self._app_update_sub = None

        self._active_brains: Dict[str, HydragonAIBrain] = {}
        self._registered_brains: Dict[str, HydragonAIBrain] = {}
        self._cached_player_path: Optional[str] = None
        self._cached_player_rb_path: Optional[str] = None
        self._cached_game_manager_path: Optional[str] = None
        self._configured_rb_paths: set = set()
        self._destroyed_prim_paths: set = set()
        self._contact_report_sub = None
        self._hits_lock = threading.Lock()
        self._pending_hits: List[Tuple[str, bool]] = []
        self._pending_bounce: bool = False
        self._pending_bounce_foe_pos: Optional[Tuple[float, float, float]] = None
        self._physics_mgr: Optional[HydragonPhysicsManager] = None
        self._last_log_time: float = 0.0
        self._last_player_bounce_time: float = 0.0

    @classmethod
    def get_instance(cls) -> Optional["HydragonFoesControllerSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

    def queue_foe_destruction(self, prim_path: str, is_stomp: bool = False, bounce_applied: bool = False):
        """Queues a foe for clean destruction on the main thread (invoked by kill volumes or hazard systems)."""
        with self._hits_lock:
            self._pending_hits.append((prim_path, is_stomp, bounce_applied))

    def startup(self):
        """Initializes subscriptions to timeline, physics step, and app updates."""
        if not HAS_KIT:
            return

        self._subscribe_timeline()
        self._subscribe_app_update()
        self._subscribe_physics()
        self._is_active = True

        if carb:
            carb.log_info("[hydragon.editor.core] HydragonFoesControllerSystem started.")

    def shutdown(self):
        """Releases subscriptions and clears all active AI brains."""
        self._is_active = False
        self._is_simulating = False
        self._active_brains.clear()
        self._registered_brains.clear()
        self._configured_rb_paths.clear()
        self._destroyed_prim_paths.clear()
        with self._hits_lock:
            self._pending_hits.clear()
        self._pending_bounce = False
        self._pending_bounce_foe_pos = None
        self._physics_mgr = None
        self._last_player_bounce_time = 0.0
        self._cached_player_path = None
        self._cached_player_rb_path = None
        self._cached_game_manager_path = None

        if not HAS_KIT:
            HydragonFoesControllerSystem._instance = None
            return

        if self._physics_step_sub:
            try:
                physx_iface = get_physx_interface()
                if physx_iface and hasattr(physx_iface, "unsubscribe_physics_step_events"):
                    physx_iface.unsubscribe_physics_step_events(self._physics_step_sub)
            except Exception:
                pass
            self._physics_step_sub = None

        if self._contact_report_sub:
            try:
                sim_iface = get_physx_simulation_interface()
                if sim_iface and hasattr(sim_iface, "unsubscribe_contact_report_events"):
                    sim_iface.unsubscribe_contact_report_events(self._contact_report_sub)
            except Exception:
                pass
            self._contact_report_sub = None

        if self._app_update_sub:
            self._app_update_sub = None
        if self._timeline_sub:
            self._timeline_sub = None

        HydragonFoesControllerSystem._instance = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonFoesControllerSystem shutdown.")

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
                carb.log_warn(f"[hydragon.editor.core] Foes controller timeline subscription failed: {e}")

    def _subscribe_app_update(self):
        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                self._on_app_update
            )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Foes controller app update subscription failed: {e}")

    def _subscribe_physics(self) -> bool:
        if self._physics_step_sub is not None:
            return True
        try:
            physx_iface = get_physx_interface()
            if physx_iface:
                self._physics_step_sub = physx_iface.subscribe_physics_step_events(self._on_physics_step)
                if carb:
                    carb.log_info("[hydragon.editor.core] Foes controller subscribed to PhysX steps.")

            sim_iface = get_physx_simulation_interface()
            if sim_iface and self._contact_report_sub is None:
                self._contact_report_sub = sim_iface.subscribe_contact_report_events(self._on_contact_report_event)
                if carb:
                    carb.log_info("[hydragon.editor.core] Foes controller subscribed to PhysX contact report events.")

            return self._physics_step_sub is not None
        except Exception:
            pass
        return False

    # -------------------------------------------------------------------------
    # Timeline & App Events
    # -------------------------------------------------------------------------
    def _on_timeline_event(self, e):
        if not HAS_KIT:
            return
        try:
            event_type = int(e.type)
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                self._is_simulating = True
                self._subscribe_physics()
                stage = omni.usd.get_context().get_stage()
                if stage:
                    self._discover_entities_once(stage)
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Play mode started. Registered {len(self._active_brains)} AI foes."
                    )
            elif event_type in (int(omni.timeline.TimelineEventType.STOP), int(omni.timeline.TimelineEventType.PAUSE)):
                self._is_simulating = False

                if event_type == int(omni.timeline.TimelineEventType.STOP):
                    # Automatic stage restoration on STOP: restore destroyed foes so USD file is never corrupted
                    stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                    if stage and self._destroyed_prim_paths:
                        restored_count = 0
                        for p_path in list(self._destroyed_prim_paths):
                            try:
                                prim = stage.GetPrimAtPath(p_path)
                                if prim and prim.IsValid():
                                    prim.SetActive(True)
                                    UsdGeom.Imageable(prim).MakeVisible()
                                    actor = HydragonActor(prim)
                                    if actor:
                                        actor.is_alive = True
                                    for child in prim.GetAllChildren():
                                        if child.HasAttribute("physics:rigidBodyEnabled"):
                                            child.GetAttribute("physics:rigidBodyEnabled").Set(True)
                                    restored_count += 1
                            except Exception:
                                pass
                        if carb:
                            carb.log_info(
                                f"[hydragon.editor.core] Re-activated {restored_count} destroyed foes on simulation STOP."
                            )
                        self._destroyed_prim_paths.clear()

                self._active_brains.clear()
                self._registered_brains.clear()
                self._configured_rb_paths.clear()
                self._cached_player_path = None
                self._cached_player_rb_path = None
                self._cached_game_manager_path = None
                self._last_player_bounce_time = 0.0
                if carb:
                    carb.log_info("[hydragon.editor.core] Play mode stopped. AI brains cleared.")
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Foes controller timeline event error: {ex}")

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

        # Process pending foe hits collected from PhysX contact reports safely on the main thread.
        # Drain the queue atomically under _hits_lock, then process outside the lock to prevent mutex recursion.
        hits_to_process: List[Tuple] = []
        with self._hits_lock:
            if self._pending_hits:
                hits_to_process = list(self._pending_hits)
                self._pending_hits.clear()

        if hits_to_process:
            stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
            for hit_item in hits_to_process:
                p_path = hit_item[0]
                is_stomp = hit_item[1]
                bounce_applied = hit_item[2] if len(hit_item) > 2 else False

                if p_path in self._destroyed_prim_paths:
                    continue
                self._destroyed_prim_paths.add(p_path)
                brain = self._active_brains.pop(p_path, None) or self._registered_brains.get(p_path)
                if brain:
                    reason = "contact_report_stomp" if is_stomp else "contact_report_collision"
                    brain.destroy(reason=reason)

                # Direct OpenUSD deactivation fallback on stage to guarantee prim removal from PhysX & Hydra
                if stage:
                    try:
                        prim = stage.GetPrimAtPath(p_path)
                        if prim and prim.IsValid() and prim.IsActive():
                            prim.SetActive(False)
                            if hasattr(UsdGeom, "Imageable"):
                                UsdGeom.Imageable(prim).MakeInvisible()
                    except Exception as ex:
                        if carb:
                            carb.log_warn(f"[hydragon.editor.core] Failed to deactivate prim at {p_path}: {ex}")
                    self._notify_game_manager_foe_destroyed(stage)
                    foe_pos = brain.current_pos if brain else (0.0, 0.0, 0.0)
                    try:
                        from .game_hud import HydragonGameHUD
                        hud = HydragonGameHUD.get_instance()
                        if hud:
                            points = 150 if is_stomp else 100
                            hud.show_score_popup(world_pos=foe_pos, points=points)
                            if carb:
                                carb.log_info(f"[hydragon.editor.core] Foe defeated at {foe_pos}! Popped +{points} score.")
                    except Exception as ex:
                        if carb:
                            carb.log_warn(f"[hydragon.editor.core] Failed to show score popup: {ex}")

                    try:
                        from .effects_controller import HydragonEffectsSystem
                        effects_sys = HydragonEffectsSystem.get_instance()
                        if effects_sys:
                            effects_sys.spawn_foe_destruction_vfx(stage=stage, world_pos=foe_pos, color_theme="gold")
                    except Exception as ex:
                        if carb:
                            carb.log_warn(f"[hydragon.editor.core] Failed to spawn foe destruction VFX: {ex}")

                    if not bounce_applied:
                        try:
                            self._apply_player_bounce(stage, foe_pos=foe_pos, is_stomp=is_stomp)
                        except Exception as ex:
                            if carb:
                                carb.log_warn(f"[hydragon.editor.core] Failed to trigger player bounce: {ex}")

        if self._physics_step_sub is None:
            if not self._subscribe_physics():
                dt = e.payload.get("dt", 1.0 / 60.0) if hasattr(e, "payload") else 1.0 / 60.0
                self._on_physics_step(dt)

    # -------------------------------------------------------------------------
    # One-Time Discovery on Simulation Start
    # -------------------------------------------------------------------------
    def _get_player_paths(self, stage) -> Tuple[Optional[str], Optional[str]]:
        """Locates the player root prim and rigid body prim agnostically."""
        if self._cached_player_path and self._cached_player_rb_path:
            return self._cached_player_path, self._cached_player_rb_path

        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys:
                if not self._cached_player_rb_path:
                    self._cached_player_rb_path = p_sys.get_player_rb_path()
                if not self._cached_player_path:
                    self._cached_player_path = p_sys.get_player_root_path()
                if self._cached_player_path and self._cached_player_rb_path:
                    return self._cached_player_path, self._cached_player_rb_path
        except Exception:
            pass

        if stage:
            for prim in stage.Traverse():
                if prim.IsValid() and prim.IsActive() and HydragonPlayerController.is_applied(prim):
                    self._cached_player_path = prim.GetPath().pathString
                    p_rb_prim = _find_rigid_body_descendant(prim)
                    if p_rb_prim:
                        self._cached_player_rb_path = p_rb_prim.GetPath().pathString
                    else:
                        self._cached_player_rb_path = self._cached_player_path
                    return self._cached_player_path, self._cached_player_rb_path

        return self._cached_player_path, self._cached_player_rb_path

    def _get_player_rb_path(self, stage) -> Optional[str]:
        """Locates the player rigid body path agnostically without hardcoded paths."""
        return self._get_player_paths(stage)[1]

    def _discover_physics_manager(self, stage) -> Optional[HydragonPhysicsManager]:
        """
        Discovers active HydragonPhysicsAPI manager prim on simulation start.
        Checks common candidate paths O(1) first, then falls back to single traversal on PLAY.
        """
        if not stage:
            return None

        candidate_paths = (
            "/World/Singletons/PhysicsManager",
            "/World/PhysicsManager",
            "/World/Singletons/Physics",
            "/World/Physics",
            "/World/PhysicsScene",
            "/PhysicsManager",
        )

        for path_str in candidate_paths:
            prim = stage.GetPrimAtPath(path_str)
            if prim and prim.IsValid() and HydragonPhysicsManager.is_applied(prim):
                return HydragonPhysicsManager(prim)

        for prim in stage.Traverse():
            if prim.IsValid() and HydragonPhysicsManager.is_applied(prim):
                return HydragonPhysicsManager(prim)

        return None

    def _discover_entities_once(self, stage):
        """
        Discovers all adversary entities, players, and game manager once on simulation start.
        NEVER run this per-frame.
        """
        self._active_brains.clear()
        self._registered_brains.clear()
        self._configured_rb_paths.clear()
        self._cached_player_path = None
        self._cached_player_rb_path = None
        self._cached_game_manager_path = None
        self._physics_mgr = self._discover_physics_manager(stage)

        if not stage:
            return

        # Query player paths from player controller singleton if ready
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys:
                self._cached_player_rb_path = p_sys.get_player_rb_path()
                self._cached_player_path = p_sys.get_player_root_path()
        except Exception:
            pass

        for prim in stage.Traverse():
            if not prim.IsValid() or not prim.IsActive():
                continue

            # Check for Chaser AI schema
            if HydragonChaserAI.is_applied(prim):
                prim_path = prim.GetPath().pathString
                brain = HydragonAIBrain(prim)

                # Locate rigid body prim (root or child geometry)
                rb_prim = _find_rigid_body_descendant(prim)
                if rb_prim:
                    rb_path = rb_prim.GetPath().pathString
                    brain.rb_path = rb_path
                    self._ensure_rigid_body_damping(rb_prim)

                    # Compute world scale, mass, and radius
                    sx, sy, sz = self._compute_prim_world_scale(rb_prim)
                    scale_avg = (sx + sy + sz) / 3.0
                    volume_scale = sx * sy * sz

                    base_mass = 5.0
                    if rb_prim.HasAttribute("physics:mass"):
                        m_val = rb_prim.GetAttribute("physics:mass").Get()
                        if m_val is not None and float(m_val) > 0.0:
                            base_mass = float(m_val)
                    brain.mass = max(0.1, base_mass * volume_scale)

                    base_radius = 50.0
                    if rb_prim.HasAttribute("radius"):
                        r_val = rb_prim.GetAttribute("radius").Get()
                        if r_val is not None and float(r_val) > 0.0:
                            base_radius = float(r_val)
                    brain.radius = max(1.0, base_radius * scale_avg)

                    # Initialize origin position directly from authored USD transform (never call PhysX on PLAY before simulation steps)
                    origin = self._compute_prim_usd_world_pos(rb_prim)
                    brain.set_origin_pos(origin)
                else:
                    brain.rb_path = None
                    if carb:
                        carb.log_warn(f"[hydragon.editor.core] Foe '{prim_path}' has no rigid body descendant.")

                self._active_brains[prim_path] = brain
                self._registered_brains[prim_path] = brain

            # Discover Player RigidBody for target tracking if not already resolved
            if self._cached_player_rb_path is None:
                if HydragonPlayerController.is_applied(prim):
                    self._cached_player_path = prim.GetPath().pathString
                    rb_p = _find_rigid_body_descendant(prim)
                    if rb_p:
                        self._cached_player_rb_path = rb_p.GetPath().pathString
                    else:
                        self._cached_player_rb_path = self._cached_player_path

            # Discover Game Manager
            if self._cached_game_manager_path is None:
                if HydragonGameManager.is_applied(prim):
                    self._cached_game_manager_path = prim.GetPath().pathString

    def _compute_prim_usd_world_pos(self, prim) -> Tuple[float, float, float]:
        """Calculates world translation of a USD prim using OpenUSD without querying PhysX."""
        if not HAS_KIT or not prim or not hasattr(prim, "IsValid") or not prim.IsValid():
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

    def _compute_prim_world_scale(self, prim) -> Tuple[float, float, float]:
        """Calculates world scale (sx, sy, sz) of a USD prim using OpenUSD without querying PhysX."""
        if not HAS_KIT or not prim or not hasattr(prim, "IsValid") or not prim.IsValid():
            return 1.0, 1.0, 1.0
        try:
            xformable = UsdGeom.Xformable(prim)
            tf = xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
            r0 = tf.GetRow(0)
            r1 = tf.GetRow(1)
            r2 = tf.GetRow(2)
            sx = math.sqrt(r0[0] * r0[0] + r0[1] * r0[1] + r0[2] * r0[2])
            sy = math.sqrt(r1[0] * r1[0] + r1[1] * r1[1] + r1[2] * r1[2])
            sz = math.sqrt(r2[0] * r2[0] + r2[1] * r2[1] + r2[2] * r2[2])
            return (float(sx), float(sy), float(sz))
        except Exception:
            return 1.0, 1.0, 1.0

    def _check_foe_grounded(
        self,
        rb_prim,
        rb_path: str,
        world_pos: Tuple[float, float, float],
        radius: float,
    ) -> bool:
        """
        Checks whether the foe sphere is resting on or close to the ground plane/obstacle
        before applying driving traction forces. Prevents horizontal flight while airborne.
        """
        bottom_y = world_pos[1] - radius

        if HAS_KIT:
            try:
                sq_iface = get_physx_scene_query_interface()
                if sq_iface and hasattr(sq_iface, "raycast_all"):
                    origin = carb.Float3(world_pos[0], world_pos[1], world_pos[2])
                    down_dir = carb.Float3(0.0, -1.0, 0.0)
                    max_dist = radius + 20.0  # 20 cm ground clearance threshold

                    ground_hits = []

                    def _on_hit(hit):
                        hit_rb = str(getattr(hit, "rigid_body", "") or "")
                        hit_col = str(getattr(hit, "collision", "") or "")
                        if rb_path in hit_rb or rb_path in hit_col:
                            return True
                        ground_hits.append(hit)
                        return True

                    sq_iface.raycast_all(origin, down_dir, max_dist, _on_hit)
                    if ground_hits:
                        return True
            except Exception:
                pass

        return bottom_y <= 25.0

    def _ensure_rigid_body_damping(self, rb_prim):
        """Applies angular damping, linear damping, max velocities, CCD, and solver iterations."""
        if not rb_prim or not rb_prim.IsValid():
            return

        rb_path = rb_prim.GetPath().pathString if hasattr(rb_prim, "GetPath") else str(id(rb_prim))
        if rb_path in self._configured_rb_paths:
            return
        self._configured_rb_paths.add(rb_path)

        phys_mgr = self._physics_mgr
        target_max_linear_vel = phys_mgr.max_linear_velocity if phys_mgr else 10000.0
        if 0.0 < target_max_linear_vel < 100.0:
            target_max_linear_vel *= 100.0
        target_max_ang_vel = phys_mgr.max_angular_velocity if phys_mgr else 3600.0
        if 0.0 < target_max_ang_vel <= 180.0:
            target_max_ang_vel = math.degrees(target_max_ang_vel)
        target_lin_damping = phys_mgr.default_linear_damping if phys_mgr else 0.05
        target_ang_damping = phys_mgr.default_angular_damping if phys_mgr else 0.1
        target_pos_iters = phys_mgr.solver_position_iterations if phys_mgr else 16
        target_vel_iters = phys_mgr.solver_velocity_iterations if phys_mgr else 4
        target_ccd = phys_mgr.enable_ccd if phys_mgr else True

        try:
            if UsdPhysics:
                rb_api = UsdPhysics.RigidBodyAPI(rb_prim)
                if rb_api:
                    ang_attr = rb_api.GetAngularDampingAttr()
                    if not ang_attr or not ang_attr.IsValid():
                        rb_api.CreateAngularDampingAttr(target_ang_damping)
                    else:
                        ang_attr.Set(target_ang_damping)

                    lin_attr = rb_api.GetLinearDampingAttr()
                    if not lin_attr or not lin_attr.IsValid():
                        rb_api.CreateLinearDampingAttr(target_lin_damping)
                    else:
                        lin_attr.Set(target_lin_damping)
        except Exception:
            pass

        try:
            if PhysxSchema and hasattr(PhysxSchema, "PhysxRigidBodyAPI"):
                physx_rb = PhysxSchema.PhysxRigidBodyAPI(rb_prim)
                if not physx_rb and hasattr(PhysxSchema.PhysxRigidBodyAPI, "Apply"):
                    physx_rb = PhysxSchema.PhysxRigidBodyAPI.Apply(rb_prim)
                if physx_rb:
                    max_ang_attr = physx_rb.GetMaxAngularVelocityAttr()
                    if not max_ang_attr or not max_ang_attr.IsValid():
                        physx_rb.CreateMaxAngularVelocityAttr(target_max_ang_vel)
                    else:
                        max_ang_attr.Set(target_max_ang_vel)

                    # Max Linear Velocity clamp: set unconditionally from PhysicsManager
                    if hasattr(physx_rb, "GetMaxLinearVelocityAttr"):
                        max_lin_attr = physx_rb.GetMaxLinearVelocityAttr()
                        if not max_lin_attr or not max_lin_attr.IsValid():
                            physx_rb.CreateMaxLinearVelocityAttr(target_max_linear_vel)
                        else:
                            max_lin_attr.Set(target_max_linear_vel)
                    elif rb_prim.HasAttribute("physxRigidBody:maxLinearVelocity"):
                        rb_prim.GetAttribute("physxRigidBody:maxLinearVelocity").Set(target_max_linear_vel)

                    # Solver iteration counts for stable constraint convergence under compression
                    if hasattr(physx_rb, "GetSolverPositionIterationCountAttr"):
                        pos_iter_attr = physx_rb.GetSolverPositionIterationCountAttr()
                        if not pos_iter_attr or not pos_iter_attr.IsValid():
                            physx_rb.CreateSolverPositionIterationCountAttr(target_pos_iters)
                        else:
                            pos_iter_attr.Set(target_pos_iters)
                    elif rb_prim.HasAttribute("physxRigidBody:solverPositionIterationCount"):
                        rb_prim.GetAttribute("physxRigidBody:solverPositionIterationCount").Set(target_pos_iters)

                    if hasattr(physx_rb, "GetSolverVelocityIterationCountAttr"):
                        vel_iter_attr = physx_rb.GetSolverVelocityIterationCountAttr()
                        if not vel_iter_attr or not vel_iter_attr.IsValid():
                            physx_rb.CreateSolverVelocityIterationCountAttr(target_vel_iters)
                        else:
                            vel_iter_attr.Set(target_vel_iters)
                    elif rb_prim.HasAttribute("physxRigidBody:solverVelocityIterationCount"):
                        rb_prim.GetAttribute("physxRigidBody:solverVelocityIterationCount").Set(target_vel_iters)

                    # Continuous Collision Detection (CCD) to prevent floor tunneling
                    # NOTE: PhysX throws an error if CCD is enabled on kinematic rigid bodies.
                    # Only enable CCD if the body is dynamic (not kinematic).
                    is_kinematic = False
                    if rb_prim.HasAttribute("physics:kinematicEnabled"):
                        is_kinematic = bool(rb_prim.GetAttribute("physics:kinematicEnabled").Get() or False)

                    ccd_attr = physx_rb.GetEnableCCDAttr()
                    if is_kinematic:
                        if ccd_attr and ccd_attr.IsValid():
                            ccd_attr.Set(False)
                    else:
                        if not ccd_attr or not ccd_attr.IsValid():
                            physx_rb.CreateEnableCCDAttr(target_ccd)
                        else:
                            ccd_attr.Set(target_ccd)
            else:
                # Direct USD attribute fallback (headless / testing outside Kit)
                if rb_prim.HasAttribute("physxRigidBody:maxLinearVelocity"):
                    attr = rb_prim.GetAttribute("physxRigidBody:maxLinearVelocity")
                    if attr:
                        attr.Set(target_max_linear_vel)
                if rb_prim.HasAttribute("physxRigidBody:maxAngularVelocity"):
                    attr = rb_prim.GetAttribute("physxRigidBody:maxAngularVelocity")
                    if attr:
                        attr.Set(target_max_ang_vel)
                if rb_prim.HasAttribute("physxRigidBody:solverPositionIterationCount"):
                    attr = rb_prim.GetAttribute("physxRigidBody:solverPositionIterationCount")
                    if attr:
                        attr.Set(target_pos_iters)
                if rb_prim.HasAttribute("physxRigidBody:solverVelocityIterationCount"):
                    attr = rb_prim.GetAttribute("physxRigidBody:solverVelocityIterationCount")
                    if attr:
                        attr.Set(target_vel_iters)
                if rb_prim.HasAttribute("physxRigidBody:enableCCD"):
                    is_kinematic = False
                    if rb_prim.HasAttribute("physics:kinematicEnabled"):
                        is_kinematic = bool(rb_prim.GetAttribute("physics:kinematicEnabled").Get() or False)
                    attr = rb_prim.GetAttribute("physxRigidBody:enableCCD")
                    if attr:
                        attr.Set(False if is_kinematic else target_ccd)

            # Contact Reporting API for native event-based collision detection
            if PhysxSchema and hasattr(PhysxSchema, "PhysxContactReportAPI"):
                targets = [rb_prim]
                if hasattr(rb_prim, "GetAllDescendants"):
                    for desc in rb_prim.GetAllDescendants():
                        if desc.IsValid() and (desc.HasAttribute("physics:collisionEnabled") or (hasattr(desc, "HasAPI") and UsdPhysics and desc.HasAPI(UsdPhysics.CollisionAPI))):
                            targets.append(desc)
                for target_p in targets:
                    contact_api = PhysxSchema.PhysxContactReportAPI(target_p)
                    if not contact_api and hasattr(PhysxSchema.PhysxContactReportAPI, "Apply"):
                        contact_api = PhysxSchema.PhysxContactReportAPI.Apply(target_p)
                    if contact_api:
                        thresh_attr = contact_api.GetThresholdAttr()
                        if not thresh_attr or not thresh_attr.IsValid():
                            contact_api.CreateThresholdAttr(0.0)
                        else:
                            thresh_attr.Set(0.0)
        except Exception:
            pass

    def _get_rigid_body_world_pos(self, rb_prim, rb_path_str: str) -> Tuple[float, float, float]:
        """Queries real-time simulated world position from PhysX interface, falling back to USD."""
        if not HAS_KIT:
            return 0.0, 0.0, 0.0

        if rb_prim and hasattr(rb_prim, "IsValid") and rb_prim.IsValid() and rb_prim.IsActive() and _is_rigid_body(rb_prim):
            try:
                physx_iface = get_physx_interface()
                if physx_iface and hasattr(physx_iface, "get_rigidbody_transformation"):
                    rb_data = physx_iface.get_rigidbody_transformation(rb_path_str)
                    if rb_data and rb_data.get("ret_val", False):
                        p = rb_data.get("position")
                        if p is not None:
                            return (float(p[0]), float(p[1]), float(p[2]))
            except Exception:
                pass

        return self._compute_prim_usd_world_pos(rb_prim)

    # -------------------------------------------------------------------------
    # Native PhysX Contact Report Event Handler (Event-Driven Collision)
    # -------------------------------------------------------------------------
    def _on_contact_report_event(self, contact_headers, contact_data):
        """Native PhysX contact report callback triggered when rigid bodies collide."""
        if not self._is_simulating or not HAS_KIT or not self._registered_brains:
            return

        stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
        if not stage:
            return

        stage_id = UsdUtils.StageCache.Get().GetId(stage).ToLongInt()
        sim_iface = get_physx_simulation_interface()
        player_root, player_rb = self._get_player_paths(stage)
        if not player_rb:
            return

        def _is_player_path(path_str: str) -> bool:
            if not path_str:
                return False
            if path_str == player_rb or path_str == player_root:
                return True
            if player_root and path_str.startswith(player_root + "/"):
                return True
            if player_rb and path_str.startswith(player_rb + "/"):
                return True
            return False

        # Build reverse lookup map from rigid body path or prim path to brain
        rb_to_brain: Dict[str, HydragonAIBrain] = {}
        for prim_path, brain in self._registered_brains.items():
            if brain.is_alive:
                rb_to_brain[prim_path] = brain
                if brain.rb_path:
                    rb_to_brain[brain.rb_path] = brain

        stomped_brains = []
        collided_brains = []

        for header in contact_headers:
            # Decode actor paths
            actor0_sdf = PhysicsSchemaTools.intToSdfPath(header.actor0)
            actor1_sdf = PhysicsSchemaTools.intToSdfPath(header.actor1)
            collider0_sdf = PhysicsSchemaTools.intToSdfPath(header.collider0)
            collider1_sdf = PhysicsSchemaTools.intToSdfPath(header.collider1)

            a0 = str(actor0_sdf)
            a1 = str(actor1_sdf)
            c0 = str(collider0_sdf)
            c1 = str(collider1_sdf)

            is_p0 = _is_player_path(a0) or _is_player_path(c0)
            is_p1 = _is_player_path(a1) or _is_player_path(c1)

            if not (is_p0 ^ is_p1):
                # Contact does not involve exactly one player body
                continue

            foe_candidates = [a1, c1] if is_p0 else [a0, c0]

            # Find matching brain
            brain = None
            for candidate in foe_candidates:
                if not candidate:
                    continue
                if candidate in rb_to_brain:
                    brain = rb_to_brain[candidate]
                    break
                for b_path, b in self._registered_brains.items():
                    if candidate == b_path or candidate.startswith(b_path + "/"):
                        brain = b
                        break
                    if b.rb_path and (candidate == b.rb_path or candidate.startswith(b.rb_path + "/")):
                        brain = b
                        break
                if brain:
                    break

            if not brain or not brain.is_alive:
                continue

            # Determine stomp vs lateral collision
            p_prim = stage.GetPrimAtPath(player_rb)
            p_pos = self._get_rigid_body_world_pos(p_prim, player_rb)
            f_pos = brain.current_pos
            is_stomp = (p_pos[1] > f_pos[1] + 25.0)

            # Check contact normal from contact_data if available
            offset = header.contact_data_offset
            num_data = header.num_contact_data
            if not is_stomp and num_data > 0 and offset < len(contact_data):
                c_norm = contact_data[offset].normal
                norm_y = c_norm[1] if is_p0 else -c_norm[1]
                if norm_y > 0.4:
                    is_stomp = True

            if is_stomp:
                stomped_brains.append(brain)
            else:
                collided_brains.append(brain)

        # Safely queue hits for destruction on the main thread (outside the PhysX simulation lock).
        # Accumulate into a local list first to avoid mutating shared state during PhysX callback iterations.
        local_hits: List[Tuple[str, bool]] = []
        for b in stomped_brains:
            local_hits.append((b.prim_path, True))
        for b in collided_brains:
            local_hits.append((b.prim_path, False))

        if local_hits:
            with self._hits_lock:
                for b_path, is_stomp in local_hits:
                    idx_found = -1
                    for idx, h in enumerate(self._pending_hits):
                        if h[0] == b_path:
                            idx_found = idx
                            break
                    if idx_found == -1:
                        self._pending_hits.append((b_path, is_stomp, False))
                    elif is_stomp and not self._pending_hits[idx_found][1]:
                        bounce_applied = self._pending_hits[idx_found][2] if len(self._pending_hits[idx_found]) > 2 else False
                        self._pending_hits[idx_found] = (b_path, True, bounce_applied)

    # -------------------------------------------------------------------------
    # Physics Simulation Step (O(N) iteration - ZERO per-frame traversals)
    # -------------------------------------------------------------------------
    def _on_physics_step(self, dt: float):
        if not self._is_simulating or not HAS_KIT or not self._active_brains:
            return

        stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
        if not stage:
            return

        stage_id = UsdUtils.StageCache.Get().GetId(stage).ToLongInt()
        sim_iface = get_physx_simulation_interface()
        if not sim_iface:
            return

        # Apply pending bounce impulse if player stomped a foe
        if self._pending_bounce:
            self._pending_bounce = False
            foe_pos = self._pending_bounce_foe_pos
            self._pending_bounce_foe_pos = None
            self._apply_player_bounce(stage, stage_id=stage_id, sim_iface=sim_iface, foe_pos=foe_pos, is_stomp=True)

        # 1. Query Player position once for all brains
        player_pos: Optional[Tuple[float, float, float]] = None
        player_rb = self._get_player_rb_path(stage)
        if player_rb:
            player_prim = stage.GetPrimAtPath(player_rb)
            player_pos = self._get_rigid_body_world_pos(player_prim, player_rb)

        # 2. Iterate registered AI brains
        dead_paths: List[str] = []

        for prim_path, brain in list(self._active_brains.items()):
            if not brain.is_alive:
                dead_paths.append(prim_path)
                continue

            rb_path = brain.rb_path
            if not rb_path:
                continue

            rb_prim = stage.GetPrimAtPath(rb_path)
            # Ensure neither the rigid body nor the actor root has been deactivated
            if (
                not rb_prim
                or not rb_prim.IsValid()
                or not rb_prim.IsActive()
                or not brain._prim
                or not brain._prim.IsValid()
                or not brain._prim.IsActive()
            ):
                dead_paths.append(prim_path)
                continue

            # Query real-time simulated world position from PhysX interface
            physx_iface = get_physx_interface()
            if physx_iface and hasattr(physx_iface, "get_rigidbody_transformation"):
                rb_data = physx_iface.get_rigidbody_transformation(rb_path)
                if not rb_data or not rb_data.get("ret_val", False):
                    # Rigid body not yet initialized or inactive in PhysX scene
                    continue
                p = rb_data.get("position")
                world_pos = (float(p[0]), float(p[1]), float(p[2])) if p is not None else (0.0, 0.0, 0.0)
            else:
                world_pos = self._compute_prim_usd_world_pos(rb_prim)

            brain.set_current_pos(world_pos)

            # Check if fallen below stage boundary (kill floor)
            if world_pos[1] < self.KILL_FLOOR_Y:
                with self._hits_lock:
                    if not any(h[0] == prim_path for h in self._pending_hits):
                        self._pending_hits.append((prim_path, False, True))
                dead_paths.append(prim_path)
                continue

            # Check player collision fallback (stomp or lateral contact)
            if player_pos is not None:
                dx = player_pos[0] - world_pos[0]
                dy = player_pos[1] - world_pos[1]
                dz = player_pos[2] - world_pos[2]
                horiz_dist = math.sqrt(dx * dx + dz * dz)
                dist_3d = math.sqrt(dx * dx + dy * dy + dz * dz)

                # Contact envelope: spheres have radius ~50cm each (center-to-center contact ~100cm)
                # Expand threshold to 135cm to reliably catch dynamic high-velocity collisions and post-solver bounces
                if dist_3d <= 135.0 or (horiz_dist < 120.0 and abs(dy) < 110.0):
                    is_stomp = (horiz_dist < 90.0 and 20.0 < dy < 150.0)
                    self._apply_player_bounce(
                        stage,
                        stage_id=stage_id,
                        sim_iface=sim_iface,
                        foe_pos=world_pos,
                        is_stomp=is_stomp,
                    )
                    with self._hits_lock:
                        idx_found = -1
                        for idx, h in enumerate(self._pending_hits):
                            if h[0] == prim_path:
                                idx_found = idx
                                break
                        if idx_found == -1:
                            self._pending_hits.append((prim_path, is_stomp, True))
                        else:
                            self._pending_hits[idx_found] = (prim_path, is_stomp or self._pending_hits[idx_found][1], True)
                    brain._is_alive = False
                    dead_paths.append(prim_path)
                    continue

            # Resolve target position for AI state machine (custom targetPrim relationship or active player)
            target_pos = player_pos
            if brain.target_prim_path:
                t_prim = stage.GetPrimAtPath(brain.target_prim_path)
                if t_prim and t_prim.IsValid() and t_prim.IsActive():
                    t_rb = _find_rigid_body_descendant(t_prim)
                    t_rb_path = t_rb.GetPath().pathString if t_rb else brain.target_prim_path
                    target_pos = self._get_rigid_body_world_pos(t_rb or t_prim, t_rb_path)

            # Update state machine and calculate horizontal actuation force
            state, (fx, fz) = brain.update_state_machine(dt, target_pos)

            # Ground traction check: ensure sphere is grounded before applying driving force
            # When airborne, eliminate driving force so gravity brings the sphere down cleanly without gliding
            is_grounded = self._check_foe_grounded(rb_prim, rb_path, world_pos, brain.radius)
            if not is_grounded:
                fx, fz = 0.0, 0.0

            # Apply force strictly at true center of mass (pure linear force with surface friction provides natural rolling torque)
            if abs(fx) > 1e-4 or abs(fz) > 1e-4:
                prim_id = PhysicsSchemaTools.sdfPathToInt(rb_path)
                force_vec = carb.Float3(fx, 0.0, fz)
                force_pos = carb.Float3(world_pos[0], world_pos[1], world_pos[2])
                sim_iface.apply_force_at_pos(stage_id, prim_id, force_vec, force_pos, "Force")

        # 3. Cleanly remove destroyed brains from active loop
        for dead_path in dead_paths:
            self._active_brains.pop(dead_path, None)

    def _apply_player_bounce(
        self,
        stage,
        stage_id: Optional[int] = None,
        sim_iface=None,
        foe_pos: Optional[Tuple[float, float, float]] = None,
        is_stomp: bool = False,
        force_bounce: bool = False,
    ):
        """
        Applies a calibrated arcade rebound velocity to the player rigid body:
        - Calculates horizontal rebound direction away from the foe (nx, nz)
        - Direct PhysX linear velocity kick (stomp: vy += 900 cm/s; lateral: vy += 350 cm/s, recoil 450 cm/s)
        - Strictly clamps resulting velocity to PhysicsManager max_linear_velocity
        - Applies via physx_iface, or gentle fallback impulse if interface unavailable
        """
        if not force_bounce:
            now = time.time()
            if now - self._last_player_bounce_time < 0.15:
                return
            self._last_player_bounce_time = now

        player_rb = self._get_player_rb_path(stage)
        if not player_rb:
            return
        try:
            player_prim = stage.GetPrimAtPath(player_rb) if stage else None
            if not (player_prim and player_prim.IsValid()):
                return

            world_pos = self._get_rigid_body_world_pos(player_prim, player_rb)

            # 1. Calculate horizontal rebound unit vector away from foe
            nx, nz = 0.0, 0.0
            if foe_pos is not None:
                dx = world_pos[0] - foe_pos[0]
                dz = world_pos[2] - foe_pos[2]
                h_dist = math.sqrt(dx * dx + dz * dz)
                if h_dist > 1e-4:
                    nx = dx / h_dist
                    nz = dz / h_dist
            if nx == 0.0 and nz == 0.0:
                nz = 1.0

            # Query max linear velocity from PhysicsManager or fallback with gameplay-safe floor
            phys_mgr = self._physics_mgr
            raw_max_vel = phys_mgr.max_linear_velocity if phys_mgr else 10000.0
            if 0.0 < raw_max_vel < 100.0:
                raw_max_vel *= 100.0
            max_linear_vel = max(raw_max_vel, 3000.0)

            # 2. Instantaneous Calibrated Arcade Velocity Kick via PhysX Interface
            physx_iface = get_physx_interface()
            new_vx, new_vy, new_vz = 0.0, 0.0, 0.0

            if physx_iface:
                cur_v = None
                for get_m in ("get_rigidbody_linear_velocity", "get_linear_velocity"):
                    if hasattr(physx_iface, get_m):
                        try:
                            cur_v = getattr(physx_iface, get_m)(player_rb)
                            if cur_v is not None:
                                break
                        except Exception:
                            pass

                vx = float(cur_v[0]) if cur_v is not None else 0.0
                vy = float(cur_v[1]) if cur_v is not None else 0.0
                vz = float(cur_v[2]) if cur_v is not None else 0.0

                if is_stomp:
                    # Mario-style stomp leap (~100cm height under -13000 cm/s^2 gravity)
                    new_vy = max(vy, 0.0) + 1600.0
                    new_vx = vx * 0.5 + nx * 400.0
                    new_vz = vz * 0.5 + nz * 400.0
                else:
                    # Snappy lateral knockback recoil and pop hop (~25cm height)
                    new_vy = max(vy, 0.0) + 650.0
                    new_vx = nx * 1000.0
                    new_vz = nz * 1000.0

                # Strictly clamp resulting speed to effective max linear velocity
                speed = math.sqrt(new_vx * new_vx + new_vy * new_vy + new_vz * new_vz)
                if speed > max_linear_vel and speed > 1e-4:
                    scale = max_linear_vel / speed
                    new_vx *= scale
                    new_vy *= scale
                    new_vz *= scale

                for set_m in ("set_rigidbody_linear_velocity", "set_linear_velocity"):
                    if hasattr(physx_iface, set_m):
                        try:
                            getattr(physx_iface, set_m)(player_rb, carb.Float3(new_vx, new_vy, new_vz))
                            break
                        except Exception:
                            pass

            # 3. PhysX solver impulse for physical momentum transfer and contact reaction
            if sim_iface is None:
                sim_iface = get_physx_simulation_interface()
            if stage_id is None and stage:
                try:
                    stage_id = UsdUtils.StageCache.Get().GetId(stage).ToLongInt()
                except Exception:
                    stage_id = None

            if sim_iface and stage_id is not None:
                prim_id = PhysicsSchemaTools.sdfPathToInt(player_rb)
                if is_stomp:
                    horiz_imp = 2000.0
                    vert_imp = 8000.0
                else:
                    horiz_imp = 5000.0
                    vert_imp = 3500.0
                bounce_vec = carb.Float3(nx * horiz_imp, vert_imp, nz * horiz_imp)
                force_pos = carb.Float3(world_pos[0], world_pos[1], world_pos[2])
                sim_iface.apply_force_at_pos(stage_id, prim_id, bounce_vec, force_pos, "Impulse")

            if carb:
                carb.log_info(
                    f"[hydragon.editor.core] Player arcade bounce applied: is_stomp={is_stomp}, new_v=({new_vx:.0f}, {new_vy:.0f}, {new_vz:.0f})"
                )
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to apply player bounce: {ex}")

    def _notify_game_manager_foe_destroyed(self, stage):
        """Notifies HydragonGameManager if present on stage to increment score and count."""
        if not self._cached_game_manager_path or not stage:
            return
        try:
            gm_prim = stage.GetPrimAtPath(self._cached_game_manager_path)
            if gm_prim and gm_prim.IsValid() and HydragonGameManager.is_applied(gm_prim):
                gm = HydragonGameManager(gm_prim)
                gm.record_foe_destroyed(points=100)
        except Exception:
            pass
