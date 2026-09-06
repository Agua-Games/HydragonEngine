"""
Hydragon Engine - Foes AI Physics Controller System

Component-driven AI system for adversary actors using OpenUSD API schemas
(HydragonChaserAIAPI, HydragonActorAPI), PhysX physics simulation, and
an ECS state machine (Patrol <-> Chase).
"""

import math
import random
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

from .schemas import HydragonActor, HydragonChaserAI, HydragonGameManager, HydragonPlayerController


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

        self._state: str = "Patrol"
        self._patrol_timer: float = 0.0
        self._patrol_duration: float = random.uniform(2.5, 4.5)
        self._is_alive: bool = True
        self._last_stomp_check_time: float = 0.0

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

    def _get_effective_force(self, force_val: float, default_mag: float = 18000.0) -> float:
        """Ensures force magnitude is sufficient for centimeter-scale PhysX rigid bodies."""
        if force_val >= 1000.0:
            return force_val
        if force_val > 0.0:
            return force_val * 50.0
        return default_mag

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
                chase_mag = 18000.0
                if self._schema:
                    chase_mag = self._get_effective_force(self._schema.chase_force, default_mag=18000.0)
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
                nx = w_dx / dist_to_waypoint
                nz = w_dz / dist_to_waypoint
                patrol_mag = 8000.0
                if self._schema:
                    patrol_mag = self._get_effective_force(self._schema.patrol_force, default_mag=8000.0)
                force_x = nx * patrol_mag
                force_z = nz * patrol_mag

        return self._state, (force_x, force_z)

    def destroy(self, reason: str = ""):
        """
        Deactivates this entity cleanly according to OpenUSD ECS guidelines.
        Sets prim.SetActive(False) to immediately cease PhysX simulation and Hydra draw calls.
        """
        if not self._is_alive:
            return

        self._is_alive = False
        if self._actor:
            self._actor.is_alive = False

        if HAS_KIT and self._prim and hasattr(self._prim, "IsValid") and self._prim.IsValid():
            try:
                self._prim.SetActive(False)
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

    _instance: Optional["HydragonFoesControllerSystem"] = None

    def __init__(self):
        HydragonFoesControllerSystem._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False
        self._physics_step_sub = None
        self._timeline_sub = None
        self._app_update_sub = None

        self._active_brains: Dict[str, HydragonAIBrain] = {}
        self._cached_player_rb_path: Optional[str] = None
        self._cached_game_manager_path: Optional[str] = None
        self._configured_rb_paths: set = set()
        self._last_log_time: float = 0.0

    @classmethod
    def get_instance(cls) -> Optional["HydragonFoesControllerSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

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
        self._configured_rb_paths.clear()
        self._cached_player_rb_path = None
        self._cached_game_manager_path = None

        if not HAS_KIT:
            HydragonFoesControllerSystem._instance = None
            return

        if self._physics_step_sub:
            self._physics_step_sub = None
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
                return True
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
                self._active_brains.clear()
                self._configured_rb_paths.clear()
                self._cached_player_rb_path = None
                self._cached_game_manager_path = None
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

        if self._physics_step_sub is None:
            if not self._subscribe_physics():
                dt = e.payload.get("dt", 1.0 / 60.0) if hasattr(e, "payload") else 1.0 / 60.0
                self._on_physics_step(dt)

    # -------------------------------------------------------------------------
    # One-Time Discovery on Simulation Start
    # -------------------------------------------------------------------------
    def _get_player_rb_path(self, stage) -> Optional[str]:
        """Locates the player rigid body path agnostically without hardcoded paths."""
        if self._cached_player_rb_path:
            return self._cached_player_rb_path

        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys:
                p_rb = p_sys.get_player_rb_path()
                if p_rb:
                    self._cached_player_rb_path = p_rb
                    return p_rb
        except Exception:
            pass

        if stage:
            for prim in stage.Traverse():
                if prim.IsValid() and prim.IsActive() and HydragonPlayerController.is_applied(prim):
                    p_rb_prim = _find_rigid_body_descendant(prim)
                    if p_rb_prim:
                        self._cached_player_rb_path = p_rb_prim.GetPath().pathString
                        return self._cached_player_rb_path

        return None

    def _discover_entities_once(self, stage):
        """
        Discovers all adversary entities, players, and game manager once on simulation start.
        NEVER run this per-frame.
        """
        self._active_brains.clear()
        self._configured_rb_paths.clear()
        self._cached_player_rb_path = None
        self._cached_game_manager_path = None

        if not stage:
            return

        # Query player rigid body from player controller singleton if ready
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys:
                self._cached_player_rb_path = p_sys.get_player_rb_path()
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

                    # Initialize origin position directly from authored USD transform (never call PhysX on PLAY before simulation steps)
                    origin = self._compute_prim_usd_world_pos(rb_prim)
                    brain.set_origin_pos(origin)
                else:
                    brain.rb_path = None
                    if carb:
                        carb.log_warn(f"[hydragon.editor.core] Foe '{prim_path}' has no rigid body descendant.")

                self._active_brains[prim_path] = brain

            # Discover Player RigidBody for target tracking if not already resolved
            if self._cached_player_rb_path is None:
                if HydragonPlayerController.is_applied(prim):
                    rb_p = _find_rigid_body_descendant(prim)
                    if rb_p:
                        self._cached_player_rb_path = rb_p.GetPath().pathString

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

    def _ensure_rigid_body_damping(self, rb_prim):
        """Applies angular damping, linear damping, max velocities, CCD, and solver iterations."""
        if not HAS_KIT or not rb_prim or not rb_prim.IsValid():
            return

        rb_path = rb_prim.GetPath().pathString
        if rb_path in self._configured_rb_paths:
            return
        self._configured_rb_paths.add(rb_path)

        try:
            if UsdPhysics:
                rb_api = UsdPhysics.RigidBodyAPI(rb_prim)
                if rb_api:
                    ang_attr = rb_api.GetAngularDampingAttr()
                    if not ang_attr or not ang_attr.IsValid():
                        rb_api.CreateAngularDampingAttr(1.0)
                    elif ang_attr.Get() is None or ang_attr.Get() < 0.1:
                        ang_attr.Set(1.0)

                    lin_attr = rb_api.GetLinearDampingAttr()
                    if not lin_attr or not lin_attr.IsValid():
                        rb_api.CreateLinearDampingAttr(0.2)
                    elif lin_attr.Get() is None or lin_attr.Get() < 0.05:
                        lin_attr.Set(0.2)
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
                        physx_rb.CreateMaxAngularVelocityAttr(25.0)
                    elif max_ang_attr.Get() is None or max_ang_attr.Get() <= 0.0:
                        max_ang_attr.Set(25.0)

                    # Max Linear Velocity clamp to prevent supersonic physics explosions
                    if hasattr(physx_rb, "GetMaxLinearVelocityAttr"):
                        max_lin_attr = physx_rb.GetMaxLinearVelocityAttr()
                        if not max_lin_attr or not max_lin_attr.IsValid():
                            physx_rb.CreateMaxLinearVelocityAttr(3000.0)
                        elif max_lin_attr.Get() is None or max_lin_attr.Get() <= 0.0 or max_lin_attr.Get() > 3000.0:
                            max_lin_attr.Set(3000.0)
                    elif rb_prim.HasAttribute("physxRigidBody:maxLinearVelocity"):
                        rb_prim.GetAttribute("physxRigidBody:maxLinearVelocity").Set(3000.0)

                    # Solver iteration counts for stable constraint convergence under compression
                    if hasattr(physx_rb, "GetSolverPositionIterationCountAttr"):
                        pos_iter_attr = physx_rb.GetSolverPositionIterationCountAttr()
                        if not pos_iter_attr or not pos_iter_attr.IsValid():
                            physx_rb.CreateSolverPositionIterationCountAttr(16)
                        elif pos_iter_attr.Get() is None or pos_iter_attr.Get() < 16:
                            pos_iter_attr.Set(16)
                    elif rb_prim.HasAttribute("physxRigidBody:solverPositionIterationCount"):
                        rb_prim.GetAttribute("physxRigidBody:solverPositionIterationCount").Set(16)

                    if hasattr(physx_rb, "GetSolverVelocityIterationCountAttr"):
                        vel_iter_attr = physx_rb.GetSolverVelocityIterationCountAttr()
                        if not vel_iter_attr or not vel_iter_attr.IsValid():
                            physx_rb.CreateSolverVelocityIterationCountAttr(4)
                        elif vel_iter_attr.Get() is None or vel_iter_attr.Get() < 4:
                            vel_iter_attr.Set(4)
                    elif rb_prim.HasAttribute("physxRigidBody:solverVelocityIterationCount"):
                        rb_prim.GetAttribute("physxRigidBody:solverVelocityIterationCount").Set(4)

                    # Continuous Collision Detection (CCD) to prevent floor tunneling
                    ccd_attr = physx_rb.GetEnableCCDAttr()
                    if not ccd_attr or not ccd_attr.IsValid():
                        physx_rb.CreateEnableCCDAttr(True)
                    elif not ccd_attr.Get():
                        ccd_attr.Set(True)
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
    # Physics Simulation Step (O(N) iteration - ZERO per-frame traversals)
    # -------------------------------------------------------------------------
    def _on_physics_step(self, dt: float):
        if not self._is_simulating or not HAS_KIT or not self._active_brains:
            return

        stage = omni.usd.get_context().get_stage()
        if not stage:
            return

        stage_id = UsdUtils.StageCache.Get().GetId(stage).ToLongInt()
        sim_iface = get_physx_simulation_interface()
        if not sim_iface:
            return

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
                world_pos = self._get_rigid_body_world_pos(rb_prim, rb_path)

            brain.set_current_pos(world_pos)

            # Check if fallen below stage boundary (kill floor)
            if world_pos[1] < -300.0:
                brain.destroy(reason="fell_off_arena")
                self._notify_game_manager_foe_destroyed(stage)
                dead_paths.append(prim_path)
                continue

            # Resolve target position (custom targetPrim relationship or active player)
            target_pos = player_pos
            if brain.target_prim_path:
                t_prim = stage.GetPrimAtPath(brain.target_prim_path)
                if t_prim and t_prim.IsValid() and t_prim.IsActive():
                    t_rb = _find_rigid_body_descendant(t_prim)
                    t_rb_path = t_rb.GetPath().pathString if t_rb else brain.target_prim_path
                    target_pos = self._get_rigid_body_world_pos(t_rb or t_prim, t_rb_path)

            # Check player collision (stomp or lateral contact)
            if target_pos is not None:
                dx = target_pos[0] - world_pos[0]
                dy = target_pos[1] - world_pos[1]
                dz = target_pos[2] - world_pos[2]
                horiz_dist = math.sqrt(dx * dx + dz * dz)
                dist_3d = math.sqrt(dx * dx + dy * dy + dz * dz)

                # Contact envelope: spheres have radius ~50cm each (center-to-center contact ~100cm)
                if dist_3d <= 115.0 or (horiz_dist < 100.0 and abs(dy) < 90.0):
                    is_stomp = (horiz_dist < 80.0 and 30.0 < dy < 130.0)
                    reason = "stomped_by_player" if is_stomp else "player_collision"
                    brain.destroy(reason=reason)
                    self._notify_game_manager_foe_destroyed(stage)
                    if is_stomp:
                        self._apply_player_bounce(stage, stage_id, sim_iface)
                    dead_paths.append(prim_path)
                    continue

            # Update state machine and calculate horizontal actuation force
            state, (fx, fz) = brain.update_state_machine(dt, target_pos)

            # Apply force strictly at true center of mass
            if abs(fx) > 1e-4 or abs(fz) > 1e-4:
                prim_id = PhysicsSchemaTools.sdfPathToInt(rb_path)
                force_vec = carb.Float3(fx, 0.0, fz)
                force_pos = carb.Float3(world_pos[0], world_pos[1], world_pos[2])
                sim_iface.apply_force_at_pos(stage_id, prim_id, force_vec, force_pos, "Force")

        # 3. Cleanly remove destroyed brains from active loop
        for dead_path in dead_paths:
            self._active_brains.pop(dead_path, None)

    def _apply_player_bounce(self, stage, stage_id: int, sim_iface):
        """Applies an upward vertical impulse to the player rigid body upon stomping a foe."""
        player_rb = self._get_player_rb_path(stage)
        if not player_rb:
            return
        try:
            player_prim = stage.GetPrimAtPath(player_rb)
            if player_prim and player_prim.IsValid():
                world_pos = self._get_rigid_body_world_pos(player_prim, player_rb)
                prim_id = PhysicsSchemaTools.sdfPathToInt(player_rb)
                bounce_vec = carb.Float3(0.0, 25000.0, 0.0)
                force_pos = carb.Float3(world_pos[0], world_pos[1], world_pos[2])
                sim_iface.apply_force_at_pos(stage_id, prim_id, bounce_vec, force_pos, "Impulse")
                if carb:
                    carb.log_info("[hydragon.editor.core] Player bounced off foe!")
        except Exception:
            pass

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
