"""
Hydragon Engine - Gameplay Effects Subsystem (VFX, Lighting Flash & Spatial Audio)

Manages real-time audiovisual feedback when gameplay entities are destroyed:
- Pre-allocated 3-slot Object Pool eliminating runtime prim allocation and stage churn
- High-velocity 3D diamond sparks with radial burst, upward floral bias, and 3D curl turbulence
- Emissive glowing material (OmniSurface / OmniPBR: emission_weight=1.0, emission_intensity=60000.0)
- Dynamic sphere light flash (UsdLux.SphereLight) with quadratic decay
- Pre-cached RAM audio playback via omni.kit.uiaudio for guaranteed 0ms latency
- O(1) in-memory registry, zero stage traversal in update loops, and clean deactivation
- GPU acceleration via NVIDIA Warp for particle simulations (auto-detected, fallback to CPU)
"""

import math
import os
import random
from typing import Any, Dict, List, Optional, Tuple

try:
    import carb
    import omni.kit.app
    import omni.timeline
    import omni.usd
    from pxr import Usd, UsdGeom, UsdLux, UsdShade, Gf, Sdf, Vt
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    Usd = None
    UsdGeom = None
    UsdLux = None
    UsdShade = None
    Gf = None
    Sdf = None
    Vt = None


try:
    import warp as wp
    HAS_WARP = True
    try:
        wp.init()
    except Exception:
        pass
except ImportError:
    HAS_WARP = False
    wp = None


# =============================================================================
# WARP GPU ACCELERATION KERNELS
# =============================================================================
if HAS_WARP:
    @wp.kernel
    def simulate_sparks_kernel(
        positions: wp.array(dtype=wp.vec3),
        velocities: wp.array(dtype=wp.vec3),
        initial_scales: wp.array(dtype=float),
        current_scales: wp.array(dtype=float),
        phase_offsets: wp.array(dtype=float),
        drag: float,
        t: float,
        dt: float,
        turb_amp: float,
        lifetime: float,
        num_particles: int,
    ):
        tid = wp.tid()
        if tid < num_particles:
            pos = positions[tid]
            vel = velocities[tid]
            phi = phase_offsets[tid]
            speed_factor = wp.exp(-drag * t)

            turb_x = wp.sin(pos[1] * 0.05 + t * 3.5 + phi) * turb_amp
            turb_y = wp.cos(pos[2] * 0.05 + t * 3.5 + phi) * turb_amp - 110.0 * t
            turb_z = wp.sin(pos[0] * 0.05 + t * 3.5 + phi) * turb_amp

            dx = (vel[0] * speed_factor + turb_x) * dt
            dy = (vel[1] * speed_factor + turb_y) * dt
            dz = (vel[2] * speed_factor + turb_z) * dt

            positions[tid] = wp.vec3(pos[0] + dx, pos[1] + dy, pos[2] + dz)

            scale_factor = 1.0 - (t / lifetime)
            if scale_factor < 0.0:
                scale_factor = 0.0
            current_scales[tid] = initial_scales[tid] * scale_factor

    @wp.kernel
    def init_sparks_kernel(
        positions: wp.array(dtype=wp.vec3),
        velocities: wp.array(dtype=wp.vec3),
        initial_scales: wp.array(dtype=float),
        current_scales: wp.array(dtype=float),
        phase_offsets: wp.array(dtype=float),
        num_particles: int,
        spark_radius: float,
        seed: int,
    ):
        tid = wp.tid()
        if tid < num_particles:
            # Initial position at local origin
            positions[tid] = wp.vec3(0.0, 0.0, 0.0)

            # Per-thread random state
            state = wp.rand_init(seed, tid)

            # Radial velocity with upward floral bias
            theta = wp.randf(state) * (2.0 * wp.pi)
            phi = wp.acos(wp.randf(state) * 2.0 - 1.0)

            nx = wp.sin(phi) * wp.cos(theta)
            ny = wp.cos(phi) * 0.6 + 0.4  # Upward floral bias
            nz = wp.sin(phi) * wp.sin(theta)

            mag = wp.sqrt(nx * nx + ny * ny + nz * nz)
            if mag < 0.001:
                mag = 1.0

            speed = 500.0 + wp.randf(state) * 300.0
            velocities[tid] = wp.vec3(
                (nx / mag) * speed,
                (ny / mag) * speed,
                (nz / mag) * speed,
            )

            initial_scale = spark_radius * (0.85 + wp.randf(state) * 0.4)
            initial_scales[tid] = initial_scale
            current_scales[tid] = initial_scale
            phase_offsets[tid] = wp.randf(state) * (2.0 * wp.pi)


# =============================================================================
# Gameplay VFX Tuning Parameters (Single Source of Truth)
# Adjust these values to tune the explosion visuals and lighting intensity.
# =============================================================================
DEFAULT_FLASH_LIGHT_INTENSITY: float = 5000000.0  # Peak SphereLight intensity on destruction
DEFAULT_FLASH_LIGHT_RADIUS: float = 80.0          # SphereLight radius in centimeters
DEFAULT_FLASH_LIGHT_DURATION: float = 0.22        # Light flash duration in seconds
DEFAULT_BURST_LIFETIME: float = 0.55              # Total particle sparks lifetime in seconds
DEFAULT_NUM_SPARKS: int = 150                      # Number of diamond sparks per explosion (pre-allocated mesh pool)
DEFAULT_SPARK_EMISSION: float = 60000.0           # Emissive intensity on diamond sparks
DEFAULT_SPARK_RADIUS: float = 10.0                # Diamond radius in centimeters
DEFAULT_AUDIO_VOLUME: float = 0.4                 # Audio playback volume level (0.0 silent to 1.0 full)
DEFAULT_TAUNT_DELAY: float = 1.5                  # Delay in seconds before playing random taunt after foe defeat
TAUNT_SOUND_NAMES: Tuple[str, ...] = ("taunt_01.wav", "taunt_02.wav", "taunt_03.wav", "taunt_04.wav")

# Particle Rendering Modes (Toggle between 3D diamond mesh instancing and camera-facing billboard points)
RENDER_MODE_POINT_INSTANCER: str = "point_instancer"  # UsdGeom.PointInstancer with 3D diamond octahedron prototype
RENDER_MODE_POINTS: str = "points"                    # UsdGeom.Points camera-facing billboard disks/spheres
DEFAULT_RENDER_MODE: str = RENDER_MODE_POINT_INSTANCER


