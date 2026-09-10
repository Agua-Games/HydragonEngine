"""
Hydragon Engine - Force Volume Physics Simulation System

Implements component-driven physics volumes (HydragonForceVolumeAPI) supporting:
- Linear Force / Directional Impulse (conveyors, jump pads, wind tunnels)
- Radial Force / Attractor / Repulsor with Linear/Squared falloff
- 3D Turbulence perturbations
- Linear and Angular Dampening (viscous drag, water, mud)
- Vortex / Whirlpool (tangential acceleration + inward centripetal pull)
"""

import math
import time
from typing import Any, Dict, List, Optional, Set, Tuple

try:
    import carb
    import omni.kit.app
    import omni.timeline
    import omni.usd
    import omni.physx
    from omni.physx import (
        get_physx_interface,
        get_physx_simulation_interface,
    )
    from pxr import Usd, UsdGeom, Sdf, Gf, UsdPhysics, UsdUtils, PhysicsSchemaTools
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    PhysicsSchemaTools = None
    UsdUtils = None

from .schemas import HydragonForceVolume, HydragonActor


def update_wireframe_guide(
    bounds_prim,
    shape: str,
    half_extents: Tuple[float, float, float] = (100.0, 100.0, 100.0),
    radius: float = 100.0,
    half_height: float = 100.0,
    color: Tuple[float, float, float] = (0.2, 0.7, 1.0),
):
    """
    Updates or converts bounds_prim to a hollow wireframe BasisCurves cage.
    Supports 'Box', 'Sphere', 'Cylinder', and 'Plane'. Zero solid faces/fill.
    """
    if not HAS_KIT or not bounds_prim or not hasattr(bounds_prim, "IsValid") or not bounds_prim.IsValid():
        return

    try:
        if bounds_prim.GetTypeName() != "BasisCurves":
            bounds_prim.SetTypeName("BasisCurves")

        curves = UsdGeom.BasisCurves(bounds_prim)
        curves.GetTypeAttr().Set("linear")
        curves.GetWrapAttr().Set("nonperiodic")

        counts = []
        points = []

        if shape == "Sphere":
            segs = 32
            r = radius
            for plane in ("XY", "XZ", "YZ"):
                counts.append(segs + 1)
                for i in range(segs + 1):
                    theta = 2.0 * math.pi * (i / segs)
                    cos_t = math.cos(theta) * r
                    sin_t = math.sin(theta) * r
                    if plane == "XY":
                        points.append(Gf.Vec3f(cos_t, sin_t, 0.0))
                    elif plane == "XZ":
                        points.append(Gf.Vec3f(cos_t, 0.0, sin_t))
                    else:
                        points.append(Gf.Vec3f(0.0, cos_t, sin_t))

            extent = [Gf.Vec3f(-r, -r, -r), Gf.Vec3f(r, r, r)]

        elif shape == "Cylinder":
            segs = 32
            r = radius
            h = half_height
            counts.append(segs + 1)
            for i in range(segs + 1):
                theta = 2.0 * math.pi * (i / segs)
                points.append(Gf.Vec3f(math.cos(theta) * r, h, math.sin(theta) * r))
            counts.append(segs + 1)
            for i in range(segs + 1):
                theta = 2.0 * math.pi * (i / segs)
                points.append(Gf.Vec3f(math.cos(theta) * r, -h, math.sin(theta) * r))
            for angle in (0.0, math.pi * 0.5, math.pi, math.pi * 1.5):
                counts.append(2)
                cx = math.cos(angle) * r
                cz = math.sin(angle) * r
                points.append(Gf.Vec3f(cx, -h, cz))
                points.append(Gf.Vec3f(cx, h, cz))

            extent = [Gf.Vec3f(-r, -h, -r), Gf.Vec3f(r, h, r)]

        elif shape == "Plane":
            hx, hz = half_extents[0], half_extents[2]
            counts.append(5)
            points.extend([
                Gf.Vec3f(-hx, 0.0, -hz),
                Gf.Vec3f(hx, 0.0, -hz),
                Gf.Vec3f(hx, 0.0, hz),
                Gf.Vec3f(-hx, 0.0, hz),
                Gf.Vec3f(-hx, 0.0, -hz),
            ])
            counts.append(2)
            points.extend([Gf.Vec3f(-hx, 0.0, -hz), Gf.Vec3f(hx, 0.0, hz)])
            counts.append(2)
            points.extend([Gf.Vec3f(hx, 0.0, -hz), Gf.Vec3f(-hx, 0.0, hz)])
            extent = [Gf.Vec3f(-hx, 0.0, -hz), Gf.Vec3f(hx, 0.0, hz)]

        else:
            hx, hy, hz = half_extents
            counts = [2] * 12
            points = [
                Gf.Vec3f(-hx, -hy, -hz), Gf.Vec3f(hx, -hy, -hz),
                Gf.Vec3f(hx, -hy, -hz), Gf.Vec3f(hx, -hy, hz),
                Gf.Vec3f(hx, -hy, hz), Gf.Vec3f(-hx, -hy, hz),
                Gf.Vec3f(-hx, -hy, hz), Gf.Vec3f(-hx, -hy, -hz),
                Gf.Vec3f(-hx, hy, -hz), Gf.Vec3f(hx, hy, -hz),
                Gf.Vec3f(hx, hy, -hz), Gf.Vec3f(hx, hy, hz),
                Gf.Vec3f(hx, hy, hz), Gf.Vec3f(-hx, hy, hz),
                Gf.Vec3f(-hx, hy, hz), Gf.Vec3f(-hx, hy, -hz),
                Gf.Vec3f(-hx, -hy, -hz), Gf.Vec3f(-hx, hy, -hz),
                Gf.Vec3f(hx, -hy, -hz), Gf.Vec3f(hx, hy, -hz),
                Gf.Vec3f(hx, -hy, hz), Gf.Vec3f(hx, hy, hz),
                Gf.Vec3f(-hx, -hy, hz), Gf.Vec3f(-hx, hy, hz),
            ]
            extent = [Gf.Vec3f(-hx, -hy, -hz), Gf.Vec3f(hx, hy, hz)]

        curves.GetCurveVertexCountsAttr().Set(counts)
        curves.GetPointsAttr().Set(points)
        curves.GetExtentAttr().Set(extent)
        curves.GetWidthsAttr().Set([2.5] * len(points))
        if hasattr(curves.GetWidthsAttr(), "SetMetadata"):
            try:
                curves.GetWidthsAttr().SetMetadata("interpolation", "constant")
            except Exception:
                pass
        curves.GetDisplayColorAttr().Set([Gf.Vec3f(*color)])
        if hasattr(curves.GetDisplayColorAttr(), "SetMetadata"):
            try:
                curves.GetDisplayColorAttr().SetMetadata("interpolation", "constant")
            except Exception:
                pass
        if bounds_prim.HasAttribute("purpose"):
            bounds_prim.GetAttribute("purpose").Set("default")
        else:
            bounds_prim.CreateAttribute("purpose", Sdf.ValueTypeNames.Token).Set("default")

    except Exception as e:
        if carb:
            carb.log_warn(f"[hydragon.editor.core] Failed to update wireframe guide: {e}")


