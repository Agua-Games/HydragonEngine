"""
Hydragon Follow Camera Controller System
Provides a third-person spring-arm orbit camera that smoothly follows the player ball,
handles mouse-driven pitch and yaw orbiting, mouse wheel zoom, anti-clipping obstacle probes,
and binds to the active viewport in Omniverse Kit.
"""

import math
import typing
from typing import Optional, Tuple

try:
    import carb
    import carb.input
    import omni.appwindow
    import omni.kit.app
    import omni.timeline
    import omni.usd
    from pxr import Gf, Sdf, Usd, UsdGeom
    from omni.physx import get_physx_interface, get_physx_scene_query_interface
    HAS_KIT = True
except ImportError:
    carb = None
    omni = None
    HAS_KIT = False

from .schemas import HydragonFollowCamera

# Game volumes (Force/Kill) author a PhysX collider with no geometry inside it, so it must never
# count as an obstacle for us. A scene query cannot be told to skip it - see
# `volume_triggers.is_trigger_collider` - so every one of our raycasts filters it in its own
# reporting callback instead. Guarded so this optional dependency cannot take the camera down with
# it; a failure here just means the probe treats volumes as obstacles again.
try:
    from .volume_triggers import is_trigger_collider
except Exception:  # pragma: no cover - defensive
    def is_trigger_collider(path: str) -> bool:
        return False


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