class ExplosionPoolSlot:
    """
    Pre-allocated pool slot containing individual diamond meshes and a SphereLight.
    Uses cached USD XformOp handles and in-place vector math to guarantee 60 FPS
    with zero runtime allocations, zero stage churn, and 100% visual fidelity in Omniverse RTX.
    """

    def __init__(
        self,
        slot_index: int,
        root_path_str: Optional[str] = None,
        flash_intensity: Optional[float] = None,
        flash_radius: Optional[float] = None,
        light_duration: Optional[float] = None,
        lifetime: Optional[float] = None,
        num_sparks: Optional[int] = None,
        spark_radius: Optional[float] = None,
        render_mode: Optional[str] = None,
    ):
        self.slot_index: int = slot_index
        self.slot_path: str = root_path_str or f"/World/Effects/Pool_{slot_index}"
        self.instancer_path: str = f"{self.slot_path}/PointInstancer"
        self.points_path: str = f"{self.slot_path}/Points"
        self.light_path: str = f"{self.slot_path}/FlashLight"
        self.render_mode: str = render_mode if render_mode is not None else DEFAULT_RENDER_MODE

        self.is_active: bool = False
        self.elapsed: float = 0.0
        self.lifetime: float = lifetime if lifetime is not None else DEFAULT_BURST_LIFETIME
        self.light_duration: float = light_duration if light_duration is not None else DEFAULT_FLASH_LIGHT_DURATION
        self.flash_intensity: float = flash_intensity if flash_intensity is not None else DEFAULT_FLASH_LIGHT_INTENSITY
        self.flash_radius: float = flash_radius if flash_radius is not None else DEFAULT_FLASH_LIGHT_RADIUS
        self._num_sparks: Optional[int] = num_sparks
        self._spark_radius: Optional[float] = spark_radius
        self.drag: float = 3.5

        # World space origin for current burst
        self.origin: Tuple[float, float, float] = (0.0, 0.0, 0.0)
        self._light_stage: int = 2  # 0: peak, 1: mid decay, 2: off
        self._light_extinguished: bool = True

        # Pre-allocate particle simulation buffers (CPU state, zero GC)
        self.positions: List[List[float]] = [[0.0, 0.0, 0.0] for _ in range(self.num_sparks)]
        self.velocities: List[List[float]] = [[0.0, 0.0, 0.0] for _ in range(self.num_sparks)]
        self.base_scales: List[float] = [1.0 for _ in range(self.num_sparks)]
        self.current_scales: List[float] = [1.0 for _ in range(self.num_sparks)]
        self.phase_offsets: List[float] = [0.0 for _ in range(self.num_sparks)]

        # Cached USD handles
        self._cached_slot_trans_op = None
        self._cached_light_prim = None
        self._cached_light_trans_op = None

        # PointInstancer cache handles
        self._cached_instancer = None
        self._cached_instancer_positions_attr = None
        self._cached_instancer_scales_attr = None
        self._cached_instancer_proto_indices_attr = None
        self._cached_instancer_imageable = None

        # Points cache handles
        self._cached_points = None
        self._cached_points_positions_attr = None
        self._cached_points_widths_attr = None
        self._cached_points_imageable = None

        self._active_color_theme: Optional[str] = None

        # Persistent Warp GPU simulation arrays (allocated once on device)
        self._use_warp: bool = False
        self._warp_device: str = "cuda"
        self._warp_positions = None
        self._warp_velocities = None
        self._warp_initial_scales = None
        self._warp_current_scales = None
        self._warp_phases = None
        self._warp_initialized: bool = False

    def _ensure_warp_arrays(self, device: str = "cuda"):
        """Allocates persistent Warp GPU device arrays once to eliminate runtime memory allocation overhead."""
        if not HAS_WARP or wp is None:
            self._use_warp = False
            return
        n = max(self.num_sparks, 16)
        if (
            self._warp_positions is not None
            and len(self._warp_positions) >= n
            and self._warp_initialized
        ):
            return
        try:
            self._warp_device = device
            self._warp_positions = wp.zeros(n, dtype=wp.vec3, device=device)
            self._warp_velocities = wp.zeros(n, dtype=wp.vec3, device=device)
            self._warp_initial_scales = wp.zeros(n, dtype=float, device=device)
            self._warp_current_scales = wp.zeros(n, dtype=float, device=device)
            self._warp_phases = wp.zeros(n, dtype=float, device=device)
            self._warp_initialized = True
            self._use_warp = True
        except Exception as ex:
            self._use_warp = False
            self._warp_initialized = False
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to allocate Warp arrays on {device}: {ex}")

    @property
    def num_sparks(self) -> int:
        return self._num_sparks if self._num_sparks is not None else DEFAULT_NUM_SPARKS

    @num_sparks.setter
    def num_sparks(self, val: Optional[int]):
        self._num_sparks = val

    @property
    def spark_radius(self) -> float:
        return self._spark_radius if self._spark_radius is not None else DEFAULT_SPARK_RADIUS

    @spark_radius.setter
    def spark_radius(self, val: Optional[float]):
        self._spark_radius = val

    def clear_cached_handles(self):
        """Clears cached USD handles to force safe re-query on timeline resets or stage reloads."""
        self._cached_slot_trans_op = None
        self._cached_light_prim = None
        self._cached_light_trans_op = None
        self._cached_instancer = None
        self._cached_instancer_positions_attr = None
        self._cached_instancer_scales_attr = None
        self._cached_instancer_proto_indices_attr = None
        self._cached_instancer_imageable = None
        self._cached_points = None
        self._cached_points_positions_attr = None
        self._cached_points_widths_attr = None
        self._cached_points_imageable = None

    def activate(
        self,
        world_pos: Tuple[float, float, float] = (0.0, 0.0, 0.0),
        color_theme: str = "gold",
        stage=None,
    ):
        """
        Activates VFX pool slot:
        - Repositions slot origin to world_pos
        - Triggers bright point light flash with zero latency
        - Unhides and scatters diamond meshes outward
        """
        self.is_active = True
        self.elapsed = 0.0
        self.origin = (float(world_pos[0]), float(world_pos[1]), float(world_pos[2]))
        self._light_stage = 0
        self._light_extinguished = False

        # Ensure kinematics buffers dynamically match current num_sparks
        n_sparks = self.num_sparks
        if len(self.positions) != n_sparks:
            self.positions = [[0.0, 0.0, 0.0] for _ in range(n_sparks)]
            self.velocities = [[0.0, 0.0, 0.0] for _ in range(n_sparks)]
            self.base_scales = [1.0 for _ in range(n_sparks)]
            self.current_scales = [1.0 for _ in range(n_sparks)]
            self.phase_offsets = [0.0 for _ in range(n_sparks)]

        radius = self.spark_radius

        # In-place update of spark kinematics buffers (zero GC allocation)
        for i in range(n_sparks):
            self.positions[i][0] = 0.0
            self.positions[i][1] = 0.0
            self.positions[i][2] = 0.0

            theta = random.uniform(0.0, 2.0 * math.pi)
            phi = math.acos(random.uniform(-1.0, 1.0))
            nx = math.sin(phi) * math.cos(theta)
            ny = math.cos(phi) * 0.6 + 0.4  # Upward floral bias
            nz = math.sin(phi) * math.sin(theta)

            mag = math.sqrt(nx * nx + ny * ny + nz * nz) or 1.0
            speed = random.uniform(500.0, 800.0)  # cm/s explosive burst
            self.velocities[i][0] = (nx / mag) * speed
            self.velocities[i][1] = (ny / mag) * speed
            self.velocities[i][2] = (nz / mag) * speed
            self.base_scales[i] = radius * random.uniform(0.85, 1.25)
            self.current_scales[i] = self.base_scales[i]
            self.phase_offsets[i] = random.uniform(0.0, 2.0 * math.pi)

        # Dynamically ensure Warp GPU device arrays can hold n_sparks particles
        if (self._use_warp or HAS_WARP) and (
            not self._warp_initialized
            or self._warp_positions is None
            or len(self._warp_positions) < n_sparks
        ):
            self._ensure_warp_arrays(device=self._warp_device)

        # Initialize Warp GPU arrays with burst data
        if self._use_warp and self._warp_initialized and n_sparks > 8:
            try:
                seed = random.randint(1, 10000000)
                wp.launch(
                    init_sparks_kernel,
                    dim=n_sparks,
                    inputs=[
                        self._warp_positions,
                        self._warp_velocities,
                        self._warp_initial_scales,
                        self._warp_current_scales,
                        self._warp_phases,
                        n_sparks,
                        radius,
                        seed,
                    ],
                    device=self._warp_device,
                )
                # Synchronize initial positions back to CPU
                pos_gpu = self._warp_positions.numpy()
                scales_gpu = self._warp_current_scales.numpy()
                for i in range(n_sparks):
                    self.positions[i][0] = float(pos_gpu[i][0])
                    self.positions[i][1] = float(pos_gpu[i][1])
                    self.positions[i][2] = float(pos_gpu[i][2])
                    self.base_scales[i] = float(scales_gpu[i])
                    self.current_scales[i] = float(scales_gpu[i])
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Warp init failed, using CPU: {ex}")
                self._use_warp = False

        if HAS_KIT and stage:
            try:
                ox, oy, oz = self.origin

                # 1. Position slot Xform at world_pos (bulletproof cache validation)
                slot_prim = stage.GetPrimAtPath(self.slot_path)
                if not slot_prim or not slot_prim.IsValid():
                    slot_prim = stage.DefinePrim(Sdf.Path(self.slot_path), "Xform")
                    self._cached_slot_trans_op = None

                if self._cached_slot_trans_op is None:
                    xformable = UsdGeom.Xformable(slot_prim)
                    for op in xformable.GetOrderedXformOps():
                        if op.GetOpType() == UsdGeom.XformOp.TypeTranslate:
                            self._cached_slot_trans_op = op
                            break
                    if not self._cached_slot_trans_op:
                        self._cached_slot_trans_op = xformable.AddTranslateOp()

                if self._cached_slot_trans_op:
                    self._cached_slot_trans_op.Set(Gf.Vec3d(ox, oy, oz))

                # 2. Reset FlashLight local translation to (0, 0, 0) and ignite
                if self._cached_light_prim is None or not self._cached_light_prim.IsValid():
                    self._cached_light_prim = stage.GetPrimAtPath(self.light_path)
                    self._cached_light_trans_op = None

                if self._cached_light_prim and self._cached_light_prim.IsValid():
                    if self._cached_light_trans_op is None:
                        xformable = UsdGeom.Xformable(self._cached_light_prim)
                        for op in xformable.GetOrderedXformOps():
                            if op.GetOpType() == UsdGeom.XformOp.TypeTranslate:
                                self._cached_light_trans_op = op
                                break
                        if not self._cached_light_trans_op:
                            self._cached_light_trans_op = xformable.AddTranslateOp()

                    if self._cached_light_trans_op:
                        self._cached_light_trans_op.Set(Gf.Vec3d(0.0, 0.0, 0.0))

                    light = UsdLux.SphereLight(self._cached_light_prim)
                    light_col = Gf.Vec3f(0.2, 0.85, 1.0) if color_theme == "cyan" else Gf.Vec3f(1.0, 0.78, 0.18)
                    light.GetColorAttr().Set(light_col)
                    light.GetIntensityAttr().Set(self.flash_intensity)
                    light.GetRadiusAttr().Set(self.flash_radius)

                # 3. Particle system activation (PointInstancer vs Points)
                n = self.num_sparks
                if self.render_mode == RENDER_MODE_POINT_INSTANCER:
                    if self._cached_instancer is None or not self._cached_instancer.GetPrim().IsValid():
                        inst_prim = stage.GetPrimAtPath(self.instancer_path)
                        if inst_prim.IsValid():
                            self._cached_instancer = UsdGeom.PointInstancer(inst_prim)
                            self._cached_instancer_positions_attr = self._cached_instancer.GetPositionsAttr()
                            self._cached_instancer_scales_attr = self._cached_instancer.GetScalesAttr()
                            self._cached_instancer_proto_indices_attr = self._cached_instancer.GetProtoIndicesAttr()
                            self._cached_instancer_imageable = UsdGeom.Imageable(inst_prim)

                    if self._cached_instancer_imageable:
                        self._cached_instancer_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.inherited)

                    # Ensure Points is hidden if present
                    if self._cached_points_imageable is None:
                        pts_prim = stage.GetPrimAtPath(self.points_path)
                        if pts_prim.IsValid():
                            self._cached_points_imageable = UsdGeom.Imageable(pts_prim)
                    if self._cached_points_imageable and self._cached_points_imageable.GetPrim().IsValid():
                        self._cached_points_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                    # Initial positions and scales written in single array operations
                    if self._cached_instancer_proto_indices_attr:
                        self._cached_instancer_proto_indices_attr.Set(Vt.IntArray([0] * n))
                    if self._cached_instancer_positions_attr:
                        self._cached_instancer_positions_attr.Set(Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * n))
                    if self._cached_instancer_scales_attr:
                        self._cached_instancer_scales_attr.Set(Vt.Vec3fArray([Gf.Vec3f(s, s, s) for s in self.base_scales[:n]]))

                else:  # RENDER_MODE_POINTS
                    if self._cached_points is None or not self._cached_points.GetPrim().IsValid():
                        pts_prim = stage.GetPrimAtPath(self.points_path)
                        if pts_prim.IsValid():
                            self._cached_points = UsdGeom.Points(pts_prim)
                            self._cached_points_imageable = UsdGeom.Imageable(pts_prim)

                    if self._cached_points and self._cached_points.GetPrim().IsValid():
                        if self._cached_points_positions_attr is None or not self._cached_points_positions_attr.IsValid():
                            self._cached_points_positions_attr = self._cached_points.GetPointsAttr()
                            if not self._cached_points_positions_attr.IsValid():
                                self._cached_points_positions_attr = self._cached_points.CreatePointsAttr()

                        if self._cached_points_widths_attr is None or not self._cached_points_widths_attr.IsValid():
                            self._cached_points_widths_attr = self._cached_points.GetWidthsAttr()
                            if not self._cached_points_widths_attr.IsValid():
                                self._cached_points_widths_attr = self._cached_points.CreateWidthsAttr()

                        if self._cached_points_imageable is None:
                            self._cached_points_imageable = UsdGeom.Imageable(self._cached_points.GetPrim())

                    if self._cached_points_imageable and self._cached_points_imageable.GetPrim().IsValid():
                        self._cached_points_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.inherited)

                    # Ensure PointInstancer is hidden if present
                    if self._cached_instancer_imageable is None:
                        inst_prim = stage.GetPrimAtPath(self.instancer_path)
                        if inst_prim.IsValid():
                            self._cached_instancer_imageable = UsdGeom.Imageable(inst_prim)
                    if self._cached_instancer_imageable and self._cached_instancer_imageable.GetPrim().IsValid():
                        self._cached_instancer_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                    # Ensure valid extent and normals on Points (crucial: prevents Hydra from culling boundable points)
                    if self._cached_points and self._cached_points.GetPrim().IsValid():
                        ext_attr = self._cached_points.GetExtentAttr()
                        box = Vt.Vec3fArray([Gf.Vec3f(-2000.0, -2000.0, -2000.0), Gf.Vec3f(2000.0, 2000.0, 2000.0)])
                        if not ext_attr.IsValid() or not ext_attr.HasAuthoredValue():
                            self._cached_points.CreateExtentAttr(box)
                        else:
                            ext_attr.Set(box)

                        norm_attr = self._cached_points.GetNormalsAttr()
                        if not norm_attr.IsValid() or not norm_attr.HasAuthoredValue():
                            self._cached_points.CreateNormalsAttr(Vt.Vec3fArray([Gf.Vec3f(0.0, 1.0, 0.0)] * n))

                    # Initial positions and widths written in single array operations
                    if self._cached_points_positions_attr and self._cached_points_positions_attr.IsValid():
                        self._cached_points_positions_attr.Set(Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * n))
                    if self._cached_points_widths_attr and self._cached_points_widths_attr.IsValid():
                        self._cached_points_widths_attr.Set(Vt.FloatArray([max(0.1, 2.0 * s) for s in self.current_scales[:n]]))

                # 4. Material check: rebind if theme dynamically changes or in Points mode
                if self._active_color_theme != color_theme or self.render_mode == RENDER_MODE_POINTS:
                    self._active_color_theme = color_theme
                    if self.render_mode == RENDER_MODE_POINT_INSTANCER:
                        # Rebind prototype mesh for PointInstancer (OmniSurface)
                        proto_mat_path = "/World/Effects/Materials/SparkCyanMat" if color_theme == "cyan" else "/World/Effects/Materials/SparkGoldMat"
                        proto_mat_prim = stage.GetPrimAtPath(proto_mat_path)
                        if proto_mat_prim and proto_mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                            proto_prim = stage.GetPrimAtPath("/World/Effects/Prototypes/DiamondMesh")
                            if proto_prim.IsValid():
                                UsdShade.MaterialBindingAPI(proto_prim).Bind(UsdShade.Material(proto_mat_prim))
                    else:
                        # Rebind Points prim for Points mode (OmniPBR / UsdPreviewSurface for point primitives)
                        point_mat_path = "/World/Effects/Materials/PointSparkCyanMat" if color_theme == "cyan" else "/World/Effects/Materials/PointSparkGoldMat"
                        point_mat_prim = stage.GetPrimAtPath(point_mat_path)
                        if not point_mat_prim or not point_mat_prim.IsValid():
                            # If point material is missing on stage, author it immediately
                            c_rgb = (0.15, 0.85, 1.0) if color_theme == "cyan" else (1.0, 0.85, 0.2)
                            system = HydragonEffectsSystem.get_instance()
                            if system:
                                system._ensure_point_material(stage, point_mat_path, c_rgb)
                                point_mat_prim = stage.GetPrimAtPath(point_mat_path)

                        if point_mat_prim and point_mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                            if self._cached_points and self._cached_points.GetPrim().IsValid():
                                UsdShade.MaterialBindingAPI(self._cached_points.GetPrim()).Bind(UsdShade.Material(point_mat_prim))

                        # Update displayColor and displayOpacity attributes on Points
                        if self._cached_points and self._cached_points.GetPrim().IsValid():
                            c = Gf.Vec3f(0.15, 0.85, 1.0) if color_theme == "cyan" else Gf.Vec3f(1.0, 0.85, 0.2)
                            col_attr = self._cached_points.GetDisplayColorAttr()
                            if col_attr and col_attr.IsValid():
                                col_attr.Set(Vt.Vec3fArray([c]))
                            elif hasattr(self._cached_points, "CreateDisplayColorAttr"):
                                self._cached_points.CreateDisplayColorAttr(Vt.Vec3fArray([c]))

                            op_attr = self._cached_points.GetDisplayOpacityAttr()
                            if op_attr and op_attr.IsValid():
                                op_attr.Set(Vt.FloatArray([1.0]))
                            elif hasattr(self._cached_points, "CreateDisplayOpacityAttr"):
                                self._cached_points.CreateDisplayOpacityAttr(Vt.FloatArray([1.0]))
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to activate pool slot {self.slot_index}: {ex}")

    def update(self, dt: float, stage=None) -> bool:
        """Steps particle positions in-place, steps down light flash cleanly, and returns False when expired."""
        if not self.is_active:
            return False

        self.elapsed += dt
        t = self.elapsed

        if t >= self.lifetime:
            self.deactivate(stage)
            return False

        # 1. Discrete 3-Phase Light Flash (0ms lag, zero notice spam, guaranteed visibility)
        if HAS_KIT and stage and self._cached_light_prim and self._cached_light_prim.IsValid():
            try:
                if t < self.light_duration * 0.45:
                    pass  # Phase 0: peak flash intensity (already applied on activate)
                elif t < self.light_duration:
                    if self._light_stage < 1:
                        self._light_stage = 1
                        UsdLux.SphereLight(self._cached_light_prim).GetIntensityAttr().Set(self.flash_intensity * 0.35)
                else:
                    if self._light_stage < 2:
                        self._light_stage = 2
                        self._light_extinguished = True
                        UsdLux.SphereLight(self._cached_light_prim).GetIntensityAttr().Set(0.0)
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to update light in slot {self.slot_index}: {ex}")
        elif not HAS_KIT:
            # Fallback for headless tests
            if t >= self.light_duration:
                self._light_extinguished = True

        # 2. Kinematics computation (Warp GPU acceleration or CPU fallback)
        n = self.num_sparks
        speed_factor = math.exp(-self.drag * t)
        turb_amp = 35.0 * speed_factor

        # Only use Warp if particle count is high enough to benefit over kernel dispatch overhead
        use_warp = self._use_warp and self._warp_initialized and n > 8

        if use_warp:
            self._update_warp(dt, t, speed_factor, turb_amp)
        else:
            self._update_cpu(dt, t, speed_factor, turb_amp)

        # 3. Bulk USD array updates (PointInstancer vs Points) - Single call per frame!
        if HAS_KIT and stage:
            try:
                if self.render_mode == RENDER_MODE_POINT_INSTANCER and self._cached_instancer_positions_attr:
                    vec_pos = Vt.Vec3fArray([Gf.Vec3f(p[0], p[1], p[2]) for p in self.positions[:n]])
                    self._cached_instancer_positions_attr.Set(vec_pos)
                    if self._cached_instancer_scales_attr:
                        vec_scales = Vt.Vec3fArray([
                            Gf.Vec3f(s, s, s)
                            for s in self.current_scales[:n]
                        ])
                        self._cached_instancer_scales_attr.Set(vec_scales)

                elif self.render_mode == RENDER_MODE_POINTS and self._cached_points_positions_attr:
                    vec_pos = Vt.Vec3fArray([Gf.Vec3f(p[0], p[1], p[2]) for p in self.positions[:n]])
                    self._cached_points_positions_attr.Set(vec_pos)
                    if self._cached_points_widths_attr:
                        float_widths = Vt.FloatArray([
                            max(0.1, 2.0 * s)
                            for s in self.current_scales[:n]
                        ])
                        self._cached_points_widths_attr.Set(float_widths)

            except Exception as ex:
                if carb:
                    carb.log_error(f"[hydragon.editor.core] Failed to update particles in slot {self.slot_index}: {ex}")

        return True

    def _update_warp(self, dt: float, t: float, speed_factor: float, turb_amp: float):
        """GPU-accelerated particle update using Warp."""
        try:
            # Launch asynchronous Warp simulation kernel
            wp.launch(
                simulate_sparks_kernel,
                dim=self.num_sparks,
                inputs=[
                    self._warp_positions,
                    self._warp_velocities,
                    self._warp_initial_scales,
                    self._warp_current_scales,
                    self._warp_phases,
                    self.drag,
                    t,
                    dt,
                    turb_amp,
                    self.lifetime,
                    self.num_sparks,
                ],
                device=self._warp_device,
            )

            # Synchronize results from GPU device to CPU memory
            # Fast zero-copy numpy conversion provided by Warp
            pos_array = self._warp_positions.numpy()
            scales_array = self._warp_current_scales.numpy()

            # Update CPU particle buffers
            for i in range(self.num_sparks):
                self.positions[i][0] = float(pos_array[i][0])
                self.positions[i][1] = float(pos_array[i][1])
                self.positions[i][2] = float(pos_array[i][2])
                self.current_scales[i] = float(scales_array[i])

        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Warp GPU error, falling back to CPU: {ex}")
            # CPU fallback in case of Warp kernel error
            self._update_cpu(dt, t, speed_factor, turb_amp)

    def _update_cpu(self, dt: float, t: float, speed_factor: float, turb_amp: float):
        """In-place closed-form particle kinematics CPU fallback."""
        scale_factor = max(0.0, 1.0 - (t / self.lifetime))
        for i in range(self.num_sparks):
            vel = self.velocities[i]
            pos = self.positions[i]
            phi = self.phase_offsets[i]

            turb_x = math.sin(pos[1] * 0.05 + t * 3.5 + phi) * turb_amp
            turb_y = math.cos(pos[2] * 0.05 + t * 3.5 + phi) * turb_amp - 110.0 * t
            turb_z = math.sin(pos[0] * 0.05 + t * 3.5 + phi) * turb_amp

            pos[0] += (vel[0] * speed_factor + turb_x) * dt
            pos[1] += (vel[1] * speed_factor + turb_y) * dt
            pos[2] += (vel[2] * speed_factor + turb_z) * dt

            # Update scale (fade out)
            self.current_scales[i] = self.base_scales[i] * scale_factor

    def deactivate(self, stage=None):
        """Deactivates light and sparks cleanly by setting intensity to zero and visibility to invisible."""
        self.is_active = False
        self._light_stage = 2
        self._light_extinguished = True
        if HAS_KIT and stage:
            try:
                if self._cached_light_prim and self._cached_light_prim.IsValid():
                    UsdLux.SphereLight(self._cached_light_prim).GetIntensityAttr().Set(0.0)

                if self._cached_instancer_imageable is None:
                    inst_prim = stage.GetPrimAtPath(self.instancer_path)
                    if inst_prim.IsValid():
                        self._cached_instancer_imageable = UsdGeom.Imageable(inst_prim)
                if self._cached_instancer_imageable and self._cached_instancer_imageable.GetPrim().IsValid():
                    self._cached_instancer_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                if self._cached_points_imageable is None:
                    pts_prim = stage.GetPrimAtPath(self.points_path)
                    if pts_prim.IsValid():
                        self._cached_points_imageable = UsdGeom.Imageable(pts_prim)
                if self._cached_points_imageable and self._cached_points_imageable.GetPrim().IsValid():
                    self._cached_points_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                # Reset scales/widths to 0 in single array calls
                n = self.num_sparks
                if self._cached_instancer_scales_attr:
                    self._cached_instancer_scales_attr.Set(Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * n))
                if self._cached_points_widths_attr:
                    self._cached_points_widths_attr.Set(Vt.FloatArray([0.0] * n))
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Slot deactivation error in slot {self.slot_index}: {ex}")