class HydragonForceVolumeZone:
    """
    Component class representing an individual physical force volume entity.
    Encapsulates oriented geometry bounds, physics behaviors, and impulse timing.
    """

    def __init__(self, prim, world_pos: Tuple[float, float, float] = (0.0, 0.0, 0.0)):
        self._prim = prim
        self._prim_path: str = prim.GetPath().pathString if prim and hasattr(prim, "GetPath") else ""
        self._schema: HydragonForceVolume = HydragonForceVolume(prim)

        self._world_pos: Tuple[float, float, float] = world_pos
        self._world_transform: Optional[Any] = None
        self._inv_world_matrix: Optional[Any] = None
        self._half_extents: Tuple[float, float, float] = (100.0, 100.0, 100.0)
        self._radius: float = 100.0
        self._half_height: float = 100.0

        self._impulse_timers: Dict[str, float] = {}

        self._cache_bounds_and_transforms()

    @property
    def prim_path(self) -> str:
        return self._prim_path

    @property
    def world_pos(self) -> Tuple[float, float, float]:
        return self._world_pos

    @property
    def is_enabled(self) -> bool:
        if self._schema:
            return self._schema.is_enabled
        return True

    @property
    def mode(self) -> str:
        if self._schema:
            return self._schema.mode
        return "Continuous"

    @property
    def volume_shape(self) -> str:
        if self._schema:
            return self._schema.volume_shape
        return "Box"

    @property
    def filter_faction(self) -> str:
        if self._schema:
            return self._schema.filter_faction
        return "All"

    @property
    def impulse_cooldown(self) -> float:
        if self._schema:
            return self._schema.impulse_cooldown
        return 1.0

    @property
    def half_extents(self) -> Tuple[float, float, float]:
        return self._half_extents

    @half_extents.setter
    def half_extents(self, ext: Tuple[float, float, float]):
        self._half_extents = ext

    @property
    def radius(self) -> float:
        return self._radius

    @radius.setter
    def radius(self, val: float):
        self._radius = val

    @property
    def half_height(self) -> float:
        return self._half_height

    @half_height.setter
    def half_height(self, val: float):
        self._half_height = val

    def _cache_bounds_and_transforms(self):
        """Extracts world transform matrix, extents, and shape dimensions from USD."""
        if not HAS_KIT or not self._prim or not hasattr(self._prim, "IsValid") or not self._prim.IsValid():
            return

        try:
            bounds_prim = self._prim.GetPrimAtPath("volumes/force_bounds")
            if bounds_prim and bounds_prim.IsValid():
                bxform = UsdGeom.Xformable(bounds_prim)
                b_xf = bxform.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
                self._world_transform = b_xf
                try:
                    self._inv_world_matrix = b_xf.GetInverse()
                except Exception:
                    self._inv_world_matrix = None

                t = b_xf.ExtractTranslation()
                self._world_pos = (float(t[0]), float(t[1]), float(t[2]))

                b_scale = (
                    float(b_xf.GetRow(0).GetLength()),
                    float(b_xf.GetRow(1).GetLength()),
                    float(b_xf.GetRow(2).GetLength()),
                )

                base_hx, base_hy, base_hz = 100.0, 100.0, 100.0
                extent_attr = bounds_prim.GetAttribute("extent")
                if extent_attr and extent_attr.IsValid():
                    ext_val = extent_attr.Get()
                    if ext_val and len(ext_val) >= 2:
                        base_hx = max(abs(float(ext_val[0][0])), abs(float(ext_val[1][0])))
                        base_hy = max(abs(float(ext_val[0][1])), abs(float(ext_val[1][1])))
                        base_hz = max(abs(float(ext_val[0][2])), abs(float(ext_val[1][2])))
                elif bounds_prim.GetTypeName() == "Cube":
                    cube_geom = UsdGeom.Cube(bounds_prim)
                    size = float(cube_geom.GetSizeAttr().Get() or 200.0) if cube_geom.GetSizeAttr() else 200.0
                    base_hx = base_hy = base_hz = size * 0.5

                hx = base_hx * b_scale[0]
                hy = base_hy * b_scale[1]
                hz = base_hz * b_scale[2]
                self._half_extents = (hx, hy, hz)
                self._radius = max(hx, hz)
                self._half_height = hy

                # Synchronize guide wireframe shape dynamically
                shape = self.volume_shape
                update_wireframe_guide(
                    bounds_prim,
                    shape=shape,
                    half_extents=(base_hx, base_hy, base_hz),
                    radius=max(base_hx, base_hz),
                    half_height=base_hy,
                    color=(0.2, 0.7, 1.0),
                )
            else:
                xformable = UsdGeom.Xformable(self._prim)
                world_xf = xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
                self._world_transform = world_xf
                try:
                    self._inv_world_matrix = world_xf.GetInverse()
                except Exception:
                    self._inv_world_matrix = None
                t = world_xf.ExtractTranslation()
                self._world_pos = (float(t[0]), float(t[1]), float(t[2]))
                scale_vec = (
                    float(world_xf.GetRow(0).GetLength()),
                    float(world_xf.GetRow(1).GetLength()),
                    float(world_xf.GetRow(2).GetLength()),
                )
                self._half_extents = (100.0 * scale_vec[0], 100.0 * scale_vec[1], 100.0 * scale_vec[2])
                self._radius = max(self._half_extents[0], self._half_extents[2])
                self._half_height = self._half_extents[1]

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to cache bounds for force volume {self._prim_path}: {e}")

    def check_shape_overlap(self, point: Tuple[float, float, float]) -> bool:
        """
        Evaluates whether point (world coordinates) is inside this volume's oriented geometric bounds.
        Supports Box (OBB), Sphere, and Cylinder.
        """
        shape = self.volume_shape

        # Sphere check (in world space directly)
        if shape == "Sphere":
            dx = point[0] - self._world_pos[0]
            dy = point[1] - self._world_pos[1]
            dz = point[2] - self._world_pos[2]
            dist_sq = dx * dx + dy * dy + dz * dz
            return dist_sq <= (self._radius * self._radius)

        # Local space transformation for Box and Cylinder
        local_x, local_y, local_z = 0.0, 0.0, 0.0
        if self._inv_world_matrix is not None and HAS_KIT:
            try:
                pt_world = Gf.Vec3d(point[0], point[1], point[2])
                pt_local = self._inv_world_matrix.Transform(pt_world)
                local_x = float(pt_local[0])
                local_y = float(pt_local[1])
                local_z = float(pt_local[2])
            except Exception:
                local_x = point[0] - self._world_pos[0]
                local_y = point[1] - self._world_pos[1]
                local_z = point[2] - self._world_pos[2]
        else:
            local_x = point[0] - self._world_pos[0]
            local_y = point[1] - self._world_pos[1]
            local_z = point[2] - self._world_pos[2]

        if shape == "Cylinder":
            r_sq = local_x * local_x + local_z * local_z
            return (r_sq <= self._radius * self._radius) and (abs(local_y) <= self._half_height)

        # Default: Box (Oriented Bounding Box)
        hx, hy, hz = self._half_extents
        return (abs(local_x) <= hx) and (abs(local_y) <= hy) and (abs(local_z) <= hz)

    def check_overlap(self, point: Tuple[float, float, float]) -> bool:
        """
        Evaluates whether point (world coordinates) is affected by this volume.
        Encompasses primary shape bounds, and uncoupled radial force radius.
        """
        if not self.is_enabled:
            return False

        if self.check_shape_overlap(point):
            return True

        # Uncoupled radial force influence zone
        if self._schema and self._schema.radial_enabled:
            dx = point[0] - self._world_pos[0]
            dy = point[1] - self._world_pos[1]
            dz = point[2] - self._world_pos[2]
            dist_sq = dx * dx + dy * dy + dz * dz
            rad = max(0.0, self._schema.radial_radius)
            if dist_sq <= rad * rad:
                return True

        return False

    def can_apply_impulse(self, rb_path: str, current_time: float) -> bool:
        """Evaluates impulse cooldown for a specific rigid body."""
        if rb_path not in self._impulse_timers:
            self._impulse_timers[rb_path] = current_time
            return True

        last_t = self._impulse_timers[rb_path]
        cooldown = self.impulse_cooldown
        if current_time - last_t >= cooldown:
            self._impulse_timers[rb_path] = current_time
            return True
        return False

    def compute_forces(
        self,
        body_pos: Tuple[float, float, float],
        body_vel: Tuple[float, float, float],
        body_ang_vel: Tuple[float, float, float],
        sim_time: float,
        dt: float,
    ) -> Tuple[Tuple[float, float, float], Tuple[float, float, float], Tuple[float, float, float]]:
        """
        Calculates total physical force, and dampened linear/angular velocities.
        Returns: (net_force_vector, damped_linear_velocity, damped_angular_velocity)
        """
        if not self._schema:
            return (0.0, 0.0, 0.0), body_vel, body_ang_vel

        schema = self._schema
        fx, fy, fz = 0.0, 0.0, 0.0
        in_shape = self.check_shape_overlap(body_pos)

        # 1. Linear Force
        if schema.linear_enabled and in_shape:
            ldir = schema.linear_direction
            mag = schema.linear_magnitude
            # Normalize direction
            d_len = math.sqrt(ldir[0] * ldir[0] + ldir[1] * ldir[1] + ldir[2] * ldir[2])
            if d_len > 1e-4:
                nx = ldir[0] / d_len
                ny = ldir[1] / d_len
                nz = ldir[2] / d_len

                coord_space = getattr(schema, "linear_coord_space", "Volume")
                if coord_space == "Volume" and self._world_transform is not None and HAS_KIT:
                    try:
                        rot_dir = self._world_transform.TransformDir(Gf.Vec3d(nx, ny, nz)).GetNormalized()
                        fx += float(rot_dir[0]) * mag
                        fy += float(rot_dir[1]) * mag
                        fz += float(rot_dir[2]) * mag
                    except Exception:
                        fx += nx * mag
                        fy += ny * mag
                        fz += nz * mag
                else:
                    # World space direction
                    fx += nx * mag
                    fy += ny * mag
                    fz += nz * mag

        # 2. Radial Force (Attractor or Repulsor) - Uncoupled from box extents
        if schema.radial_enabled:
            dx = body_pos[0] - self._world_pos[0]
            dy = body_pos[1] - self._world_pos[1]
            dz = body_pos[2] - self._world_pos[2]
            dist = math.sqrt(dx * dx + dy * dy + dz * dz)
            max_r = max(1.0, schema.radial_radius)

            if dist <= max_r and dist > 1e-3:
                falloff_mode = schema.radial_falloff
                if falloff_mode == "Squared":
                    ratio = max(0.0, 1.0 - (dist / max_r) ** 2)
                elif falloff_mode == "Linear":
                    ratio = max(0.0, 1.0 - (dist / max_r))
                else:  # None
                    ratio = 1.0

                rad_mag = schema.radial_magnitude * ratio
                sign = -1.0 if rad_mag >= 0.0 else 1.0
                abs_mag = abs(rad_mag)

                fx += (dx / dist) * abs_mag * sign
                fy += (dy / dist) * abs_mag * sign
                fz += (dz / dist) * abs_mag * sign

        # 3. Turbulence (Chaotic 3D perturbation)
        if schema.turbulence_enabled and in_shape:
            freq = schema.turbulence_frequency
            t_mag = schema.turbulence_magnitude
            t = sim_time * freq
            tx = math.sin(t + body_pos[0] * 0.02) + math.cos(t * 1.3 + body_pos[2] * 0.015)
            ty = math.cos(t + body_pos[1] * 0.02) + math.sin(t * 0.7 + body_pos[0] * 0.015)
            tz = math.sin(t * 1.1 + body_pos[2] * 0.02) + math.cos(t * 0.9 + body_pos[1] * 0.015)
            t_len = math.sqrt(tx * tx + ty * ty + tz * tz)
            if t_len > 1e-3:
                fx += (tx / t_len) * t_mag
                fy += (ty / t_len) * t_mag
                fz += (tz / t_len) * t_mag

        # 4. Vortex (Tangential swirl + centripetal inward pull)
        if schema.vortex_enabled and in_shape:
            v_axis = schema.vortex_axis
            a_len = math.sqrt(v_axis[0] * v_axis[0] + v_axis[1] * v_axis[1] + v_axis[2] * v_axis[2])
            ax = v_axis[0] / a_len if a_len > 1e-4 else 0.0
            ay = v_axis[1] / a_len if a_len > 1e-4 else 1.0
            az = v_axis[2] / a_len if a_len > 1e-4 else 0.0

            rx = body_pos[0] - self._world_pos[0]
            ry = body_pos[1] - self._world_pos[1]
            rz = body_pos[2] - self._world_pos[2]

            # Project out axis component: r_planar = r - (r . axis) * axis
            dot = rx * ax + ry * ay + rz * az
            px = rx - dot * ax
            py = ry - dot * ay
            pz = rz - dot * az
            p_dist = math.sqrt(px * px + py * py + pz * pz)

            if p_dist > 1e-2:
                in_x = -px / p_dist
                in_y = -py / p_dist
                in_z = -pz / p_dist

                tx = ay * (-in_z) - az * (-in_y)
                ty = az * (-in_x) - ax * (-in_z)
                tz = ax * (-in_y) - ay * (-in_x)

                v_mag = schema.vortex_magnitude
                in_pull = schema.vortex_inward_pull

                # Attenuate inward pull smoothly towards the vortex eye
                # to prevent radial sign-flip oscillations across the axis and allow stable orbits
                pull_ratio = min(1.0, p_dist / 50.0)
                effective_pull = in_pull * pull_ratio

                fx += tx * v_mag + in_x * effective_pull
                fy += ty * v_mag + in_y * effective_pull
                fz += tz * v_mag + in_z * effective_pull

        # 5. Dampening (Linear & Angular Drag)
        damped_v = body_vel
        damped_w = body_ang_vel
        if schema.dampening_enabled and in_shape:
            lin_damp = max(0.0, schema.linear_damping)
            ang_damp = max(0.0, schema.angular_damping)
            lin_factor = max(0.0, 1.0 - lin_damp * dt)
            ang_factor = max(0.0, 1.0 - ang_damp * dt)

            damped_v = (body_vel[0] * lin_factor, body_vel[1] * lin_factor, body_vel[2] * lin_factor)
            damped_w = (body_ang_vel[0] * ang_factor, body_ang_vel[1] * ang_factor, body_ang_vel[2] * ang_factor)

        return (fx, fy, fz), damped_v, damped_w


