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

        # Lag Damping state
        self._current_focus_pos: Optional[Tuple[float, float, float]] = None
        self._lag_speed: float = 12.0  # Smooth follow speed

        # Mouse input tracking
        self._mouse_sub = None
        self._is_rmb_down: bool = False
        self._is_mmb_down: bool = False
        self._last_mouse_pos: Optional[Tuple[float, float]] = None

        # Event subscriptions
        self._app_update_sub = None
        self._timeline_sub = None

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
            elif event_type in (int(omni.timeline.TimelineEventType.STOP), int(omni.timeline.TimelineEventType.PAUSE)):
                self._is_simulating = False
                self._is_rmb_down = False
                self._is_mmb_down = False
                self._last_mouse_pos = None
                self._set_camera_lock(False)
                if carb:
                    carb.log_info("[hydragon.editor.core] Simulation stopped/paused. Follow Camera INACTIVE (cameraLock=False).")
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
                        self._yaw = (self._yaw - dx * self._mouse_sensitivity) % 360.0
                        self._pitch = max(
                            self._min_pitch,
                            min(self._max_pitch, self._pitch + dy * self._mouse_sensitivity),
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

        target_pos = self._resolve_target_world_position(stage)
        if target_pos is None:
            return

        # 1. Smooth Position Lag Damping
        if self._current_focus_pos is None:
            self._current_focus_pos = target_pos
        else:
            blend_factor = min(1.0, self._lag_speed * dt)
            fx = self._current_focus_pos[0] + (target_pos[0] - self._current_focus_pos[0]) * blend_factor
            fy = self._current_focus_pos[1] + (target_pos[1] - self._current_focus_pos[1]) * blend_factor
            fz = self._current_focus_pos[2] + (target_pos[2] - self._current_focus_pos[2]) * blend_factor
            self._current_focus_pos = (fx, fy, fz)

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
        """Sets translation and rotation on the USD camera prim."""
        try:
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
    def _resolve_target_world_position(self, stage) -> Optional[Tuple[float, float, float]]:
        """Queries the true real-time simulated position of the player target."""
        # 1. Resolve RigidBody prim path for PhysX query
        if not self._cached_target_rb_path:
            player_root = stage.GetPrimAtPath("/World/Player")
            if player_root and player_root.IsValid():
                ball_mesh = stage.GetPrimAtPath("/World/Player/geometry/ball_mesh")
                if ball_mesh and ball_mesh.IsValid():
                    self._cached_target_rb_path = ball_mesh.GetPath().pathString
                else:
                    self._cached_target_rb_path = player_root.GetPath().pathString
            else:
                return None

        # 2. Query PhysX C++ interface for real-time simulated rigid body transform
        try:
            physx_iface = get_physx_interface()
            if physx_iface and hasattr(physx_iface, "get_rigidbody_transformation"):
                rb_data = physx_iface.get_rigidbody_transformation(self._cached_target_rb_path)
                if rb_data and rb_data.get("ret_val", False):
                    p = rb_data.get("position")
                    if p is not None:
                        return (float(p[0]), float(p[1]), float(p[2]))
        except Exception:
            pass

        # 3. Fallback to USD transform
        try:
            target_prim = stage.GetPrimAtPath(self._cached_target_rb_path or "/World/Player")
            if target_prim and target_prim.IsValid():
                xformable = UsdGeom.Xformable(target_prim)
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
        try:
            sq_iface = get_physx_scene_query_interface()
            if not sq_iface:
                return desired_cam_pos

            vx = desired_cam_pos[0] - focus_pos[0]
            vy = desired_cam_pos[1] - focus_pos[1]
            vz = desired_cam_pos[2] - focus_pos[2]
            dist = math.sqrt(vx * vx + vy * vy + vz * vz)
            if dist < 1e-4:
                return desired_cam_pos

            nx, ny, nz = vx / dist, vy / dist, vz / dist
            origin = carb.Float3(focus_pos[0], focus_pos[1], focus_pos[2])
            direction = carb.Float3(nx, ny, nz)

            closest_hit_dist = dist
            player_root = "/World/Player"

            def _on_probe_hit(hit):
                nonlocal closest_hit_dist
                hit_rb = str(getattr(hit, "rigid_body", "") or "")
                hit_col = str(getattr(hit, "collision", "") or "")
                # Ignore self
                if hit_rb.startswith(player_root) or hit_col.startswith(player_root):
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
                return (
                    focus_pos[0] + nx * clamped_dist,
                    focus_pos[1] + ny * clamped_dist,
                    focus_pos[2] + nz * clamped_dist,
                )

        except Exception:
            pass

        return desired_cam_pos

    # -------------------------------------------------------------------------
    # Prim Discovery & Sync
    # -------------------------------------------------------------------------
    def find_camera_prim(self, stage) -> Optional[object]:
        if not stage:
            return None

        cam_prim = stage.GetPrimAtPath(self._cached_cam_path or "/World/FollowCamera")
        if cam_prim and cam_prim.IsValid():
            return cam_prim

        for prim in stage.Traverse():
            if prim.IsA(UsdGeom.Camera) and HydragonFollowCamera.is_applied(prim):
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
                self._mouse_sensitivity = cam.mouse_sensitivity
                self._min_pitch = cam.min_pitch
                self._max_pitch = cam.max_pitch
                self._collision_offset = cam.collision_offset
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