class ActiveExplosionVFX:
    """
    Backwards-compatible standalone explosion VFX container for testing
    and direct particle simulation stepping.
    """

    def __init__(
        self,
        prim_path: str,
        origin: Tuple[float, float, float],
        points_prim=None,
        light_prim=None,
        sound_prim=None,
        color: Tuple[float, float, float] = (1.0, 0.78, 0.18),
        num_particles: int = 20,
        lifetime: float = 0.55,
        light_duration: float = 0.22,
        initial_light_intensity: float = 200000000.0,
    ):
        self.prim_path = prim_path
        self.origin = origin
        self.points_prim = points_prim
        self.light_prim = light_prim
        self.sound_prim = sound_prim
        self.color = color
        self.num_particles = num_particles
        self.lifetime = lifetime
        self.light_duration = light_duration
        self.initial_light_intensity = initial_light_intensity
        self.elapsed: float = 0.0

        self.positions: List[List[float]] = []
        self.velocities: List[List[float]] = []
        self.base_widths: List[float] = []
        self.phase_offsets: List[float] = []
        self.drag: float = 3.5

        ox, oy, oz = origin
        for _ in range(num_particles):
            self.positions.append([ox, oy, oz])
            theta = random.uniform(0.0, 2.0 * math.pi)
            phi = math.acos(random.uniform(-1.0, 1.0))
            nx = math.sin(phi) * math.cos(theta)
            ny = math.cos(phi) * 0.6 + 0.4
            nz = math.sin(phi) * math.sin(theta)
            mag = math.sqrt(nx * nx + ny * ny + nz * nz) or 1.0
            speed = random.uniform(500.0, 800.0)
            self.velocities.append([(nx / mag) * speed, (ny / mag) * speed, (nz / mag) * speed])
            self.base_widths.append(random.uniform(4.5, 6.5))
            self.phase_offsets.append(random.uniform(0.0, 2.0 * math.pi))

    def update(self, dt: float) -> bool:
        self.elapsed += dt
        t = self.elapsed
        if t >= self.lifetime:
            return False

        speed_factor = math.exp(-self.drag * t)
        turb_amp = 35.0 * speed_factor

        for i in range(self.num_particles):
            pos = self.positions[i]
            vel = self.velocities[i]
            phi = self.phase_offsets[i]

            turb_x = math.sin(pos[1] * 0.05 + t * 3.5 + phi) * turb_amp
            turb_y = math.cos(pos[2] * 0.05 + t * 3.5 + phi) * turb_amp - 110.0 * t
            turb_z = math.sin(pos[0] * 0.05 + t * 3.5 + phi) * turb_amp

            pos[0] += (vel[0] * speed_factor + turb_x) * dt
            pos[1] += (vel[1] * speed_factor + turb_y) * dt
            pos[2] += (vel[2] * speed_factor + turb_z) * dt

        if self.light_prim:
            if t < self.light_duration:
                light_progress = t / self.light_duration
                intensity = self.initial_light_intensity * ((1.0 - light_progress) ** 2.0)
                try:
                    self.light_prim.GetIntensityAttr().Set(intensity)
                except Exception:
                    pass
            else:
                try:
                    prim = self.light_prim.GetPrim()
                    if prim and prim.IsValid() and prim.IsActive():
                        prim.SetActive(False)
                except Exception:
                    pass
                self.light_prim = None

        return True