class HydragonForceVolumeSystem:
    """
    Simulation system managing all active HydragonForceVolumeAPI entities.
    Strictly follows ECS guidelines: discovers volumes once on PLAY and indexes in memory.
    """

    _instance: Optional["HydragonForceVolumeSystem"] = None

    def __init__(self):
        HydragonForceVolumeSystem._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False
        self._physics_step_sub = None
        self._timeline_sub = None

        self._active_volumes: Dict[str, HydragonForceVolumeZone] = {}
        self._sim_time: float = 0.0

    @classmethod
    def get_instance(cls) -> Optional["HydragonForceVolumeSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

    @property
    def active_volume_count(self) -> int:
        return len(self._active_volumes)

    def startup(self):
        """Initializes subscriptions to timeline and physics simulation."""
        self._is_active = True
        if not HAS_KIT:
            return

        try:
            timeline = omni.timeline.get_timeline_interface()
            self._timeline_sub = timeline.get_timeline_event_stream().create_subscription_to_pop(
                self._on_timeline_event
            )
            self._subscribe_physics()
            if carb:
                carb.log_info("[hydragon.editor.core] HydragonForceVolumeSystem started.")
        except Exception as e:
            if carb:
                carb.log_error(f"[hydragon.editor.core] HydragonForceVolumeSystem startup failed: {e}")

    def shutdown(self):
        """Cleans up subscriptions and cached volume registry."""
        self._is_active = False
        self._is_simulating = False
        self._active_volumes.clear()
        self._timeline_sub = None
        self._physics_step_sub = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonForceVolumeSystem shut down.")

    def _subscribe_physics(self) -> bool:
        if not HAS_KIT:
            return False
        try:
            physx_iface = get_physx_interface()
            if physx_iface and hasattr(physx_iface, "subscribe_physics_step_events"):
                self._physics_step_sub = physx_iface.subscribe_physics_step_events(self._on_physics_step)
                return True
        except Exception:
            pass
        return False

    def _on_timeline_event(self, event):
        if not HAS_KIT:
            return
        event_type = event.type
        if event_type == int(omni.timeline.TimelineEventType.PLAY):
            self._is_simulating = True
            self._sim_time = 0.0
            self._discover_volumes_on_play()
        elif event_type in (
            int(omni.timeline.TimelineEventType.STOP),
            int(omni.timeline.TimelineEventType.PAUSE),
        ):
            self._is_simulating = False
            self._active_volumes.clear()

    def _discover_volumes_on_play(self):
        """Discovers and caches all prims with HydragonForceVolumeAPI applied."""
        self._active_volumes.clear()
        if not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
        if not stage:
            return

        for prim in stage.Traverse():
            if not prim or not prim.IsValid():
                continue
            if HydragonForceVolume.is_applied(prim):
                zone = HydragonForceVolumeZone(prim)
                self._active_volumes[zone.prim_path] = zone
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Registered Force Volume: {zone.prim_path} (mode={zone.mode}, shape={zone.volume_shape})"
                    )

    def _on_physics_step(self, dt: float):
        if not self._is_simulating or not self._active_volumes:
            return

        self._sim_time += dt
        now = time.time()

        stage = omni.usd.get_context().get_stage() if HAS_KIT and omni.usd.get_context() else None
        if not stage:
            return

        stage_id = UsdUtils.StageCache.Get().GetId(stage).ToLongInt() if HAS_KIT and UsdUtils else 0
        physx_iface = get_physx_interface() if HAS_KIT else None
        sim_iface = get_physx_simulation_interface() if HAS_KIT else None

        # 1. Collect tracked dynamic bodies
        tracked_bodies = self._gather_active_rigid_bodies(stage, physx_iface)

        # 2. Iterate each volume zone
        for zone in self._active_volumes.values():
            if not zone.is_enabled:
                continue

            filter_fac = zone.filter_faction
            mode = zone.mode

            for rb_info in tracked_bodies:
                faction = rb_info.get("faction", "Unknown")
                if filter_fac != "All" and filter_fac != "RigidBodies" and filter_fac != faction:
                    continue

                pos = rb_info["world_pos"]
                rb_path = rb_info["rb_path"]
                prim_id = rb_info["prim_id"]
                vel = rb_info["linear_vel"]
                ang_vel = rb_info["angular_vel"]

                # Overlap check
                if not zone.check_overlap(pos):
                    continue

                # Impulse vs Continuous logic
                if mode == "Impulse":
                    if not zone.can_apply_impulse(rb_path, now):
                        continue
                    force_vec, _, _ = zone.compute_forces(pos, vel, ang_vel, self._sim_time, dt)
                    if sim_iface and any(abs(c) > 1e-3 for c in force_vec):
                        sim_iface.apply_force_at_pos(
                            stage_id, prim_id, carb.Float3(force_vec[0], force_vec[1], force_vec[2]), carb.Float3(pos[0], pos[1], pos[2]), "Impulse"
                        )
                else:  # Continuous
                    force_vec, damped_v, damped_w = zone.compute_forces(pos, vel, ang_vel, self._sim_time, dt)
                    # Apply continuous force
                    if sim_iface and any(abs(c) > 1e-3 for c in force_vec):
                        sim_iface.apply_force_at_pos(
                            stage_id, prim_id, carb.Float3(force_vec[0], force_vec[1], force_vec[2]), carb.Float3(pos[0], pos[1], pos[2]), "Force"
                        )
                    # Apply dampening directly if changed
                    if physx_iface and (damped_v != vel or damped_w != ang_vel):
                        try:
                            for set_m in ("set_rigidbody_linear_velocity", "set_linear_velocity"):
                                if hasattr(physx_iface, set_m):
                                    getattr(physx_iface, set_m)(rb_path, carb.Float3(damped_v[0], damped_v[1], damped_v[2]))
                                    break
                            for set_wm in ("set_rigidbody_angular_velocity", "set_angular_velocity"):
                                if hasattr(physx_iface, set_wm):
                                    getattr(physx_iface, set_wm)(rb_path, carb.Float3(damped_w[0], damped_w[1], damped_w[2]))
                                    break
                        except Exception:
                            pass

    def _gather_active_rigid_bodies(self, stage, physx_iface) -> List[Dict[str, Any]]:
        """Collects Player and Foes active rigid bodies from in-memory controllers."""
        bodies = []

        # A. Player Body
        try:
            from .player_controller import HydragonPlayerControllerSystem
            player_sys = HydragonPlayerControllerSystem.get_instance()
            if player_sys and player_sys.is_active_and_simulating():
                rb_path = player_sys.get_player_rb_path()
                pos = player_sys.get_player_world_pos()
                if rb_path and pos:
                    prim_id = PhysicsSchemaTools.sdfPathToInt(rb_path) if PhysicsSchemaTools else 0
                    vel, w_vel = self._get_body_velocities(physx_iface, rb_path)
                    bodies.append({
                        "rb_path": rb_path,
                        "prim_id": prim_id,
                        "world_pos": pos,
                        "linear_vel": vel,
                        "angular_vel": w_vel,
                        "faction": "Player",
                    })
        except Exception:
            pass

        # B. Foes Bodies
        try:
            from .foes_controller import HydragonFoesControllerSystem
            foes_sys = HydragonFoesControllerSystem.get_instance()
            if foes_sys and foes_sys.is_active_and_simulating():
                for brain in getattr(foes_sys, "_active_brains", {}).values():
                    if brain and brain.is_alive and getattr(brain, "_rb_path", None):
                        rb_path = brain._rb_path
                        pos = brain._current_pos
                        prim_id = PhysicsSchemaTools.sdfPathToInt(rb_path) if PhysicsSchemaTools else 0
                        vel, w_vel = self._get_body_velocities(physx_iface, rb_path)
                        bodies.append({
                            "rb_path": rb_path,
                            "prim_id": prim_id,
                            "world_pos": pos,
                            "linear_vel": vel,
                            "angular_vel": w_vel,
                            "faction": "Enemy",
                        })
        except Exception:
            pass

        return bodies

    def _get_body_velocities(self, physx_iface, rb_path: str) -> Tuple[Tuple[float, float, float], Tuple[float, float, float]]:
        """Extracts current linear and angular velocities from PhysX interface."""
        vel = (0.0, 0.0, 0.0)
        w_vel = (0.0, 0.0, 0.0)
        if not physx_iface or not rb_path:
            return vel, w_vel

        try:
            for get_m in ("get_rigidbody_linear_velocity", "get_linear_velocity"):
                if hasattr(physx_iface, get_m):
                    v = getattr(physx_iface, get_m)(rb_path)
                    if v is not None:
                        vel = (float(v[0]), float(v[1]), float(v[2]))
                        break
            for get_wm in ("get_rigidbody_angular_velocity", "get_angular_velocity"):
                if hasattr(physx_iface, get_wm):
                    w = getattr(physx_iface, get_wm)(rb_path)
                    if w is not None:
                        w_vel = (float(w[0]), float(w[1]), float(w[2]))
                        break
        except Exception:
            pass

        return vel, w_vel