class HydragonCameraControllerSystem:
    """
    Spring-arm orbit camera system for Hydragon Engine.
    Controls `/World/FollowCamera` during simulation.
    """

    _instance: Optional["HydragonCameraControllerSystem"] = None

    def __init__(self):
        HydragonCameraControllerSystem._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False

        # Target tracking state
        self._cached_cam_path: Optional[str] = "/World/FollowCamera"
        self._cached_target_path: Optional[str] = None
        self._cached_target_rb_path: Optional[str] = None

        # Spring arm / orbit parameters (in cm)
        self._arm_length: float = 800.0
        self._min_arm_length: float = 150.0
        self._max_arm_length: float = 3000.0
        self._pitch: float = 25.0
        self._yaw: float = 0.0
        self._min_pitch: float = -20.0
        self._max_pitch: float = 80.0
        self._mouse_sensitivity: float = 0.15
        self._collision_offset: float = 30.0
        self._elevation_offset: float = 35.0  # Look slightly above ball center

        # Lag Damping and Inertia state (ease in/out)
        self._current_focus_pos: Optional[Tuple[float, float, float]] = None
        self._position_damping: float = 8.0  # Smooth position follow damping
        self._rotation_damping: float = 12.0  # Smooth rotational orbit inertia
        self._target_yaw: float = 0.0
        self._target_pitch: float = 25.0

        # Mouse input tracking
        self._mouse_sub = None
        self._is_rmb_down: bool = False
        self._is_mmb_down: bool = False
        self._last_mouse_pos: Optional[Tuple[float, float]] = None

        # Event subscriptions
        self._app_update_sub = None
        self._timeline_sub = None

        # Applied transform dirty checking (skip USD writes when camera hasn't moved)
        self._last_applied_pos: Optional[Tuple[float, float, float]] = None
        self._last_applied_rot: Optional[Tuple[float, float, float]] = None

        # Raycast collision probe caching
        self._last_probe_focus: Optional[Tuple[float, float, float]] = None
        self._last_probe_desired: Optional[Tuple[float, float, float]] = None
        self._last_probe_result: Optional[Tuple[float, float, float]] = None

        # Viewport restoration
        self._previous_viewport_cam: Optional[str] = None
        self._viewport_bound: bool = False

    @classmethod
    def get_instance(cls) -> Optional["HydragonCameraControllerSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

    def get_camera_axes(self) -> Tuple[Tuple[float, float], Tuple[float, float]]:
        """
        Returns horizontal (X, Z) normalized unit vectors for (forward, right)
        in camera view space.
        """
        rad_yaw = math.radians(self._yaw)
        fx = math.sin(rad_yaw)
        fz = math.cos(rad_yaw)
        rx = -fz
        rz = fx
        return (fx, fz), (rx, rz)

    # -------------------------------------------------------------------------
    # Lifecycle
    # -------------------------------------------------------------------------
    def startup(self):
        if not HAS_KIT:
            return

        self._subscribe_timeline()
        self._subscribe_app_update()
        self._subscribe_mouse()
        self._is_active = True
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonCameraControllerSystem started.")

    def shutdown(self):
        self._restore_viewport_camera()
        self._set_camera_lock(False)
        self._app_update_sub = None
        self._timeline_sub = None
        if self._mouse_sub:
            try:
                input_iface = carb.input.acquire_input_interface()
                appwindow = omni.appwindow.get_default_app_window()
                if appwindow:
                    mouse = appwindow.get_mouse()
                    if mouse:
                        input_iface.unsubscribe_to_mouse_events(mouse, self._mouse_sub)
            except Exception:
                pass
            self._mouse_sub = None
        self._is_simulating = False
        self._is_active = False
        self._current_focus_pos = None
        self._last_mouse_pos = None
        if HydragonCameraControllerSystem._instance is self:
            HydragonCameraControllerSystem._instance = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonCameraControllerSystem shutdown.")

    # -------------------------------------------------------------------------
    # Subscriptions
    # -------------------------------------------------------------------------
    def _subscribe_timeline(self):
        try:
            timeline = omni.timeline.get_timeline_interface()
            if timeline:
                stream = timeline.get_timeline_event_stream()
                self._timeline_sub = stream.create_subscription_to_pop(self._on_timeline_event)
                self._is_simulating = timeline.is_playing()
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Camera timeline subscription error: {e}")

    def _subscribe_app_update(self):
        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                self._on_app_update
            )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Camera app update subscription error: {e}")

    def _subscribe_mouse(self):
        try:
            input_iface = carb.input.acquire_input_interface()
            appwindow = omni.appwindow.get_default_app_window()
            if appwindow:
                mouse = appwindow.get_mouse()
                if mouse:
                    self._mouse_sub = input_iface.subscribe_to_mouse_events(
                        mouse, self._on_mouse_event
                    )
                    if carb:
                        carb.log_info("[hydragon.editor.core] Mouse subscribed for camera control.")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Camera mouse subscription error: {e}")

    # -------------------------------------------------------------------------
    # Event Handlers
    # -------------------------------------------------------------------------
    def _on_timeline_event(self, e):
        if not HAS_KIT:
            return
        try:
            event_type = int(e.type)
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                self._is_simulating = True
                self._sync_attributes_from_stage()
                self._bind_viewport_camera()
                self._set_camera_lock(True)
                self._last_mouse_pos = None
                if carb:
                    carb.log_info("[hydragon.editor.core] Simulation started. Follow Camera ACTIVE (cameraLock=True).")
            elif event_type == int(omni.timeline.TimelineEventType.PAUSE):
                self._is_simulating = False
                self._is_rmb_down = False
                self._is_mmb_down = False
                self._last_mouse_pos = None
                if carb:
                    carb.log_info("[hydragon.editor.core] Simulation paused. Follow Camera suspended (cameraLock preserved).")
            elif event_type == int(omni.timeline.TimelineEventType.STOP):
                self._is_simulating = False
                self._is_rmb_down = False
                self._is_mmb_down = False
                self._last_mouse_pos = None
                self._last_applied_pos = None
                self._last_applied_rot = None
                self._last_probe_focus = None
                self._last_probe_desired = None
                self._last_probe_result = None
                self._set_camera_lock(False)
                self._restore_viewport_camera()
                if carb:
                    carb.log_info("[hydragon.editor.core] Simulation stopped. Follow Camera INACTIVE (cameraLock=False).")
        except Exception:
            pass

    def _on_mouse_event(self, event, *args, **kwargs) -> bool:
        if not self._is_simulating or not HAS_KIT:
            return True

        try:
            event_type = int(event.type) if hasattr(event, "type") else None

            # Zoom on Scroll event
            if event_type == int(carb.input.MouseEventType.SCROLL):
                scroll_delta = getattr(event, "scrollDelta", None)
                scroll_val = None
                if scroll_delta is not None:
                    try:
                        scroll_val = float(scroll_delta[1])
                    except Exception:
                        pass
                if scroll_val is None:
                    scroll_val = getattr(event, "value", None)
                if scroll_val is None:
                    scroll_val = getattr(event, "y", None)

                if scroll_val:
                    zoom_delta = float(scroll_val) * 60.0
                    self._arm_length = max(
                        self._min_arm_length,
                        min(self._max_arm_length, self._arm_length - zoom_delta),
                    )
        except Exception:
            pass

        return True

    def _poll_mouse(self):
        """Directly polls hardware mouse position and wheel from the OS window every frame."""
        try:
            input_iface = carb.input.acquire_input_interface()
            appwindow = omni.appwindow.get_default_app_window()
            if not appwindow or not input_iface:
                return

            mouse = appwindow.get_mouse()
            if not mouse:
                return

            # A. Pixel Coordinates for Orbit (Yaw & Pitch)
            coords = input_iface.get_mouse_coords_pixel(mouse)
            if coords is not None:
                cur_x = float(coords[0])
                cur_y = float(coords[1])
                if self._last_mouse_pos is not None:
                    dx = cur_x - self._last_mouse_pos[0]
                    dy = cur_y - self._last_mouse_pos[1]
                    # Filter out huge single-frame teleport jumps (e.g. window focus/teleport)
                    if 0.0 < abs(dx) < 500.0 or 0.0 < abs(dy) < 500.0:
                        self._target_yaw = (self._target_yaw - dx * self._mouse_sensitivity) % 360.0
                        self._target_pitch = max(
                            self._min_pitch,
                            min(self._max_pitch, self._target_pitch + dy * self._mouse_sensitivity),
                        )
                self._last_mouse_pos = (cur_x, cur_y)

            # B. Scroll Delta for Spring-Arm Zoom
            scroll_up = input_iface.get_mouse_value(mouse, carb.input.MouseInput.SCROLL_UP)
            scroll_down = input_iface.get_mouse_value(mouse, carb.input.MouseInput.SCROLL_DOWN)
            if scroll_up > 0.0:
                self._arm_length = max(
                    self._min_arm_length,
                    min(self._max_arm_length, self._arm_length - scroll_up * 60.0),
                )
            elif scroll_down > 0.0:
                self._arm_length = max(
                    self._min_arm_length,
                    min(self._max_arm_length, self._arm_length + scroll_down * 60.0),
                )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Mouse polling error: {e}")

    def _on_app_update(self, e):
        if not self._is_simulating or not HAS_KIT:
            return

        dt = 1.0 / 60.0
        if hasattr(e, "payload") and isinstance(e.payload, dict):
            dt = e.payload.get("dt", 1.0 / 60.0)

        # 1. Real-time hardware mouse polling (Yaw, Pitch, Zoom)
        self._poll_mouse()

        stage = omni.usd.get_context().get_stage()
        if not stage:
            return

        # 2. Update FollowCamera
        self._update_camera(stage, dt)

    # -------------------------------------------------------------------------
    # Core Camera Update
    # -------------------------------------------------------------------------
    def _update_camera(self, stage, dt: float):
        cam_prim = self.find_camera_prim(stage)
        if not cam_prim or not cam_prim.IsValid():
            return

        target_pos = self.get_target_world_pos(stage, cam_prim)
        if target_pos is None:
            return

        # 1. Smooth Position Lag Damping (Ease-in / Ease-out)
        if self._current_focus_pos is None:
            self._current_focus_pos = target_pos
        else:
            pos_blend = (
                1.0 - math.exp(-self._position_damping * dt)
                if self._position_damping > 0.0
                else 1.0
            )
            pos_blend = max(0.0, min(1.0, pos_blend))
            fx = self._current_focus_pos[0] + (target_pos[0] - self._current_focus_pos[0]) * pos_blend
            fy = self._current_focus_pos[1] + (target_pos[1] - self._current_focus_pos[1]) * pos_blend
            fz = self._current_focus_pos[2] + (target_pos[2] - self._current_focus_pos[2]) * pos_blend
            self._current_focus_pos = (fx, fy, fz)

        # 2. Smooth Rotation Lag Damping (Ease-in / Ease-out rotational inertia)
        rot_blend = (
            1.0 - math.exp(-self._rotation_damping * dt)
            if self._rotation_damping > 0.0
            else 1.0
        )
        rot_blend = max(0.0, min(1.0, rot_blend))

        # Shortest angular distance for yaw wrap-around (0..360)
        yaw_diff = (self._target_yaw - self._yaw + 180.0) % 360.0 - 180.0
        self._yaw = (self._yaw + yaw_diff * rot_blend) % 360.0

        pitch_diff = self._target_pitch - self._pitch
        self._pitch = self._pitch + pitch_diff * rot_blend

        focus_x, focus_y, focus_z = self._current_focus_pos
        focus_center = (focus_x, focus_y + self._elevation_offset, focus_z)

        # 2. Compute Desired Camera Position via Spherical Coordinates
        # Standard Maya/USD coordinate convention:
        # Forward into arena towards yellow cylinder is +Z.
        # Camera rests BEHIND the player (-Z) at yaw=0, looking towards +Z.
        rad_pitch = math.radians(self._pitch)
        rad_yaw = math.radians(self._yaw)

        dx = -self._arm_length * math.cos(rad_pitch) * math.sin(rad_yaw)
        dy = self._arm_length * math.sin(rad_pitch)
        dz = -self._arm_length * math.cos(rad_pitch) * math.cos(rad_yaw)

        desired_cam_pos = (
            focus_center[0] + dx,
            focus_center[1] + dy,
            focus_center[2] + dz,
        )

        # 3. Anti-Clipping Obstacle Probe (Spring Arm Obstacle Avoidance)
        actual_cam_pos = self._probe_camera_collision(focus_center, desired_cam_pos)

        # 4. LookAt Rotation Math
        # In USD Camera convention, camera looks along -Z with +Y up.
        # With rotZ = -180, rotX = 180 - pitch, rotY = -yaw:
        # Camera points directly towards +Z and looks down at the ball and arena!
        rot_euler_deg = (180.0 - self._pitch, -self._yaw, -180.0)

        # 5. Apply to USD Camera Prim
        self.apply_camera_transform(cam_prim, actual_cam_pos, rot_euler_deg)

    def apply_camera_transform(self, cam_prim, pos: Tuple[float, float, float], rot_deg: Tuple[float, float, float]):
        """Sets translation and rotation on the USD camera prim if values have changed."""
        try:
            # Dirty checking: skip writing to USD if camera has not noticeably moved or rotated
            if self._last_applied_pos is not None and self._last_applied_rot is not None:
                dp = (
                    (pos[0] - self._last_applied_pos[0]) ** 2
                    + (pos[1] - self._last_applied_pos[1]) ** 2
                    + (pos[2] - self._last_applied_pos[2]) ** 2
                )
                dr = (
                    abs(rot_deg[0] - self._last_applied_rot[0])
                    + abs(rot_deg[1] - self._last_applied_rot[1])
                    + abs(rot_deg[2] - self._last_applied_rot[2])
                )
                if dp < 1e-4 and dr < 1e-4:
                    return

            self._last_applied_pos = pos
            self._last_applied_rot = rot_deg

            if not cam_prim:
                return

            if not HAS_KIT:
                # Outside Kit (e.g. pytest unit tests with mock prims)
                if hasattr(cam_prim, "GetAttribute"):
                    translate_attr = cam_prim.GetAttribute("xformOp:translate")
                    if translate_attr:
                        translate_attr.Set(pos)
                    rotate_attr = cam_prim.GetAttribute("xformOp:rotateXYZ")
                    if rotate_attr:
                        rotate_attr.Set(rot_deg)
                return

            xformable = UsdGeom.Xformable(cam_prim)

            # 1. Update xformOp:translate
            translate_attr = cam_prim.GetAttribute("xformOp:translate")
            if translate_attr and translate_attr.IsValid():
                translate_attr.Set(Gf.Vec3d(pos[0], pos[1], pos[2]))
            else:
                xformable.AddTranslateOp().Set(Gf.Vec3d(pos[0], pos[1], pos[2]))

            # 2. Update xformOp:rotateXYZ
            rotate_attr = cam_prim.GetAttribute("xformOp:rotateXYZ")
            if rotate_attr and rotate_attr.IsValid():
                rotate_attr.Set(Gf.Vec3f(rot_deg[0], rot_deg[1], rot_deg[2]))
            else:
                xformable.AddRotateXYZOp().Set(Gf.Vec3f(rot_deg[0], rot_deg[1], rot_deg[2]))

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Error applying camera transform: {e}")

    # -------------------------------------------------------------------------
    # Target Resolution & PhysX Live Transform
    # -------------------------------------------------------------------------
    def _resolve_target_world_position(self, stage, cam_prim=None) -> Optional[Tuple[float, float, float]]:
        """Resolves real-time simulated world position of the tracked target."""
        return self.get_target_world_pos(stage, cam_prim)

    def get_target_world_pos(self, stage, cam_prim=None) -> Optional[Tuple[float, float, float]]:
        """Queries real-time simulated world position of the target tracked by this camera."""
        if not stage:
            return None

        # 1. Fast path: If target is player or default, read cached player pos from player controller system
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys and p_sys.is_active_and_simulating():
                p_rb = p_sys.get_player_rb_path()
                # If target is player or not yet bound, use player controller's cached world pos
                if not self._cached_target_rb_path or self._cached_target_rb_path == p_rb:
                    pos = p_sys.get_player_world_pos()
                    if pos and (pos[0] != 0.0 or pos[1] != 0.0 or pos[2] != 0.0):
                        if not self._cached_target_rb_path and p_rb:
                            self._cached_target_rb_path = p_rb
                        return pos
        except Exception:
            pass

        if not cam_prim:
            cam_prim = self.find_camera_prim(stage)

        # 2. Resolve Target RigidBody path
        target_path_str = self._cached_target_rb_path
        if not target_path_str:
            target_prim = None
            if cam_prim:
                cam = HydragonFollowCamera(cam_prim)
                if cam.target_path:
                    target_prim = stage.GetPrimAtPath(str(cam.target_path))

            # If relationship points to an actor root (e.g. /World/Player), find its rigid body!
            if target_prim and target_prim.IsValid():
                rb_prim = _find_rigid_body_descendant(target_prim)
                if rb_prim:
                    self._cached_target_rb_path = rb_prim.GetPath().pathString
                elif _is_rigid_body(target_prim):
                    self._cached_target_rb_path = target_prim.GetPath().pathString

            # Fallback: query active player rigid body from player controller
            if not self._cached_target_rb_path:
                try:
                    from .player_controller import HydragonPlayerControllerSystem
                    p_sys = HydragonPlayerControllerSystem.get_instance()
                    if p_sys:
                        self._cached_target_rb_path = p_sys.get_player_rb_path()
                except Exception:
                    pass

            # Fallback: search stage for HydragonPlayerController and find its rigid body
            if not self._cached_target_rb_path:
                from .schemas import HydragonPlayerController
                for prim in stage.Traverse():
                    if prim.IsValid() and prim.IsActive() and HydragonPlayerController.is_applied(prim):
                        rb_p = _find_rigid_body_descendant(prim)
                        if rb_p:
                            self._cached_target_rb_path = rb_p.GetPath().pathString
                            break

            target_path_str = self._cached_target_rb_path

        if not target_path_str:
            return None

        # 3. Query PhysX C++ interface ONLY if target prim has rigid body and is active
        t_prim = stage.GetPrimAtPath(target_path_str)
        if t_prim and t_prim.IsValid() and t_prim.IsActive() and _is_rigid_body(t_prim):
            try:
                physx_iface = get_physx_interface()
                if physx_iface and hasattr(physx_iface, "get_rigidbody_transformation"):
                    rb_data = physx_iface.get_rigidbody_transformation(target_path_str)
                    if rb_data and rb_data.get("ret_val", False):
                        p = rb_data.get("position")
                        if p is not None:
                            return (float(p[0]), float(p[1]), float(p[2]))
            except Exception:
                pass

        # 4. Fallback to USD transform
        try:
            if t_prim and t_prim.IsValid():
                xformable = UsdGeom.Xformable(t_prim)
                tf = xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
                t = (
                    tf.ExtractTranslation()
                    if hasattr(tf, "ExtractTranslation")
                    else (tf.GetTranslation() if hasattr(tf, "GetTranslation") else Gf.Vec3d(0.0))
                )
                return (float(t[0]), float(t[1]), float(t[2]))
        except Exception:
            pass

        return None

    # -------------------------------------------------------------------------
    # Anti-Clipping Obstacle Raycast Probe
    # -------------------------------------------------------------------------
    def _probe_camera_collision(
        self,
        focus_pos: Tuple[float, float, float],
        desired_cam_pos: Tuple[float, float, float],
    ) -> Tuple[float, float, float]:
        """Probes from focus to desired camera position and pulls camera in front of obstacles."""
        if (
            self._last_probe_focus is not None
            and self._last_probe_desired is not None
            and self._last_probe_result is not None
        ):
            df = (
                (focus_pos[0] - self._last_probe_focus[0]) ** 2
                + (focus_pos[1] - self._last_probe_focus[1]) ** 2
                + (focus_pos[2] - self._last_probe_focus[2]) ** 2
            )
            dd = (
                (desired_cam_pos[0] - self._last_probe_desired[0]) ** 2
                + (desired_cam_pos[1] - self._last_probe_desired[1]) ** 2
                + (desired_cam_pos[2] - self._last_probe_desired[2]) ** 2
            )
            if df < 1e-4 and dd < 1e-4:
                return self._last_probe_result

        result_pos = desired_cam_pos
        try:
            sq_iface = get_physx_scene_query_interface()
            if not sq_iface:
                self._last_probe_focus = focus_pos
                self._last_probe_desired = desired_cam_pos
                self._last_probe_result = result_pos
                return result_pos

            vx = desired_cam_pos[0] - focus_pos[0]
            vy = desired_cam_pos[1] - focus_pos[1]
            vz = desired_cam_pos[2] - focus_pos[2]
            dist = math.sqrt(vx * vx + vy * vy + vz * vz)
            if dist < 1e-4:
                self._last_probe_focus = focus_pos
                self._last_probe_desired = desired_cam_pos
                self._last_probe_result = result_pos
                return result_pos

            nx, ny, nz = vx / dist, vy / dist, vz / dist
            origin = carb.Float3(focus_pos[0], focus_pos[1], focus_pos[2])
            direction = carb.Float3(nx, ny, nz)

            closest_hit_dist = dist
            target_root = self._cached_target_rb_path or ""

            def _on_probe_hit(hit):
                nonlocal closest_hit_dist
                hit_rb = str(getattr(hit, "rigid_body", "") or "")
                hit_col = str(getattr(hit, "collision", "") or "")
                # Ignore self/target
                if target_root and (hit_rb.startswith(target_root) or hit_col.startswith(target_root)):
                    return True
                # Ignore a game volume's trigger collider. It is a physics region, not geometry, so
                # treating it as an obstacle yanks the camera into the player as soon as the player
                # steps into a volume. See `volume_triggers.is_trigger_collider` for why this is a
                # callback filter rather than a PhysX collision channel.
                if is_trigger_collider(hit_col) or is_trigger_collider(hit_rb):
                    return True

                hit_pos = getattr(hit, "position", None)
                if hit_pos is not None:
                    hdist = math.sqrt(
                        (hit_pos[0] - origin[0]) ** 2
                        + (hit_pos[1] - origin[1]) ** 2
                        + (hit_pos[2] - origin[2]) ** 2
                    )
                    if hdist < closest_hit_dist:
                        closest_hit_dist = hdist
                return True

            sq_iface.raycast_all(origin, direction, dist, _on_probe_hit)

            if closest_hit_dist < dist:
                clamped_dist = max(100.0, closest_hit_dist - self._collision_offset)
                result_pos = (
                    focus_pos[0] + nx * clamped_dist,
                    focus_pos[1] + ny * clamped_dist,
                    focus_pos[2] + nz * clamped_dist,
                )

        except Exception:
            pass

        self._last_probe_focus = focus_pos
        self._last_probe_desired = desired_cam_pos
        self._last_probe_result = result_pos
        return result_pos

    # -------------------------------------------------------------------------
    # Prim Discovery & Sync (Agnostic - Zero Hardcoded Paths)
    # -------------------------------------------------------------------------
    def find_camera_prim(self, stage) -> Optional[object]:
        """Locates the active follow camera prim, prioritizing viewport binding and applied schemas."""
        if not stage:
            return None

        # 1. Check cached path
        if self._cached_cam_path:
            prim = stage.GetPrimAtPath(self._cached_cam_path)
            if prim and prim.IsValid() and prim.IsActive() and HydragonFollowCamera.is_applied(prim):
                return prim
            self._cached_cam_path = None

        # 2. Check active viewport bound camera
        try:
            from omni.kit.viewport.utility.camera_state import ViewportCameraState
            cam_state = ViewportCameraState()
            bound_path = cam_state.get_camera_path()
            if bound_path:
                prim = stage.GetPrimAtPath(bound_path)
                if prim and prim.IsValid() and prim.IsActive() and HydragonFollowCamera.is_applied(prim):
                    self._cached_cam_path = bound_path
                    return prim
        except Exception:
            pass

        # 3. Discover camera prim with HydragonFollowCameraAPI on the stage
        for prim in stage.Traverse():
            if prim.IsValid() and prim.IsActive() and HydragonFollowCamera.is_applied(prim):
                self._cached_cam_path = prim.GetPath().pathString
                return prim

        return None

    def _sync_attributes_from_stage(self):
        """Reads initial schema attributes from `/World/FollowCamera` if authored."""
        try:
            stage = omni.usd.get_context().get_stage()
            cam_prim = self.find_camera_prim(stage)
            if cam_prim and cam_prim.IsValid():
                cam = HydragonFollowCamera(cam_prim)
                self._arm_length = cam.arm_length
                self._pitch = cam.pitch
                self._yaw = cam.yaw
                self._target_yaw = cam.yaw
                self._target_pitch = cam.pitch
                self._mouse_sensitivity = cam.mouse_sensitivity
                self._min_pitch = cam.min_pitch
                self._max_pitch = cam.max_pitch
                self._collision_offset = cam.collision_offset
                self._position_damping = cam.position_damping
                self._rotation_damping = cam.rotation_damping
        except Exception:
            pass

    # -------------------------------------------------------------------------
    # Viewport Camera Binding & Locking
    # -------------------------------------------------------------------------
    def _set_camera_lock(self, lock: bool):
        """Locks camera to disable Kit's built-in ViewportCameraManipulator gestures (tumble, pan, zoom)."""
        if not HAS_KIT:
            return
        try:
            stage = omni.usd.get_context().get_stage()
            if not stage:
                return
            cam_prim = self.find_camera_prim(stage)
            if cam_prim and cam_prim.IsValid():
                lock_attr = cam_prim.GetAttribute("omni:kit:cameraLock")
                if not lock_attr or not lock_attr.IsValid():
                    lock_attr = cam_prim.CreateAttribute("omni:kit:cameraLock", Sdf.ValueTypeNames.Bool)
                lock_attr.Set(lock)
                if carb:
                    carb.log_info(f"[hydragon.editor.core] Set omni:kit:cameraLock = {lock} on FollowCamera")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to set camera lock: {e}")

    def _bind_viewport_camera(self):
        """Switches the active viewport to view through `/World/FollowCamera`."""
        try:
            import omni.kit.viewport.utility as vp_util
            active_vp = vp_util.get_active_viewport()
            if active_vp:
                cur_cam = active_vp.camera_path
                cur_cam_str = cur_cam.pathString if cur_cam else ""
                if cur_cam_str != "/World/FollowCamera":
                    self._previous_viewport_cam = cur_cam_str
                    active_vp.camera_path = Sdf.Path("/World/FollowCamera")
                    self._viewport_bound = True
                    if carb:
                        carb.log_info("[hydragon.editor.core] Active viewport bound to /World/FollowCamera")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to bind viewport camera: {e}")

    def _restore_viewport_camera(self):
        """Restores previous viewport camera on shutdown if bound by us."""
        if not self._viewport_bound or not self._previous_viewport_cam:
            return
        try:
            import omni.kit.viewport.utility as vp_util
            active_vp = vp_util.get_active_viewport()
            if active_vp and self._previous_viewport_cam:
                active_vp.camera_path = Sdf.Path(self._previous_viewport_cam)
                self._viewport_bound = False
        except Exception:
            pass