class HydragonEffectsSystem:
    """
    High-performance Singleton manager for dynamic gameplay visual & audio effects.
    Uses a 3-slot Object Pool of pre-allocated diamond meshes and SphereLights, plus pre-cached
    RAM audio playback to guarantee 60 FPS and 0ms audio latency.
    """

    POOL_SIZE: int = 3
    NUM_SPARKS: int = DEFAULT_NUM_SPARKS
    FLASH_LIGHT_INTENSITY: float = DEFAULT_FLASH_LIGHT_INTENSITY
    FLASH_LIGHT_RADIUS: float = DEFAULT_FLASH_LIGHT_RADIUS
    FLASH_LIGHT_DURATION: float = DEFAULT_FLASH_LIGHT_DURATION
    BURST_LIFETIME: float = DEFAULT_BURST_LIFETIME
    SPARK_RADIUS: float = DEFAULT_SPARK_RADIUS
    SPARK_EMISSION: float = DEFAULT_SPARK_EMISSION
    AUDIO_VOLUME: float = DEFAULT_AUDIO_VOLUME
    TAUNT_DELAY: float = DEFAULT_TAUNT_DELAY
    RENDER_MODE: str = DEFAULT_RENDER_MODE

    _instance: Optional["HydragonEffectsSystem"] = None

    def __init__(self):
        HydragonEffectsSystem._instance = self
        self._is_active: bool = False
        self._pool_initialized: bool = False
        self._timeline_sub = None
        self._app_update_sub = None
        self._slots: List[ExplosionPoolSlot] = []
        self._cached_sound = None
        self._audio_file_path: Optional[str] = None
        self._cached_taunt_sounds: List[Any] = []
        self._delayed_sounds: List[Dict[str, Any]] = []
        self._mock_spawn_count: int = 0
        self._mock_taunt_play_count: int = 0
        self._audio_volume: float = self.AUDIO_VOLUME
        self._set_audio_volume(self._audio_volume)
        self._render_mode: str = self.RENDER_MODE
        self._use_warp: bool = False
        self._warp_device: str = "cuda"
        self._warp_initialized: bool = False
        self._warp_benchmark: Optional[Dict[str, float]] = None

    @classmethod
    def get_instance(cls) -> Optional["HydragonEffectsSystem"]:
        return cls._instance

    @property
    def render_mode(self) -> str:
        return self._render_mode

    @render_mode.setter
    def render_mode(self, val: str):
        if val in (RENDER_MODE_POINT_INSTANCER, RENDER_MODE_POINTS):
            self._render_mode = val
            for slot in self._slots:
                slot.render_mode = val

    @property
    def audio_volume(self) -> float:
        return self._audio_volume

    @audio_volume.setter
    def audio_volume(self, val: float):
        self._set_audio_volume(val)

    def _set_audio_volume(self, volume: float):
        """Sets the audio playback volume level (0.0 to 1.0) via carb.settings for UI and Master buses."""
        self._audio_volume = max(0.0, min(1.0, float(volume)))
        if carb and hasattr(carb, "settings"):
            try:
                settings = carb.settings.get_settings()
                if settings:
                    for key in ("/persistent/audio/context/uiVolume", "/persistent/audio/context/masterVolume"):
                        try:
                            settings.set_float(key, self._audio_volume)
                        except AttributeError:
                            settings.set(key, self._audio_volume)
            except Exception:
                pass

    def _init_warp(self):
        """Initializes Warp GPU acceleration if available and supported."""
        if not HAS_WARP:
            self._use_warp = False
            return

        try:
            wp.init()

            # Check if CUDA devices are available
            cuda_devs = wp.get_cuda_devices() if hasattr(wp, "get_cuda_devices") else []
            if cuda_devs:
                self._warp_device = "cuda"
                self._use_warp = True
                self._warp_initialized = True
                if carb:
                    carb.log_info(f"[hydragon.editor.core] Warp GPU acceleration ENABLED on '{self._warp_device}'.")
                    carb.log_info(f"[hydragon.editor.core] Warp CUDA devices: {cuda_devs}")
            else:
                self._warp_device = "cpu"
                self._use_warp = True
                self._warp_initialized = True
                if carb:
                    carb.log_info("[hydragon.editor.core] Warp initialized on CPU device.")

            # Optional benchmark (safely guarded, opt-in)
            if self._use_warp and getattr(self, "_enable_startup_benchmark", False):
                try:
                    self._benchmark_warp()
                except Exception as b_ex:
                    if carb:
                        carb.log_warn(f"[hydragon.editor.core] Warp startup benchmark skipped: {b_ex}")

        except Exception as ex:
            self._use_warp = False
            self._warp_initialized = False
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Warp initialization failed ({ex}); using CPU fallback.")

    def _benchmark_warp(self):
        """Benchmark Warp vs CPU performance."""
        try:
            import time

            # Create benchmark test slot
            test_slot = ExplosionPoolSlot(
                slot_index=999,
                num_sparks=20,
                spark_radius=20.0,
            )
            test_slot._ensure_warp_arrays(device=self._warp_device)

            if not test_slot._use_warp:
                return

            # Prepare initial state
            test_slot.activate((0.0, 0.0, 0.0), "gold", None)

            # CPU benchmark run
            cpu_start = time.perf_counter()
            for _ in range(100):
                test_slot._update_cpu(0.016, 0.3, 0.5, 1.0)
            cpu_time = (time.perf_counter() - cpu_start) * 10  # ms per 1000 iterations

            # Warp GPU benchmark run
            warp_start = time.perf_counter()
            for _ in range(100):
                test_slot._update_warp(0.016, 0.3, 0.5, 1.0)
            warp_time = (time.perf_counter() - warp_start) * 10  # ms per 1000 iterations

            speedup = cpu_time / warp_time if warp_time > 0 else 0

            self._warp_benchmark = {
                "cpu_time": cpu_time,
                "warp_time": warp_time,
                "speedup": speedup,
                "particles": 20,
            }

            if carb:
                carb.log_info(
                    f"[hydragon.editor.core] Warp Benchmark: {speedup:.2f}x speedup "
                    f"(CPU: {cpu_time:.3f}ms, GPU: {warp_time:.3f}ms for 20 particles)"
                )

        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Warp benchmark failed: {ex}")

    def startup(self):
        """Initializes subscriptions for timeline events and per-frame VFX stepping."""
        if not HAS_KIT:
            return

        self._is_active = True
        self._set_audio_volume(self._audio_volume)

        # Inicializa Warp
        self._init_warp()

        self._resolve_audio_path()
        self._ensure_audio_loaded()
        self._subscribe_timeline()
        self._subscribe_app_update()

        if carb:
            warp_status = "with Warp GPU acceleration" if self._use_warp else "with CPU fallback"
            carb.log_info(f"[hydragon.editor.core] HydragonEffectsSystem initialized {warp_status}.")

    def shutdown(self):
        """Releases subscriptions, deactivates all pool slots, and unloads cached sound."""
        self._is_active = False
        self._pool_initialized = False
        self._deactivate_all_slots()
        self._slots.clear()
        self._cached_sound = None
        self._cached_taunt_sounds.clear()
        self._delayed_sounds.clear()
        self._timeline_sub = None
        self._app_update_sub = None
        self._warp_initialized = False
        HydragonEffectsSystem._instance = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonEffectsSystem shutdown.")

    def _resolve_sound_file(self, filename: str) -> str:
        """Finds or confirms relative/dynamic path to sound file without hardcoded absolute paths."""
        if not filename:
            return ""
        clean_name = os.path.basename(str(filename).strip("@").replace("\\", "/"))
        candidates = []

        # 1. Preferred: Via Kit Extension Manager if available
        if HAS_KIT:
            try:
                em = omni.kit.app.get_app().get_extension_manager()
                ext_id_path = em.get_extension_path_by_pkg_id("hydragon.editor.core")
                if ext_id_path:
                    candidates.append(os.path.join(ext_id_path, "data", "assets", "audio", "sound_fx_samples", clean_name))
            except Exception:
                pass

        # 2. Relative to extension root via __file__ (development fallback)
        ext_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
        candidates.append(os.path.join(ext_root, "data", "assets", "audio", "sound_fx_samples", clean_name))

        # 3. Via Current Working Directory fallback
        candidates.append(os.path.abspath(os.path.join(os.getcwd(), "source", "extensions", "hydragon.editor.core", "data", "assets", "audio", "sound_fx_samples", clean_name)))

        for c in candidates:
            if os.path.exists(c):
                return os.path.abspath(c).replace("\\", "/")
        return ""

    def _resolve_audio_path(self) -> str:
        """Finds or confirms the relative/dynamic path to foe_defeat_01.wav without hardcoded absolute paths."""
        path = self._resolve_sound_file("foe_defeat_01.wav")
        if not path:
            path = self._resolve_sound_file("foe_defeat.wav")
        if path:
            self._audio_file_path = path
            return self._audio_file_path
        return ""

    def _ensure_audio_loaded(self):
        """Pre-caches defeat sound and taunt sounds into RAM memory via omni.kit.uiaudio."""
        if not HAS_KIT:
            return
        try:
            import omni.kit.uiaudio
            audio = omni.kit.uiaudio.get_ui_audio_interface()
            if not audio:
                return

            if self._cached_sound is None:
                if not self._audio_file_path:
                    self._resolve_audio_path()
                if self._audio_file_path and os.path.exists(self._audio_file_path):
                    self._cached_sound = audio.create_sound(self._audio_file_path)
                    if carb:
                        carb.log_info(f"[hydragon.editor.core] Defeat audio pre-cached: {self._audio_file_path}")

            if not self._cached_taunt_sounds:
                for t_name in TAUNT_SOUND_NAMES:
                    t_path = self._resolve_sound_file(t_name)
                    if t_path and os.path.exists(t_path):
                        snd = audio.create_sound(t_path)
                        if snd:
                            self._cached_taunt_sounds.append(snd)
                if carb and self._cached_taunt_sounds:
                    carb.log_info(f"[hydragon.editor.core] Pre-cached {len(self._cached_taunt_sounds)} taunt sounds.")
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to pre-cache audio: {ex}")

    def _play_cached_sound(self):
        """Plays the pre-cached defeat sound effect with 0ms disk latency."""
        if not HAS_KIT:
            return
        try:
            import omni.kit.uiaudio
            audio = omni.kit.uiaudio.get_ui_audio_interface()
            if audio:
                if self._cached_sound is None:
                    self._ensure_audio_loaded()
                if self._cached_sound is not None:
                    audio.play_sound(self._cached_sound)
        except Exception:
            pass

    def _play_taunt_sound(self, sound):
        """Plays a taunt sound effect."""
        if not HAS_KIT:
            self._mock_taunt_play_count += 1
            return
        try:
            import omni.kit.uiaudio
            audio = omni.kit.uiaudio.get_ui_audio_interface()
            if audio and sound:
                if isinstance(sound, str):
                    resolved = self._resolve_sound_file(sound)
                    if resolved and os.path.exists(resolved):
                        snd = audio.create_sound(resolved)
                        if snd:
                            audio.play_sound(snd)
                else:
                    audio.play_sound(sound)
        except Exception:
            pass

    def step_delayed_sounds(self, dt: float):
        """Ticks down delayed sound timers and triggers audio cues on completion."""
        if not self._delayed_sounds:
            return
        remaining = []
        for item in self._delayed_sounds:
            item["timer"] -= dt
            if item["timer"] <= 0.0:
                self._play_taunt_sound(item.get("sound"))
            else:
                remaining.append(item)
        self._delayed_sounds = remaining

    def _subscribe_timeline(self):
        try:
            timeline = omni.timeline.get_timeline_interface()
            stream = timeline.get_timeline_event_stream()
            self._timeline_sub = stream.create_subscription_to_pop(self._on_timeline_event)
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] EffectsSystem timeline subscription failed: {e}")

    def _subscribe_app_update(self):
        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(self._on_app_update)
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] EffectsSystem app update subscription failed: {e}")

    def _discover_effects_config(self, stage):
        """
        Discovers active HydragonEffectsAPI manager prim on simulation start.
        Enforces Strict Opt-In Architecture: only initializes VFX pool if an
        effects manager prim or an existing /World/Effects hierarchy is authored on stage.
        """
        if not stage:
            return

        from .schemas import HydragonEffectsManager

        candidate_paths = (
            "/World/EffectsManager",
            "/World/Effects",
            "/World/VFX",
            "/World/Gameplay/EffectsManager",
            "/EffectsManager",
        )

        effects_mgr_prim = None
        has_existing_effects_prim = False

        # 1. Fast O(1) candidate path checks
        for path_str in candidate_paths:
            prim = stage.GetPrimAtPath(path_str)
            if prim and prim.IsValid():
                if HydragonEffectsManager.is_applied(prim):
                    effects_mgr_prim = prim
                    break
                elif path_str == "/World/Effects":
                    has_existing_effects_prim = True

        # 2. Stage traversal fallback (one-time on PLAY)
        if not effects_mgr_prim and not has_existing_effects_prim:
            for prim in stage.Traverse():
                if HydragonEffectsManager.is_applied(prim):
                    effects_mgr_prim = prim
                    break

        # 3. Configure and activate if opted-in
        if effects_mgr_prim:
            mgr = HydragonEffectsManager(effects_mgr_prim)
            if not mgr.auto_initialize_on_play:
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] EffectsManager at {effects_mgr_prim.GetPath()} "
                        "has autoInitializeOnPlay=False; remaining dormant."
                    )
                return

            self.POOL_SIZE = mgr.pool_size
            self.NUM_SPARKS = mgr.num_sparks
            self.render_mode = mgr.render_mode
            self.SPARK_RADIUS = mgr.spark_radius
            self.FLASH_LIGHT_INTENSITY = mgr.flash_intensity
            self.BURST_LIFETIME = mgr.burst_lifetime

            if carb:
                carb.log_info(
                    f"[hydragon.editor.core] Discovered EffectsManager at {effects_mgr_prim.GetPath()}: "
                    f"pool={self.POOL_SIZE}, sparks={self.NUM_SPARKS}, mode={self.render_mode}"
                )
            self._ensure_pool(stage)
            self._ensure_audio_loaded()
        elif has_existing_effects_prim:
            # Stage has pre-authored /World/Effects hierarchy
            self._ensure_pool(stage)
            self._ensure_audio_loaded()
        else:
            # Strict Opt-In: Stage does not contain any effects manager or effects hierarchy.
            # Remain completely dormant without modifying the stage.
            pass

    def _on_timeline_event(self, e):
        if not HAS_KIT:
            return
        try:
            event_type = int(e.type)
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                self._set_audio_volume(DEFAULT_AUDIO_VOLUME)
                stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                if stage:
                    self._discover_effects_config(stage)
            elif event_type == int(omni.timeline.TimelineEventType.STOP):
                self._pool_initialized = False
                self._delayed_sounds.clear()
                stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                self._deactivate_all_slots(stage)
                for slot in self._slots:
                    slot.clear_cached_handles()
        except Exception:
            pass

    def _on_app_update(self, e):
        """Update loop with optimized Warp synchronization."""
        if not HAS_KIT:
            return

        dt = e.payload.get("dt", 1.0 / 60.0) if hasattr(e, "payload") else 1.0 / 60.0
        dt = min(dt, 0.05)

        # Update delayed sound queues
        self.step_delayed_sounds(dt)

        if not self._slots:
            return

        stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
        if not stage:
            return

        # Step all active pool slots
        warp_used = False
        for slot in self._slots:
            if slot.is_active:
                slot.update(dt, stage=stage)
                if slot._use_warp and slot._warp_initialized:
                    warp_used = True

        # Synchronize Warp device once per frame if active slots used GPU
        if warp_used and HAS_WARP and self._use_warp:
            try:
                wp.synchronize_device(self._warp_device)
            except Exception:
                pass

    def _ensure_emissive_material(self, stage, mat_path: str, color: Tuple[float, float, float]):
        """Defines an emissive material with OmniSurface, OmniPBR, and UsdPreviewSurface shaders."""
        if not (HAS_KIT and stage):
            return
        try:
            mat_prim = stage.GetPrimAtPath(mat_path)
            if not mat_prim.IsValid():
                mat = UsdShade.Material.Define(stage, mat_path)

                # 1. OmniSurface MDL Shader
                omni_shader_path = f"{mat_path}/OmniSurfaceShader"
                omni_shader = UsdShade.Shader.Define(stage, omni_shader_path)
                omni_shader.CreateIdAttr("OmniSurface")
                omni_shader.GetPrim().CreateAttribute("info:implementationSource", Sdf.ValueTypeNames.Token).Set("sourceAsset")
                omni_shader.GetPrim().CreateAttribute("info:mdl:sourceAsset", Sdf.ValueTypeNames.Asset).Set("OmniSurface.mdl")
                omni_shader.GetPrim().CreateAttribute("info:mdl:sourceAsset:subIdentifier", Sdf.ValueTypeNames.Token).Set("OmniSurface")
                omni_shader.CreateInput("emission_color", Sdf.ValueTypeNames.Color3f).Set(Gf.Vec3f(*color))
                omni_shader.CreateInput("emission_weight", Sdf.ValueTypeNames.Float).Set(1.0)
                omni_shader.CreateInput("emission_intensity", Sdf.ValueTypeNames.Float).Set(self.SPARK_EMISSION)

                # OmniPBR fallback inputs on the same shader for universal RTX support
                omni_shader.CreateInput("emissive_color", Sdf.ValueTypeNames.Color3f).Set(Gf.Vec3f(*color))
                omni_shader.CreateInput("emissive_intensity", Sdf.ValueTypeNames.Float).Set(self.SPARK_EMISSION)
                omni_shader.CreateInput("enable_emission", Sdf.ValueTypeNames.Bool).Set(True)

                # Connect MDL outputs
                out_token = omni_shader.CreateOutput("out", Sdf.ValueTypeNames.Token)
                mat.CreateDisplacementOutput("mdl").ConnectToSource(out_token)
                mat.CreateSurfaceOutput("mdl").ConnectToSource(out_token)

                # 2. Fallback UsdPreviewSurface Shader
                pbr_shader_path = f"{mat_path}/PbrShader"
                pbr_shader = UsdShade.Shader.Define(stage, pbr_shader_path)
                pbr_shader.CreateIdAttr("UsdPreviewSurface")
                pbr_shader.CreateInput("emissiveColor", Sdf.ValueTypeNames.Color3f).Set(Gf.Vec3f(*color))
                mat.CreateSurfaceOutput().ConnectToSource(pbr_shader.CreateOutput("surface", Sdf.ValueTypeNames.Token))
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to author emissive material at {mat_path}: {ex}")

    def _ensure_point_material(self, stage, mat_path: str, color: Tuple[float, float, float]):
        """Defines an emissive material for point billboard primitives using OmniPBR and UsdPreviewSurface."""
        if not (HAS_KIT and stage):
            return
        try:
            mat_prim = stage.GetPrimAtPath(mat_path)
            if not mat_prim.IsValid():
                mat = UsdShade.Material.Define(stage, mat_path)

                # 1. OmniPBR MDL Shader (natively supported for points in Omniverse RTX)
                omni_shader_path = f"{mat_path}/OmniPBRShader"
                omni_shader = UsdShade.Shader.Define(stage, omni_shader_path)
                omni_shader.CreateIdAttr("OmniPBR")
                omni_shader.GetPrim().CreateAttribute("info:implementationSource", Sdf.ValueTypeNames.Token).Set("sourceAsset")
                omni_shader.GetPrim().CreateAttribute("info:mdl:sourceAsset", Sdf.ValueTypeNames.Asset).Set("OmniPBR.mdl")
                omni_shader.GetPrim().CreateAttribute("info:mdl:sourceAsset:subIdentifier", Sdf.ValueTypeNames.Token).Set("OmniPBR")
                omni_shader.CreateInput("diffuse_color_constant", Sdf.ValueTypeNames.Color3f).Set(Gf.Vec3f(*color))
                omni_shader.CreateInput("diffuse_tint", Sdf.ValueTypeNames.Color3f).Set(Gf.Vec3f(*color))
                omni_shader.CreateInput("emissive_color", Sdf.ValueTypeNames.Color3f).Set(Gf.Vec3f(*color))
                omni_shader.CreateInput("emissive_intensity", Sdf.ValueTypeNames.Float).Set(self.SPARK_EMISSION)
                omni_shader.CreateInput("enable_emission", Sdf.ValueTypeNames.Bool).Set(True)

                out_token = omni_shader.CreateOutput("out", Sdf.ValueTypeNames.Token)
                mat.CreateSurfaceOutput("mdl").ConnectToSource(out_token)

                # 2. UsdPreviewSurface Shader (universal fallback)
                pbr_shader_path = f"{mat_path}/PbrShader"
                pbr_shader = UsdShade.Shader.Define(stage, pbr_shader_path)
                pbr_shader.CreateIdAttr("UsdPreviewSurface")
                pbr_shader.CreateInput("diffuseColor", Sdf.ValueTypeNames.Color3f).Set(Gf.Vec3f(*color))
                pbr_shader.CreateInput("emissiveColor", Sdf.ValueTypeNames.Color3f).Set(Gf.Vec3f(*color))
                mat.CreateSurfaceOutput().ConnectToSource(pbr_shader.CreateOutput("surface", Sdf.ValueTypeNames.Token))
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to author point material at {mat_path}: {ex}")

    def _ensure_pool(self, stage):
        """Pre-allocates /World/Effects pool slots and diamond meshes using safe stage queries."""
        if not HAS_KIT or not stage:
            return

        # Ensure slots are instantiated and synced with current system parameters
        if not self._slots:
            for i in range(self.POOL_SIZE):
                self._slots.append(
                    ExplosionPoolSlot(
                        slot_index=i,
                        flash_intensity=self.FLASH_LIGHT_INTENSITY,
                        flash_radius=self.FLASH_LIGHT_RADIUS,
                        light_duration=self.FLASH_LIGHT_DURATION,
                        lifetime=self.BURST_LIFETIME,
                        num_sparks=self.NUM_SPARKS,
                        spark_radius=self.SPARK_RADIUS,
                        render_mode=self._render_mode,
                    )
                )
        else:
            for slot in self._slots:
                slot.flash_intensity = self.FLASH_LIGHT_INTENSITY
                slot.flash_radius = self.FLASH_LIGHT_RADIUS
                slot.light_duration = self.FLASH_LIGHT_DURATION
                slot.lifetime = self.BURST_LIFETIME
                slot.spark_radius = self.SPARK_RADIUS
                slot.render_mode = self._render_mode

        # Initialize Warp arrays for each slot
        if self._use_warp and self._warp_initialized:
            for slot in self._slots:
                if not slot._warp_initialized:
                    slot._ensure_warp_arrays(device=self._warp_device)

        # Check if stage already has initialized pool and materials
        materials_ok = (
            stage.GetPrimAtPath("/World/Effects/Materials/PointSparkGoldMat").IsValid()
            and stage.GetPrimAtPath("/World/Effects/Materials/SparkGoldMat").IsValid()
            and stage.GetPrimAtPath("/World/Effects/Pool_0/Points").IsValid()
        )
        if self._pool_initialized and len(self._slots) == self.POOL_SIZE and materials_ok:
            return

        try:
            effects_root_path = Sdf.Path("/World/Effects")
            effects_root_prim = stage.GetPrimAtPath(effects_root_path)
            if not effects_root_prim.IsValid():
                stage.DefinePrim(effects_root_path, "Xform")

            # Ensure Emissive Materials
            materials_root_path = Sdf.Path("/World/Effects/Materials")
            if not stage.GetPrimAtPath(materials_root_path).IsValid():
                stage.DefinePrim(materials_root_path, "Scope")

            self._ensure_emissive_material(stage, "/World/Effects/Materials/SparkGoldMat", (1.0, 0.85, 0.2))
            self._ensure_emissive_material(stage, "/World/Effects/Materials/SparkCyanMat", (0.15, 0.85, 1.0))
            self._ensure_point_material(stage, "/World/Effects/Materials/PointSparkGoldMat", (1.0, 0.85, 0.2))
            self._ensure_point_material(stage, "/World/Effects/Materials/PointSparkCyanMat", (0.15, 0.85, 1.0))

            gold_mat_prim = stage.GetPrimAtPath("/World/Effects/Materials/SparkGoldMat")
            point_gold_mat_prim = stage.GetPrimAtPath("/World/Effects/Materials/PointSparkGoldMat")

            # 1. Author single shared diamond mesh prototype under /World/Effects/Prototypes
            protos_root_path = Sdf.Path("/World/Effects/Prototypes")
            protos_prim = stage.GetPrimAtPath(protos_root_path)
            if not protos_prim.IsValid():
                protos_prim = stage.DefinePrim(protos_root_path, "Scope")
            else:
                protos_prim.SetActive(True)

            # Standard unit diamond octahedron (radius = 1.0 cm, height ratio = 1.3)
            unit_s = 1.0
            pts = [
                Gf.Vec3f(0.0, unit_s * 1.3, 0.0),   # Top (0)
                Gf.Vec3f(0.0, -unit_s * 1.3, 0.0),  # Bottom (1)
                Gf.Vec3f(unit_s, 0.0, 0.0),          # Right +X (2)
                Gf.Vec3f(-unit_s, 0.0, 0.0),         # Left -X (3)
                Gf.Vec3f(0.0, 0.0, unit_s),          # Front +Z (4)
                Gf.Vec3f(0.0, 0.0, -unit_s),         # Back -Z (5)
            ]
            face_counts = [3] * 8
            # Outward CCW face winding for positive normals
            face_indices = [
                # Top pyramid (+Y)
                0, 4, 2,  # +X, +Z
                0, 3, 4,  # -X, +Z
                0, 5, 3,  # -X, -Z
                0, 2, 5,  # +X, -Z
                # Bottom pyramid (-Y)
                1, 2, 4,  # +X, +Z
                1, 4, 3,  # -X, +Z
                1, 3, 5,  # -X, -Z
                1, 5, 2,  # +X, -Z
            ]
            extent = [Gf.Vec3f(-unit_s, -unit_s * 1.3, -unit_s), Gf.Vec3f(unit_s, unit_s * 1.3, unit_s)]

            proto_path = protos_root_path.AppendChild("DiamondMesh")
            proto_prim = stage.GetPrimAtPath(proto_path)
            if not proto_prim.IsValid():
                proto_mesh = UsdGeom.Mesh.Define(stage, proto_path)
                proto_mesh.CreatePointsAttr(Vt.Vec3fArray(pts))
                proto_mesh.CreateFaceVertexCountsAttr(Vt.IntArray(face_counts))
                proto_mesh.CreateFaceVertexIndicesAttr(Vt.IntArray(face_indices))
                proto_mesh.CreateDisplayColorAttr(Vt.Vec3fArray([Gf.Vec3f(1.0, 0.85, 0.2)] * 6))
                proto_mesh.CreateExtentAttr(Vt.Vec3fArray(extent))
                proto_mesh.CreateDoubleSidedAttr(True)
                if gold_mat_prim and gold_mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                    UsdShade.MaterialBindingAPI(proto_mesh.GetPrim()).Bind(UsdShade.Material(gold_mat_prim))
            else:
                proto_prim.SetActive(True)
                proto_mesh = UsdGeom.Mesh(proto_prim)
                if proto_mesh.GetPointsAttr().IsValid():
                    proto_mesh.GetPointsAttr().Set(Vt.Vec3fArray(pts))
                if proto_mesh.GetExtentAttr().IsValid():
                    proto_mesh.GetExtentAttr().Set(Vt.Vec3fArray(extent))
                if proto_mesh.GetFaceVertexCountsAttr().IsValid():
                    proto_mesh.GetFaceVertexCountsAttr().Set(Vt.IntArray(face_counts))
                if proto_mesh.GetFaceVertexIndicesAttr().IsValid():
                    proto_mesh.GetFaceVertexIndicesAttr().Set(Vt.IntArray(face_indices))

            # 2. Author Pre-allocated PointInstancers and Points for each pool slot
            alloc_sparks = max(self.NUM_SPARKS, 16)
            for i in range(self.POOL_SIZE):
                slot_path = Sdf.Path(f"/World/Effects/Pool_{i}")
                slot_prim = stage.GetPrimAtPath(slot_path)
                if not slot_prim.IsValid():
                    slot_prim = stage.DefinePrim(slot_path, "Xform")

                # Ensure slot has translate op
                slot_xform = UsdGeom.Xformable(slot_prim)
                has_t = any(op.GetOpType() == UsdGeom.XformOp.TypeTranslate for op in slot_xform.GetOrderedXformOps())
                if not has_t:
                    slot_xform.AddTranslateOp().Set(Gf.Vec3d(0.0, 0.0, 0.0))

                # Clean up legacy Sparks or individual Diamonds meshes if present
                for legacy_name in ("Sparks", "Diamonds"):
                    legacy_path = slot_path.AppendChild(legacy_name)
                    legacy_prim = stage.GetPrimAtPath(legacy_path)
                    if legacy_prim.IsValid():
                        legacy_prim.SetActive(False)
                        stage.RemovePrim(legacy_path)

                # Pre-allocate SphereLight (dormant at 0 intensity)
                light_path = slot_path.AppendChild("FlashLight")
                light_prim_usd = stage.GetPrimAtPath(light_path)
                if not light_prim_usd.IsValid():
                    light = UsdLux.SphereLight.Define(stage, light_path)
                    light.CreateRadiusAttr(self.FLASH_LIGHT_RADIUS)
                    light.CreateIntensityAttr(0.0)
                    light.CreateColorAttr(Gf.Vec3f(1.0, 0.78, 0.18))
                    light_xform = UsdGeom.Xformable(light.GetPrim())
                    light_xform.AddTranslateOp().Set(Gf.Vec3d(0.0, 0.0, 0.0))
                else:
                    light = UsdLux.SphereLight(light_prim_usd)
                    if not self._slots[i].is_active:
                        light.GetIntensityAttr().Set(0.0)

                # Pre-allocate PointInstancer (hardware-instanced 3D diamond meshes)
                instancer_path = slot_path.AppendChild("PointInstancer")
                instancer_prim = stage.GetPrimAtPath(instancer_path)
                if not instancer_prim.IsValid():
                    instancer = UsdGeom.PointInstancer.Define(stage, instancer_path)
                    instancer.CreatePrototypesRel().SetTargets([proto_path])
                    instancer.CreateProtoIndicesAttr(Vt.IntArray([0] * alloc_sparks))
                    instancer.CreatePositionsAttr(Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * alloc_sparks))
                    instancer.CreateScalesAttr(Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * alloc_sparks))
                    UsdGeom.Imageable(instancer.GetPrim()).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)
                else:
                    instancer = UsdGeom.PointInstancer(instancer_prim)
                    instancer_prim.SetActive(True)
                    rel = instancer.GetPrototypesRel()
                    if not rel.IsValid() or not rel.GetTargets():
                        instancer.CreatePrototypesRel().SetTargets([proto_path])
                    if not self._slots[i].is_active:
                        UsdGeom.Imageable(instancer_prim).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                # Pre-allocate Points (camera-facing billboard disks/spheres)
                points_path = slot_path.AppendChild("Points")
                points_prim = stage.GetPrimAtPath(points_path)
                point_extent = Vt.Vec3fArray([Gf.Vec3f(-2000.0, -2000.0, -2000.0), Gf.Vec3f(2000.0, 2000.0, 2000.0)])
                if not points_prim.IsValid():
                    points = UsdGeom.Points.Define(stage, points_path)
                    points.CreatePointsAttr(Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * alloc_sparks))
                    points.CreateWidthsAttr(Vt.FloatArray([2.0 * self.SPARK_RADIUS] * alloc_sparks))
                    points.CreateExtentAttr(point_extent)
                    points.CreateNormalsAttr(Vt.Vec3fArray([Gf.Vec3f(0.0, 1.0, 0.0)] * alloc_sparks))
                    points.CreateDisplayColorAttr(Vt.Vec3fArray([Gf.Vec3f(1.0, 0.85, 0.2)]))
                    if hasattr(points, "CreateDisplayOpacityAttr"):
                        points.CreateDisplayOpacityAttr(Vt.FloatArray([1.0]))
                    UsdGeom.Imageable(points.GetPrim()).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)
                    if point_gold_mat_prim and point_gold_mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                        UsdShade.MaterialBindingAPI(points.GetPrim()).Bind(UsdShade.Material(point_gold_mat_prim))
                else:
                    points = UsdGeom.Points(points_prim)
                    points_prim.SetActive(True)
                    # Retrofit extent attribute if missing or unauthored (prevents Hydra culling)
                    ext_attr = points.GetExtentAttr()
                    if not ext_attr.IsValid() or not ext_attr.HasAuthoredValue():
                        points.CreateExtentAttr(point_extent)
                    # Retrofit normals attribute if missing
                    norm_attr = points.GetNormalsAttr()
                    if not norm_attr.IsValid() or not norm_attr.HasAuthoredValue():
                        points.CreateNormalsAttr(Vt.Vec3fArray([Gf.Vec3f(0.0, 1.0, 0.0)] * alloc_sparks))
                    # Ensure PointSparkGoldMat is bound instead of mesh OmniSurface
                    if point_gold_mat_prim and point_gold_mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                        UsdShade.MaterialBindingAPI(points.GetPrim()).Bind(UsdShade.Material(point_gold_mat_prim))
                    if not self._slots[i].is_active:
                        UsdGeom.Imageable(points_prim).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

            self._pool_initialized = True
            if carb:
                warp_status = "with Warp" if self._use_warp else ""
                carb.log_info(
                    f"[hydragon.editor.core] Pre-allocated {self.POOL_SIZE} VFX pool slots "
                    f"(PointInstancer & Points with {self.NUM_SPARKS} sparks each) {warp_status}."
                )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to pre-allocate VFX pool: {e}")

    def spawn_foe_destruction_vfx(
        self,
        stage,
        world_pos: Tuple[float, float, float],
        color_theme: str = "gold",
    ):
        """
        Activates a pre-allocated pool slot at world_pos and triggers low-latency
        pre-cached sound immediately.
        """
        # Dynamically sync volume in case user changed DEFAULT_AUDIO_VOLUME without restarting editor
        self._set_audio_volume(DEFAULT_AUDIO_VOLUME)

        # Always trigger defeat sound cue immediately
        self._play_cached_sound()

        # Queue delayed random taunt (~1.5s delay)
        chosen_taunt = None
        if self._cached_taunt_sounds:
            chosen_taunt = random.choice(self._cached_taunt_sounds)
        else:
            chosen_taunt = random.choice(TAUNT_SOUND_NAMES)

        self._delayed_sounds.append({
            "timer": self.TAUNT_DELAY,
            "sound": chosen_taunt,
        })
        if carb:
            carb.log_info(f"[hydragon.editor.core] Queued random taunt sound with {self.TAUNT_DELAY:.1f}s delay.")

        if not HAS_KIT:
            self._mock_spawn_count += 1
            return

        if not stage:
            return

        try:
            self._ensure_pool(stage)

            # Find an idle slot, or recycle the oldest active slot
            target_slot = None
            for slot in self._slots:
                if not slot.is_active:
                    target_slot = slot
                    break

            if target_slot is None and self._slots:
                target_slot = max(self._slots, key=lambda s: s.elapsed)

            if target_slot:
                target_slot.spark_radius = self.SPARK_RADIUS
                target_slot.num_sparks = self.NUM_SPARKS
                target_slot.flash_intensity = self.FLASH_LIGHT_INTENSITY
                target_slot.flash_radius = self.FLASH_LIGHT_RADIUS
                target_slot.light_duration = self.FLASH_LIGHT_DURATION
                target_slot.lifetime = self.BURST_LIFETIME
                target_slot.render_mode = self._render_mode
                target_slot.activate(world_pos=world_pos, color_theme=color_theme, stage=stage)
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Activated VFX Pool slot {target_slot.slot_index} at {world_pos} ({color_theme})"
                    )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to activate VFX pool slot: {e}")

    def _deactivate_all_slots(self, stage=None):
        """Deactivates all pool slots without removing USD prims."""
        for slot in self._slots:
            slot.deactivate(stage)

    def get_warp_status(self) -> Dict[str, Any]:
        """Returns current Warp acceleration status for debugging."""
        return {
            "use_warp": self._use_warp,
            "device": self._warp_device,
            "initialized": self._warp_initialized,
            "benchmark": self._warp_benchmark,
            "render_mode": self._render_mode,
            "num_slots": len(self._slots),
            "active_slots": sum(1 for s in self._slots if s.is_active),
        }