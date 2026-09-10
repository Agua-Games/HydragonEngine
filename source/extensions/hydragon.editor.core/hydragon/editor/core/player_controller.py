"""
Hydragon Engine - Player Physics Controller System

Handles physics-based rolling and jumping for the player character sphere
using Omniverse Kit's PhysX interface, carb.input, and the HydragonPlayerControllerAPI schema.
"""

import math
from typing import Optional, Set, Tuple

try:
    import carb
    import carb.input
    import omni.appwindow
    import omni.kit.app
    import omni.kit.commands
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

from .schemas import HydragonPlayerController, HydragonPhysicsManager


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


class HydragonPlayerControllerSystem:
    """Manages player input and physics actuation for rolling ball actors."""

    KILL_FLOOR_Y: float = -200.0  # Fall boundary in stage units (cm)

    _instance: Optional["HydragonPlayerControllerSystem"] = None

    def __init__(self):
        HydragonPlayerControllerSystem._instance = self
        self._is_active = False
        self._is_simulating = False
        self._physics_step_sub = None
        self._timeline_sub = None
        self._keyboard_sub = None
        self._app_update_sub = None
        self._keys_down: Set[int] = set()
        self._jump_requested = False
        self._jump_consumed = False
        self._cached_player_path: Optional[str] = None
        self._cached_rb_path: Optional[str] = None
        self._configured_rb_paths: Set[str] = set()
        self._last_world_pos: Tuple[float, float, float] = (0.0, 0.0, 0.0)
        self._spawn_pos: Tuple[float, float, float] = (0.0, 50.0, -1000.0)
        self._custom_respawn_target: Optional[Tuple[float, float, float]] = None
        self._needs_respawn: bool = False
        self._is_respawning: bool = False
        self._respawn_cooldown: float = 0.0
        self._physics_mgr: Optional[HydragonPhysicsManager] = None
        self._last_log_time = 0.0

    @classmethod
    def get_instance(cls) -> Optional["HydragonPlayerControllerSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

    def request_respawn(self, custom_spawn_pos: Optional[Tuple[float, float, float]] = None):
        """
        Public method to safely trigger a player respawn.
        Can be invoked by KillVolumeSystem, hazard triggers, or gameplay scripts.
        """
        if custom_spawn_pos is not None:
            self._custom_respawn_target = custom_spawn_pos
        else:
            self._custom_respawn_target = None
        self._needs_respawn = True

    def get_player_rb_path(self) -> Optional[str]:
        return self._cached_rb_path

    def get_player_root_path(self) -> Optional[str]:
        return self._cached_player_path

    def get_player_world_pos(self) -> Tuple[float, float, float]:
        if self._is_respawning or self._respawn_cooldown > 0.8:
            return (self._spawn_pos[0], max(120.0, self._spawn_pos[1]), self._spawn_pos[2])
        return self._last_world_pos

    def startup(self):
        """Initializes subscriptions to keyboard, timeline, app update, and PhysX simulation steps."""
        if not HAS_KIT:
            return

        self._subscribe_timeline()
        self._subscribe_keyboard()
        self._subscribe_app_update()
        self._subscribe_physics()
        self._is_active = True

        if carb:
            carb.log_info("[hydragon.editor.core] HydragonPlayerControllerSystem started.")

    def shutdown(self):
        """Releases all input, timeline, update, and physics subscriptions."""
        self._is_active = False
        self._is_simulating = False
        self._keys_down.clear()
        self._jump_requested = False
        self._jump_consumed = False
        self._cached_player_path = None
        self._cached_rb_path = None
        self._configured_rb_paths.clear()
        self._needs_respawn = False
        self._is_respawning = False
        self._respawn_cooldown = 0.0
        self._physics_mgr = None

        if not HAS_KIT:
            return

        if self._physics_step_sub:
            try:
                physx_iface = get_physx_interface()
                if physx_iface and hasattr(physx_iface, "unsubscribe_physics_step_events"):
                    physx_iface.unsubscribe_physics_step_events(self._physics_step_sub)
            except Exception:
                pass
            self._physics_step_sub = None

        if self._app_update_sub:
            self._app_update_sub = None

        if self._keyboard_sub:
            try:
                input_iface = carb.input.acquire_input_interface()
                appwindow = omni.appwindow.get_default_app_window()
                if appwindow:
                    keyboard = appwindow.get_keyboard()
                    if keyboard:
                        input_iface.unsubscribe_to_keyboard_events(keyboard, self._keyboard_sub)
            except Exception as e:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Error unsubscribing keyboard: {e}")
            self._keyboard_sub = None

        if self._timeline_sub:
            self._timeline_sub = None

        if carb:
            carb.log_info("[hydragon.editor.core] HydragonPlayerControllerSystem shutdown.")

    # -------------------------------------------------------------------------
    # Subscriptions Setup
    # -------------------------------------------------------------------------
    def _subscribe_timeline(self):
        try:
            timeline = omni.timeline.get_timeline_interface()
            stream = timeline.get_timeline_event_stream()
            self._timeline_sub = stream.create_subscription_to_pop(self._on_timeline_event)
            self._is_simulating = timeline.is_playing()
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to subscribe timeline events: {e}")

    def _subscribe_keyboard(self):
        try:
            input_iface = carb.input.acquire_input_interface()
            appwindow = omni.appwindow.get_default_app_window()
            if appwindow:
                keyboard = appwindow.get_keyboard()
                if keyboard:
                    self._keyboard_sub = input_iface.subscribe_to_keyboard_events(
                        keyboard, self._on_keyboard_event
                    )
                    if carb:
                        carb.log_info("[hydragon.editor.core] Keyboard subscribed successfully.")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to subscribe keyboard events: {e}")

    def _subscribe_app_update(self):
        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                self._on_app_update
            )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to subscribe app update events: {e}")

    def _subscribe_physics(self) -> bool:
        if self._physics_step_sub is not None:
            return True
        try:
            physx_iface = get_physx_interface()
            if physx_iface:
                self._physics_step_sub = physx_iface.subscribe_physics_step_events(self._on_physics_step)
                if carb:
                    carb.log_info("[hydragon.editor.core] Subscribed to PhysX step events successfully.")
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
                self._needs_respawn = False
                self._is_respawning = False
                self._respawn_cooldown = 0.0
                self._subscribe_physics()
                stage = omni.usd.get_context().get_stage()
                if stage:
                    self._physics_mgr = self._discover_physics_manager(stage)
                    self._ensure_physics_scene_settings(stage, self._physics_mgr)
                    player_prim = self.find_player_prim(stage)
                    if player_prim:
                        # Restore player input if previously disabled by victory/game over, unless menu is active
                        ctl = HydragonPlayerController(player_prim)
                        menu_active = False
                        try:
                            from .game_hud import HydragonGameHUD
                            hud = HydragonGameHUD.get_instance()
                            if hud and hud.is_menu_active:
                                menu_active = True
                        except Exception:
                            pass
                        ctl.input_enabled = not menu_active
                        rb_p = self.get_rigid_body_prim(player_prim)
                        if rb_p:
                            self._spawn_pos = self._compute_prim_usd_world_pos(rb_p)
                if carb:
                    carb.log_info("[hydragon.editor.core] Play mode started. Player Controller ACTIVE.")
            elif event_type == int(omni.timeline.TimelineEventType.PAUSE):
                self._is_simulating = False
                self._keys_down.clear()
                self._jump_requested = False
                self._jump_consumed = False
                if carb:
                    carb.log_info("[hydragon.editor.core] Simulation paused. Player Controller suspended.")
            elif event_type == int(omni.timeline.TimelineEventType.STOP):
                self._is_simulating = False
                self._keys_down.clear()
                self._jump_requested = False
                self._jump_consumed = False
                self._configured_rb_paths.clear()
                self._needs_respawn = False
                self._is_respawning = False
                self._respawn_cooldown = 0.0
                self._physics_mgr = None
                stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                if stage:
                    player_prim = self.find_player_prim(stage)
                    if player_prim:
                        ctl = HydragonPlayerController(player_prim)
                        ctl.input_enabled = True
                if carb:
                    carb.log_info("[hydragon.editor.core] Play mode stopped. Player Controller INACTIVE.")
        except Exception:
            pass

    def _on_app_update(self, e):
        """Runs every frame in Kit. Ensures PhysX subscription is active and provides fallback."""
        if not HAS_KIT:
            return

        # Keep simulation state in sync with timeline
        try:
            timeline = omni.timeline.get_timeline_interface()
            self._is_simulating = timeline.is_playing()
        except Exception:
            pass

        if not self._is_simulating:
            return

        # Safely execute player respawn on main thread outside PhysX simulation lock
        if self._needs_respawn:
            self._needs_respawn = False
            self._respawn_player_main_thread()

        # Safely unfreeze rigid body from kinematic back to dynamic after pose has been ingested by PhysX
        if self._is_respawning:
            self._is_respawning = False
            stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
            if stage:
                player_prim = self.find_player_prim(stage)
                if player_prim:
                    rb_prim = self.get_rigid_body_prim(player_prim)
                    if rb_prim and rb_prim.IsValid():
                        kin_attr = rb_prim.GetAttribute("physics:kinematicEnabled")
                        if kin_attr and kin_attr.IsValid():
                            kin_attr.Set(False)
                        # Re-enable CCD now that body is dynamic again if requested by PhysicsManager
                        target_ccd = self._physics_mgr.enable_ccd if self._physics_mgr else True
                        if target_ccd:
                            ccd_attr = rb_prim.GetAttribute("physxRigidBody:enableCCD")
                            if ccd_attr and ccd_attr.IsValid():
                                ccd_attr.Set(True)

        # Attempt to lazily acquire PhysX if not already subscribed
        if self._physics_step_sub is None:
            if not self._subscribe_physics():
                # Fallback: run step directly from app update
                dt = e.payload.get("dt", 1.0 / 60.0) if hasattr(e, "payload") else 1.0 / 60.0
                self._on_physics_step(dt)

    def _on_keyboard_event(self, event, *args, **kwargs) -> bool:
        if not self._is_simulating:
            return True

        try:
            key = event.input
            event_type = event.type

            jump_inputs = {
                carb.input.KeyboardInput.SPACE,
                carb.input.KeyboardInput.LEFT_SHIFT,
                carb.input.KeyboardInput.RIGHT_SHIFT,
                carb.input.KeyboardInput.LEFT_CONTROL,
                carb.input.KeyboardInput.RIGHT_CONTROL,
                carb.input.KeyboardInput.C,
            }

            if event_type == carb.input.KeyboardEventType.KEY_PRESS:
                self._keys_down.add(key)
                if key in jump_inputs:
                    self._jump_requested = True
                    self._jump_consumed = False
            elif event_type == carb.input.KeyboardEventType.KEY_REPEAT:
                self._keys_down.add(key)
            elif event_type == carb.input.KeyboardEventType.KEY_RELEASE:
                self._keys_down.discard(key)
                if key in jump_inputs:
                    self._jump_requested = False
                    self._jump_consumed = False
        except Exception:
            pass
        return True

    # -------------------------------------------------------------------------
    # Prim & RigidBody Discovery (Agnostic - Zero Hardcoded Paths or Names)
    # -------------------------------------------------------------------------
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

    def _ensure_physics_scene_settings(self, stage, phys_mgr: Optional[HydragonPhysicsManager]):
        """
        Ensures the PhysicsScene prim has PhysxSchema.PhysxSceneAPI applied with global
        Continuous Collision Detection (CCD) enabled and bounce threshold configured.
        NVIDIA PhysX ignores per-rigidbody CCD flags unless PhysxSceneAPI.enableCCD is True at the scene level!
        """
        if not stage:
            return

        target_ccd = phys_mgr.enable_ccd if phys_mgr else True
        target_bounce = phys_mgr.bounce_threshold if phys_mgr else 200.0

        # Discover PhysicsScene prim
        scene_prim = None
        for path in ("/World/PhysicsScene", "/PhysicsScene"):
            p = stage.GetPrimAtPath(path)
            if p and p.IsValid():
                scene_prim = p
                break

        if not scene_prim:
            for p in stage.Traverse():
                if p.IsValid() and p.GetTypeName() == "PhysicsScene":
                    scene_prim = p
                    break

        if not scene_prim:
            return

        try:
            if PhysxSchema and hasattr(PhysxSchema, "PhysxSceneAPI"):
                physx_scene = PhysxSchema.PhysxSceneAPI(scene_prim)
                if not physx_scene and hasattr(PhysxSchema.PhysxSceneAPI, "Apply"):
                    physx_scene = PhysxSchema.PhysxSceneAPI.Apply(scene_prim)
                if physx_scene:
                    ccd_attr = physx_scene.GetEnableCCDAttr()
                    if not ccd_attr or not ccd_attr.IsValid():
                        physx_scene.CreateEnableCCDAttr(target_ccd)
                    else:
                        ccd_attr.Set(target_ccd)

                    if hasattr(physx_scene, "GetBounceThresholdAttr"):
                        b_attr = physx_scene.GetBounceThresholdAttr()
                        if not b_attr or not b_attr.IsValid():
                            physx_scene.CreateBounceThresholdAttr(target_bounce)
                        else:
                            b_attr.Set(target_bounce)
            else:
                # Direct USD attribute fallback (headless / testing outside Kit)
                if not scene_prim.HasAttribute("physxScene:enableCCD"):
                    if HAS_PXR and Sdf:
                        scene_prim.CreateAttribute("physxScene:enableCCD", Sdf.ValueTypeNames.Bool).Set(target_ccd)
                    elif hasattr(scene_prim, "CreateAttribute"):
                        scene_prim.CreateAttribute("physxScene:enableCCD", None).Set(target_ccd)
                else:
                    attr = scene_prim.GetAttribute("physxScene:enableCCD")
                    if attr:
                        attr.Set(target_ccd)

                if hasattr(scene_prim, "HasAttribute") and scene_prim.HasAttribute("physxScene:bounceThreshold"):
                    b_attr = scene_prim.GetAttribute("physxScene:bounceThreshold")
                    if b_attr:
                        b_attr.Set(target_bounce)
                elif hasattr(scene_prim, "CreateAttribute"):
                    if HAS_PXR and Sdf:
                        scene_prim.CreateAttribute("physxScene:bounceThreshold", Sdf.ValueTypeNames.Float).Set(target_bounce)
                    else:
                        scene_prim.CreateAttribute("physxScene:bounceThreshold", None).Set(target_bounce)
        except Exception as err:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to configure PhysicsScene: {err}")

    def find_player_prim(self, stage) -> Optional[object]:
        """Locates the active player prim containing HydragonPlayerControllerAPI."""
        if not stage:
            return None

        # 1. Check cached path
        if self._cached_player_path:
            prim = stage.GetPrimAtPath(self._cached_player_path)
            if prim and prim.IsValid() and prim.IsActive() and HydragonPlayerController.is_applied(prim):
                return prim
            self._cached_player_path = None
            self._cached_rb_path = None

        # 2. Search stage once for any prim with HydragonPlayerControllerAPI
        for prim in stage.Traverse():
            if prim.IsValid() and prim.IsActive() and HydragonPlayerController.is_applied(prim):
                self._cached_player_path = prim.GetPath().pathString
                return prim

        return None

    def get_rigid_body_prim(self, player_prim) -> Optional[object]:
        """
        Finds the exact prim that has rigid body physics.
        Recursively inspects the prim and all its descendants without hardcoded naming assumptions.
        """
        if not player_prim or not player_prim.IsValid():
            return None

        if self._cached_rb_path:
            stage = player_prim.GetStage()
            rb_prim = stage.GetPrimAtPath(self._cached_rb_path)
            if rb_prim and rb_prim.IsValid() and rb_prim.IsActive() and _is_rigid_body(rb_prim):
                return rb_prim
            self._cached_rb_path = None

        # Check if player root itself is the rigid body
        if _is_rigid_body(player_prim):
            self._cached_rb_path = player_prim.GetPath().pathString
            return player_prim

        # Check standard child path first (geometry/ball_mesh)
        try:
            mesh_prim = player_prim.GetPrimAtPath("geometry/ball_mesh")
            if mesh_prim and mesh_prim.IsValid() and _is_rigid_body(mesh_prim):
                self._cached_rb_path = mesh_prim.GetPath().pathString
                return mesh_prim
        except Exception:
            pass

        # Search descendants for rigid body
        for child in player_prim.GetChildren():
            if _is_rigid_body(child):
                self._cached_rb_path = child.GetPath().pathString
                return child
        for child in player_prim.GetAllChildren():
            if _is_rigid_body(child):
                self._cached_rb_path = child.GetPath().pathString
                return child
            for grandchild in child.GetAllChildren():
                if _is_rigid_body(grandchild):
                    self._cached_rb_path = grandchild.GetPath().pathString
                    return grandchild

        return None

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

    def _get_rigid_body_world_pos(self, rb_prim, rb_path_str: str) -> Tuple[float, float, float]:
        """Queries real-time simulated world position from PhysX interface, falling back to USD."""
        if not HAS_KIT:
            return 0.0, 0.0, 0.0

        # 1. Query real-time simulated transform from PhysX C++ interface ONLY if prim has rigid body and is active
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

        # 2. Fallback to USD transform
        return self._compute_prim_usd_world_pos(rb_prim)

    def _ensure_rigid_body_damping(self, rb_prim, controller: HydragonPlayerController):
        """Ensures angular damping, linear damping, CCD, max velocities, and solver iterations."""
        if not rb_prim or not rb_prim.IsValid():
            return

        # Fetch Physics Manager settings or apply high-performance defaults
        phys_mgr = self._physics_mgr
        target_max_linear_vel = phys_mgr.max_linear_velocity if phys_mgr else 10000.0
        if 0.0 < target_max_linear_vel < 100.0:
            target_max_linear_vel *= 100.0
        target_max_ang_vel = (
            phys_mgr.max_angular_velocity
            if phys_mgr
            else (controller.max_angular_velocity if controller.max_angular_velocity > 0.0 else 3600.0)
        )
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

                    # Continuous Collision Detection (CCD) to prevent tunneling through thin floors/obstacles
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

    # -------------------------------------------------------------------------
    # Camera Vectors & Input Processing
    # -------------------------------------------------------------------------
    @staticmethod
    def compute_camera_axes(stage) -> Tuple[Tuple[float, float], Tuple[float, float]]:
        """
        Returns horizontal (X, Z) normalized unit vectors for (forward, right)
        based on active camera or viewport view.
        """
        forward = (0.0, 1.0)  # Default: +Z is forward in test stage
        right = (-1.0, 0.0)

        if not HAS_KIT or not stage:
            return forward, right

        # 1. Try to query real-time camera axes from HydragonCameraControllerSystem
        try:
            from .camera_controller import HydragonCameraControllerSystem
            cam_sys = HydragonCameraControllerSystem.get_instance()
            if cam_sys and cam_sys.is_active_and_simulating():
                return cam_sys.get_camera_axes()
        except Exception:
            pass

        # 2. Try ViewportCameraState
        try:
            from omni.kit.viewport.utility.camera_state import ViewportCameraState
            cam_state = ViewportCameraState()
            cam_pos = cam_state.get_position_world()
            cam_target = cam_state.get_target_world()

            dx = cam_target[0] - cam_pos[0]
            dz = cam_target[2] - cam_pos[2]
            length = math.sqrt(dx * dx + dz * dz)
            if length > 1e-4:
                fx = dx / length
                fz = dz / length
                rx = -fz
                rz = fx
                return (fx, fz), (rx, rz)
        except Exception:
            pass

        # 3. Fallback to USD FollowCamera prim transform
        try:
            cam_prim = stage.GetPrimAtPath("/World/FollowCamera")
            if cam_prim and cam_prim.IsValid():
                xformable = UsdGeom.Xformable(cam_prim)
                tf = xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
                local_fwd = Gf.Vec4d(0.0, 0.0, -1.0, 0.0)
                world_fwd = local_fwd * tf
                dx = world_fwd[0]
                dz = world_fwd[2]
                length = math.sqrt(dx * dx + dz * dz)
                if length > 1e-4:
                    fx = dx / length
                    fz = dz / length
                    rx = -fz
                    rz = fx
                    return (fx, fz), (rx, rz)
        except Exception:
            pass

        return forward, right

    def _compute_input_vector(self) -> Tuple[float, float]:
        """Calculates raw input vector (-1..1) for (forward, right) from held keys and polling."""
        if not HAS_KIT:
            return 0.0, 0.0

        fwd_input = 0.0
        right_input = 0.0

        # 1. From event set
        fwd_keys = {carb.input.KeyboardInput.W, carb.input.KeyboardInput.UP}
        back_keys = {carb.input.KeyboardInput.S, carb.input.KeyboardInput.DOWN}
        left_keys = {carb.input.KeyboardInput.A, carb.input.KeyboardInput.LEFT}
        right_keys = {carb.input.KeyboardInput.D, carb.input.KeyboardInput.RIGHT}

        if any(k in self._keys_down for k in fwd_keys):
            fwd_input += 1.0
        if any(k in self._keys_down for k in back_keys):
            fwd_input -= 1.0
        if any(k in self._keys_down for k in right_keys):
            right_input += 1.0
        if any(k in self._keys_down for k in left_keys):
            right_input -= 1.0

        # 2. Polling fallback via carb.input
        try:
            input_iface = carb.input.acquire_input_interface()
            appwindow = omni.appwindow.get_default_app_window()
            if appwindow:
                keyboard = appwindow.get_keyboard()
                if keyboard and hasattr(input_iface, "get_keyboard_value"):
                    if input_iface.get_keyboard_value(keyboard, carb.input.KeyboardInput.W) > 0.5:
                        fwd_input += 1.0
                    if input_iface.get_keyboard_value(keyboard, carb.input.KeyboardInput.S) > 0.5:
                        fwd_input -= 1.0
                    if input_iface.get_keyboard_value(keyboard, carb.input.KeyboardInput.D) > 0.5:
                        right_input += 1.0
                    if input_iface.get_keyboard_value(keyboard, carb.input.KeyboardInput.A) > 0.5:
                        right_input -= 1.0
        except Exception:
            pass

        # Clamp and normalize diagonal input
        fwd_input = max(-1.0, min(1.0, fwd_input))
        right_input = max(-1.0, min(1.0, right_input))

        length = math.sqrt(fwd_input * fwd_input + right_input * right_input)
        if length > 1.0:
            fwd_input /= length
            right_input /= length

        return fwd_input, right_input

    # -------------------------------------------------------------------------
    # Physics Update Step
    # -------------------------------------------------------------------------
    def _on_physics_step(self, dt: float):
        if not self._is_simulating or not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage()
        if not stage:
            return

        player_prim = self.find_player_prim(stage)
        if not player_prim:
            return

        controller = HydragonPlayerController(player_prim)
        if not controller.is_active or not controller.input_enabled:
            return

        # Find the actual RigidBody prim (may be ball_mesh child)
        rb_prim = self.get_rigid_body_prim(player_prim)
        if not rb_prim:
            return

        stage_id = UsdUtils.StageCache.Get().GetId(stage).ToLongInt()
        rb_path_str = rb_prim.GetPath().pathString
        prim_id = PhysicsSchemaTools.sdfPathToInt(rb_path_str)

        # Configure damping and stability clamp once per play session
        if rb_path_str not in self._configured_rb_paths:
            self._ensure_rigid_body_damping(rb_prim, controller)
            self._configured_rb_paths.add(rb_path_str)

        sim_iface = get_physx_simulation_interface()

        # Decrement respawn cooldown
        if self._respawn_cooldown > 0.0:
            self._respawn_cooldown = max(0.0, self._respawn_cooldown - dt)

        # Query real-time simulated world position for applying force directly at center of mass
        world_pos = self._get_rigid_body_world_pos(rb_prim, rb_path_str)
        self._last_world_pos = world_pos

        # Floor drop / tunneling safeguard: if player falls below arena boundary, queue respawn
        # Suppressed during respawn cooldown grace period to avoid endless re-triggering loops
        if self._respawn_cooldown <= 0.0 and world_pos[1] < self.KILL_FLOOR_Y:
            self._needs_respawn = True
            return

        force_pos = carb.Float3(world_pos[0], world_pos[1], world_pos[2])

        # 1. Rolling Movement
        fwd_input, right_input = self._compute_input_vector()
        if abs(fwd_input) > 1e-4 or abs(right_input) > 1e-4:
            (fx, fz), (rx, rz) = self.compute_camera_axes(stage)
            move_force = controller.move_force
            # Ensure sensible force magnitude for centimeter-scale scene
            if move_force < 1000.0:
                move_force = 25000.0

            world_move_x = (fx * fwd_input + rx * right_input) * move_force
            world_move_z = (fz * fwd_input + rz * right_input) * move_force

            force_vec = carb.Float3(world_move_x, 0.0, world_move_z)
            sim_iface.apply_force_at_pos(
                stage_id, prim_id, force_vec, force_pos, "Force"
            )

            import time
            now = time.time()
            if now - self._last_log_time > 1.5:
                self._last_log_time = now
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Rolling {rb_path_str}: force=({world_move_x:.0f}, {world_move_z:.0f})"
                    )

        # 2. Jump Handling (Space, Shift, or Ctrl)
        if self._jump_requested and not self._jump_consumed:
            can_jump = controller.can_jump_in_air
            if not can_jump:
                can_jump = self._check_is_grounded(stage, rb_prim)

            if can_jump:
                jump_impulse = controller.jump_impulse
                if jump_impulse <= 0.0:
                    jump_impulse = 2500.0

                jump_force = carb.Float3(0.0, jump_impulse, 0.0)
                sim_iface.apply_force_at_pos(
                    stage_id, prim_id, jump_force, force_pos, "Impulse"
                )
                self._jump_consumed = True
                if carb:
                    carb.log_info(f"[hydragon.editor.core] Player Jump applied on {rb_path_str}")
            else:
                self._jump_consumed = True
                if carb:
                    carb.log_info("[hydragon.editor.core] Player Jump blocked: not grounded")

    def _check_is_grounded(self, stage, rb_prim) -> bool:
        """Performs downward raycast from true simulated position to check if resting on ground."""
        try:
            rb_path = rb_prim.GetPath().pathString
            player_root_path = self._cached_player_path or ""

            radius = 50.0
            sphere_geom = UsdGeom.Sphere(rb_prim)
            if sphere_geom:
                radius_attr = sphere_geom.GetRadiusAttr()
                if radius_attr.IsValid():
                    radius = float(radius_attr.Get())

            world_pos = self._get_rigid_body_world_pos(rb_prim, rb_path)
            origin = carb.Float3(world_pos[0], world_pos[1], world_pos[2])
            down_dir = carb.Float3(0.0, -1.0, 0.0)
            max_dist = radius + 15.0  # 15 cm tolerance

            sq_iface = get_physx_scene_query_interface()
            valid_hits = []

            def _on_hit(hit):
                hit_rb = str(getattr(hit, "rigid_body", "") or "")
                hit_col = str(getattr(hit, "collision", "") or "")
                # Ignore self
                if hit_rb.startswith(player_root_path) or hit_col.startswith(player_root_path):
                    return True
                if hit_rb == rb_path or hit_col == rb_path:
                    return True

                hit_pos = getattr(hit, "position", None)
                if hit_pos is not None:
                    dist = math.sqrt(
                        (hit_pos[0] - origin[0]) ** 2
                        + (hit_pos[1] - origin[1]) ** 2
                        + (hit_pos[2] - origin[2]) ** 2
                    )
                    if dist <= max_dist:
                        valid_hits.append(hit)
                else:
                    valid_hits.append(hit)
                return True

            sq_iface.raycast_all(origin, down_dir, max_dist, _on_hit)
            if valid_hits:
                return True

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Ground check error: {e}")
        return False

    def _respawn_player_main_thread(self):
        """
        Safely teleports player back to spawn on the main thread outside the PhysX simulation lock.
        Temporarily sets kinematicEnabled = True to snap the PhysX actor and eliminate momentum,
        releasing it on the next frame with zero accumulated velocities.
        """
        if not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
        if not stage:
            return

        player_prim = self.find_player_prim(stage)
        if not player_prim:
            return

        rb_prim = self.get_rigid_body_prim(player_prim)
        rb_path_str = rb_prim.GetPath().pathString if rb_prim else None

        target_pos = self._custom_respawn_target if self._custom_respawn_target is not None else self._spawn_pos
        self._custom_respawn_target = None
        spawn_x, spawn_y, spawn_z = target_pos
        # Safe spawn height (at least 120cm above floor) to prevent ground box interpenetration and tunneling
        safe_y = max(120.0, spawn_y)
        self._last_world_pos = (spawn_x, safe_y, spawn_z)

        try:
            # 1. Temporarily freeze rigid body as kinematic in PhysX to wipe all velocities and lock pose
            if rb_prim and rb_prim.IsValid():
                # PhysX forbids CCD on kinematic bodies; disable CCD before enabling kinematic mode
                ccd_attr = rb_prim.GetAttribute("physxRigidBody:enableCCD")
                if ccd_attr and ccd_attr.IsValid() and ccd_attr.Get():
                    ccd_attr.Set(False)
                kin_attr = rb_prim.GetAttribute("physics:kinematicEnabled")
                if not kin_attr or not kin_attr.IsValid():
                    kin_attr = rb_prim.CreateAttribute("physics:kinematicEnabled", Sdf.ValueTypeNames.Bool)
                kin_attr.Set(True)

            # 2. Teleport parent Player prim in USD using Kit command (with direct USD XformOp fallback)
            target_prim = player_prim
            if target_prim and target_prim.IsValid():
                cmd_ok = False
                try:
                    if HAS_KIT and hasattr(omni, "kit") and hasattr(omni.kit, "commands"):
                        cmd_ok = omni.kit.commands.execute(
                            "TransformPrimSRTCommand",
                            path=target_prim.GetPath().pathString,
                            new_translation=Gf.Vec3d(spawn_x, safe_y, spawn_z),
                        )
                except Exception:
                    cmd_ok = False

                if not cmd_ok:
                    xformable = UsdGeom.Xformable(target_prim)
                    for op in xformable.GetOrderedXformOps():
                        if op.GetOpType() == UsdGeom.XformOp.TypeTranslate:
                            op.Set(Gf.Vec3d(spawn_x, safe_y, spawn_z))
                            break

            # 3. Reset child rigid body local translation and rotation to origin
            if rb_prim and rb_prim != target_prim and rb_prim.IsValid():
                rb_cmd_ok = False
                try:
                    if HAS_KIT and hasattr(omni, "kit") and hasattr(omni.kit, "commands"):
                        rb_cmd_ok = omni.kit.commands.execute(
                            "TransformPrimSRTCommand",
                            path=rb_prim.GetPath().pathString,
                            new_translation=Gf.Vec3d(0.0, 0.0, 0.0),
                            new_rotation_euler=Gf.Vec3d(0.0, 0.0, 0.0),
                        )
                except Exception:
                    rb_cmd_ok = False

                if not rb_cmd_ok:
                    rb_xformable = UsdGeom.Xformable(rb_prim)
                    for op in rb_xformable.GetOrderedXformOps():
                        if op.GetOpType() == UsdGeom.XformOp.TypeTranslate:
                            typeName = type(op.Get()) if op.Get() is not None else Gf.Vec3d
                            op.Set(typeName(0.0, 0.0, 0.0))
                        elif op.GetOpType() == UsdGeom.XformOp.TypeRotateXYZ:
                            op.Set(Gf.Vec3f(0.0, 0.0, 0.0))
                        elif op.GetOpType() == UsdGeom.XformOp.TypeOrient:
                            op.Set(Gf.Quatf(1.0, 0.0, 0.0, 0.0))

            # 4. Reset velocities in USD RigidBodyAPI
            if rb_prim and rb_prim.IsValid() and hasattr(rb_prim, "GetAttribute"):
                vel_attr = rb_prim.GetAttribute("physics:velocity")
                if vel_attr and vel_attr.IsValid():
                    vel_attr.Set(Gf.Vec3f(0.0, 0.0, 0.0))
                ang_attr = rb_prim.GetAttribute("physics:angularVelocity")
                if ang_attr and ang_attr.IsValid():
                    ang_attr.Set(Gf.Vec3f(0.0, 0.0, 0.0))

            # 5. Mark respawn in progress and apply grace period cooldown
            self._is_respawning = True
            self._respawn_cooldown = 1.0

            if carb:
                carb.log_info(
                    f"[hydragon.editor.core] Player safely respawned at ({spawn_x:.1f}, {safe_y:.1f}, {spawn_z:.1f})."
                )

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Player respawn error: {e}")
