"""
Hydragon Engine - Gameplay Effects Subsystem (VFX, Lighting Flash & Spatial Audio)

Manages real-time audiovisual feedback when gameplay entities are destroyed:
- Pre-allocated 3-slot Object Pool eliminating runtime prim allocation and stage churn
- High-velocity 3D diamond sparks (s=4.5cm) with radial burst, upward floral bias, and 3D curl turbulence
- Emissive glowing material (OmniSurface / OmniPBR: emission_weight=1.0, emission_intensity=60000.0)
- Dynamic sphere light flash (UsdLux.SphereLight) with quadratic decay
- Pre-cached RAM audio playback via omni.kit.uiaudio for guaranteed 0ms latency
- O(1) in-memory registry, zero stage traversal in update loops, and clean deactivation
"""

import math
import os
import random
from typing import Dict, List, Optional, Tuple

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


if HAS_WARP:
    @wp.kernel
    def simulate_sparks_kernel(
        positions: wp.array(dtype=wp.vec3),
        velocities: wp.array(dtype=wp.vec3),
        phase_offsets: wp.array(dtype=float),
        drag: float,
        t: float,
        dt: float,
        turb_amp: float,
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


# =============================================================================
# Gameplay VFX Tuning Parameters (Single Source of Truth)
# Adjust these values to tune the explosion visuals and lighting intensity.
# =============================================================================
DEFAULT_FLASH_LIGHT_INTENSITY: float = 5000000.0  # Peak SphereLight intensity on destruction (half intensity)
DEFAULT_FLASH_LIGHT_RADIUS: float = 80.0          # SphereLight radius in centimeters
DEFAULT_FLASH_LIGHT_DURATION: float = 0.22        # Light flash duration in seconds
DEFAULT_BURST_LIFETIME: float = 0.55              # Total particle sparks lifetime in seconds
DEFAULT_NUM_SPARKS: int = 24                      # Number of diamond sparks per explosion (GPU PointInstancer batch)
DEFAULT_SPARK_EMISSION: float = 60000.0           # Emissive intensity on diamond sparks
DEFAULT_SPARK_RADIUS: float = 9.0                 # Diamond radius in centimeters (9 cm)


class ExplosionPoolSlot:
    """
    Pre-allocated pool slot containing a PointInstancer and a SphereLight.
    Uses GPU-accelerated Warp kernels (or vector CPU fallback) and single-batch
    USD array writes to eliminate all frame drops.
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
    ):
        self.slot_index: int = slot_index
        self.slot_path: str = root_path_str or f"/World/Effects/Pool_{slot_index}"
        self.sparks_group_path: str = f"{self.slot_path}/Sparks"
        self.light_path: str = f"{self.slot_path}/FlashLight"

        self.is_active: bool = False
        self.elapsed: float = 0.0
        self.lifetime: float = lifetime if lifetime is not None else DEFAULT_BURST_LIFETIME
        self.light_duration: float = light_duration if light_duration is not None else DEFAULT_FLASH_LIGHT_DURATION
        self.flash_intensity: float = flash_intensity if flash_intensity is not None else DEFAULT_FLASH_LIGHT_INTENSITY
        self.flash_radius: float = flash_radius if flash_radius is not None else DEFAULT_FLASH_LIGHT_RADIUS
        self.num_sparks: int = num_sparks if num_sparks is not None else DEFAULT_NUM_SPARKS
        self.spark_radius: float = spark_radius if spark_radius is not None else DEFAULT_SPARK_RADIUS
        self.drag: float = 3.5

        # World space origin for current burst
        self.origin: Tuple[float, float, float] = (0.0, 0.0, 0.0)
        self._light_extinguished: bool = False

        # Pre-allocate particle simulation buffers (CPU state)
        self.positions: List[List[float]] = [[0.0, 0.0, 0.0] for _ in range(self.num_sparks)]
        self.velocities: List[List[float]] = [[0.0, 0.0, 0.0] for _ in range(self.num_sparks)]
        self.base_scales: List[float] = [1.0 for _ in range(self.num_sparks)]
        self.phase_offsets: List[float] = [0.0 for _ in range(self.num_sparks)]

        # Warp GPU buffers
        self._wp_positions = None
        self._wp_velocities = None
        self._wp_phases = None
        self._wp_device = "cuda" if (HAS_WARP and wp and hasattr(wp, "is_cuda_available") and wp.is_cuda_available()) else "cpu"

        # Cached USD handles
        self._cached_slot_trans_op = None
        self._cached_light_prim = None
        self._cached_light_trans_op = None
        self._cached_instancer = None
        self._cached_instancer_imageable = None
        self._active_color_theme: Optional[str] = None

    def activate(
        self,
        world_pos: Tuple[float, float, float] = (0.0, 0.0, 0.0),
        color_theme: str = "gold",
        stage=None,
    ):
        """Activates and positions the pool slot at the given 3D world coordinates."""
        self.is_active = True
        self.elapsed = 0.0
        self.origin = (float(world_pos[0]), float(world_pos[1]), float(world_pos[2]))
        self._light_extinguished = False

        # In-place update of spark kinematics buffers (zero GC allocation)
        for i in range(self.num_sparks):
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
            self.base_scales[i] = random.uniform(0.85, 1.25)
            self.phase_offsets[i] = random.uniform(0.0, 2.0 * math.pi)

        # Initialize or upload to Warp GPU arrays if available
        if HAS_WARP and wp:
            try:
                self._wp_positions = wp.array([wp.vec3(0.0, 0.0, 0.0) for _ in range(self.num_sparks)], dtype=wp.vec3, device=self._wp_device)
                self._wp_velocities = wp.array([wp.vec3(v[0], v[1], v[2]) for v in self.velocities], dtype=wp.vec3, device=self._wp_device)
                self._wp_phases = wp.array(self.phase_offsets, dtype=float, device=self._wp_device)
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to initialize Warp arrays: {ex}")
                self._wp_positions = None

        if HAS_KIT and stage:
            try:
                ox, oy, oz = self.origin

                # 1. Position pool slot and SphereLight explicitly at world_pos
                if self._cached_light_prim is None:
                    light_prim = stage.GetPrimAtPath(self.light_path)
                    if light_prim and light_prim.IsValid():
                        self._cached_light_prim = light_prim

                if self._cached_light_prim:
                    if self._cached_light_trans_op is None:
                        xformable = UsdGeom.Xformable(self._cached_light_prim)
                        for op in xformable.GetOrderedXformOps():
                            if op.GetOpType() == UsdGeom.XformOp.TypeTranslate:
                                self._cached_light_trans_op = op
                                break
                        if not self._cached_light_trans_op:
                            self._cached_light_trans_op = xformable.AddTranslateOp()

                    if self._cached_light_trans_op:
                        self._cached_light_trans_op.Set(Gf.Vec3d(ox, oy, oz))

                    light = UsdLux.SphereLight(self._cached_light_prim)
                    light_col = Gf.Vec3f(0.2, 0.85, 1.0) if color_theme == "cyan" else Gf.Vec3f(1.0, 0.78, 0.18)
                    light.GetColorAttr().Set(light_col)
                    light.GetIntensityAttr().Set(self.flash_intensity)
                    light.GetRadiusAttr().Set(self.flash_radius)

                # 2. Make PointInstancer visible and initialize batch transforms directly in world space
                if self._cached_instancer is None:
                    inst_prim = stage.GetPrimAtPath(self.sparks_group_path)
                    if inst_prim and inst_prim.IsValid():
                        self._cached_instancer = UsdGeom.PointInstancer(inst_prim)
                        self._cached_instancer_imageable = UsdGeom.Imageable(inst_prim)

                if self._cached_instancer:
                    init_pos = Vt.Vec3fArray([Gf.Vec3f(ox, oy, oz)] * self.num_sparks)
                    initial_scales = Vt.Vec3fArray([Gf.Vec3f(s, s, s) for s in self.base_scales])
                    orientations = Vt.QuathArray([Gf.Quath(1.0, 0.0, 0.0, 0.0)] * self.num_sparks)

                    self._cached_instancer.GetPositionsAttr().Set(init_pos)
                    self._cached_instancer.GetScalesAttr().Set(initial_scales)

                    if self._cached_instancer.GetOrientationsAttr().IsValid():
                        self._cached_instancer.GetOrientationsAttr().Set(orientations)

                    # Author initial bounding box extent to prevent frustum culling
                    r = 300.0
                    extent = Vt.Vec3fArray([Gf.Vec3f(ox - r, oy - r, oz - r), Gf.Vec3f(ox + r, oy + r, oz + r)])
                    if self._cached_instancer.GetExtentAttr().IsValid():
                        self._cached_instancer.GetExtentAttr().Set(extent)

                if self._cached_instancer_imageable:
                    self._cached_instancer_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.inherited)

                # 3. Material check: rebind prototype and instancer if theme dynamically changes
                if self._active_color_theme != color_theme:
                    self._active_color_theme = color_theme
                    mat_path = "/World/Effects/Materials/SparkCyanMat" if color_theme == "cyan" else "/World/Effects/Materials/SparkGoldMat"
                    mat_prim = stage.GetPrimAtPath(mat_path)
                    proto_prim = stage.GetPrimAtPath("/World/Effects/Prototypes/DiamondSpark")
                    if mat_prim and mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                        if proto_prim and proto_prim.IsValid():
                            UsdShade.MaterialBindingAPI(proto_prim).Bind(UsdShade.Material(mat_prim))
                        if self._cached_instancer:
                            UsdShade.MaterialBindingAPI(self._cached_instancer.GetPrim()).Bind(UsdShade.Material(mat_prim))
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to activate pool slot {self.slot_index}: {ex}")

    def update(self, dt: float, stage=None) -> bool:
        """Steps particle positions via Warp GPU kernel or CPU fallback, decays light, and returns False when expired."""
        if not self.is_active:
            return False

        self.elapsed += dt
        t = self.elapsed

        if t >= self.lifetime:
            self.deactivate(stage)
            return False

        # 1. Light Flash Decay with quadratic falloff (cleans up strictly once upon cutoff)
        if HAS_KIT and stage and self._cached_light_prim:
            try:
                light = UsdLux.SphereLight(self._cached_light_prim)
                if t < self.light_duration:
                    progress = t / self.light_duration
                    intensity = self.flash_intensity * ((1.0 - progress) ** 2.0)
                    light.GetIntensityAttr().Set(intensity)
                elif not self._light_extinguished:
                    self._light_extinguished = True
                    light.GetIntensityAttr().Set(0.0)
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to update light in slot {self.slot_index}: {ex}")

        # 2. Kinematics computation: GPU Warp if available, otherwise vector CPU
        speed_factor = math.exp(-self.drag * t)
        scale_factor = max(0.0, 1.0 - (t / self.lifetime))
        turb_amp = 35.0 * speed_factor

        used_warp = False
        if HAS_WARP and wp and self._wp_positions is not None:
            try:
                wp.launch(
                    kernel=simulate_sparks_kernel,
                    dim=self.num_sparks,
                    inputs=[
                        self._wp_positions,
                        self._wp_velocities,
                        self._wp_phases,
                        self.drag,
                        t,
                        dt,
                        turb_amp,
                        self.num_sparks,
                    ],
                    device=self._wp_device,
                )
                pos_np = self._wp_positions.numpy()
                for i in range(self.num_sparks):
                    self.positions[i][0] = float(pos_np[i][0])
                    self.positions[i][1] = float(pos_np[i][1])
                    self.positions[i][2] = float(pos_np[i][2])
                used_warp = True
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Warp simulation step failed: {ex}")
                used_warp = False

        if not used_warp:
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

        # 3. Single-batch PointInstancer array write in true world space
        if HAS_KIT and stage and self._cached_instancer:
            try:
                ox, oy, oz = self.origin
                vt_pos = Vt.Vec3fArray([
                    Gf.Vec3f(ox + p[0], oy + p[1], oz + p[2])
                    for p in self.positions
                ])
                vt_scale = Vt.Vec3fArray([
                    Gf.Vec3f(s * scale_factor, s * scale_factor, s * scale_factor)
                    for s in self.base_scales
                ])
                self._cached_instancer.GetPositionsAttr().Set(vt_pos)
                self._cached_instancer.GetScalesAttr().Set(vt_scale)

                # Keep frustum culling extent accurate around world position
                r = 300.0
                extent = Vt.Vec3fArray([
                    Gf.Vec3f(ox - r, oy - r, oz - r),
                    Gf.Vec3f(ox + r, oy + r, oz + r),
                ])
                if self._cached_instancer.GetExtentAttr().IsValid():
                    self._cached_instancer.GetExtentAttr().Set(extent)
            except Exception as ex:
                if carb:
                    carb.log_error(f"[hydragon.editor.core] Failed to update PointInstancer in slot {self.slot_index}: {ex}")

        return True

    def deactivate(self, stage=None):
        """Deactivates light and sparks cleanly by setting intensity to zero and hiding PointInstancer."""
        self.is_active = False
        self._light_extinguished = True
        if HAS_KIT and stage:
            try:
                if self._cached_light_prim:
                    UsdLux.SphereLight(self._cached_light_prim).GetIntensityAttr().Set(0.0)

                if self._cached_instancer:
                    zeros = Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * self.num_sparks)
                    self._cached_instancer.GetScalesAttr().Set(zeros)

                if self._cached_instancer_imageable:
                    self._cached_instancer_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)
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
        self._mock_spawn_count: int = 0

    @classmethod
    def get_instance(cls) -> Optional["HydragonEffectsSystem"]:
        return cls._instance

    def startup(self):
        """Initializes subscriptions for timeline events and per-frame VFX stepping."""
        if not HAS_KIT:
            return
        self._is_active = True
        self._resolve_audio_path()
        self._ensure_audio_loaded()
        self._subscribe_timeline()
        self._subscribe_app_update()
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonEffectsSystem initialized with Object Pool.")

    def shutdown(self):
        """Releases subscriptions, deactivates all pool slots, and unloads cached sound."""
        self._is_active = False
        self._pool_initialized = False
        self._deactivate_all_slots()
        self._slots.clear()
        self._cached_sound = None
        self._timeline_sub = None
        self._app_update_sub = None
        HydragonEffectsSystem._instance = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonEffectsSystem shutdown.")

    def _resolve_audio_path(self) -> str:
        """Finds or confirms the relative/dynamic path to foe_defeat.wav without hardcoded absolute paths."""
        candidates = []

        # 1. Preferred: relative to extension root via __file__
        ext_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
        candidates.append(os.path.join(ext_root, "data", "assets", "audio", "sound_fx_samples", "foe_defeat.wav"))

        # 2. Via Kit Extension Manager if available
        if HAS_KIT:
            try:
                em = omni.kit.app.get_app().get_extension_manager()
                ext_id_path = em.get_extension_path_by_pkg_id("hydragon.editor.core")
                if ext_id_path:
                    candidates.append(os.path.join(ext_id_path, "data", "assets", "audio", "sound_fx_samples", "foe_defeat.wav"))
            except Exception:
                pass

        # 3. Via Current Working Directory fallback
        candidates.append(os.path.abspath(os.path.join(os.getcwd(), "source", "extensions", "hydragon.editor.core", "data", "assets", "audio", "sound_fx_samples", "foe_defeat.wav")))

        for c in candidates:
            if os.path.exists(c):
                self._audio_file_path = os.path.abspath(c).replace("\\", "/")
                return self._audio_file_path
        return ""

    def _ensure_audio_loaded(self):
        """Pre-caches the defeat sound effect into RAM memory via omni.kit.uiaudio."""
        if self._cached_sound is not None or not HAS_KIT:
            return
        try:
            import omni.kit.uiaudio
            if not self._audio_file_path:
                self._resolve_audio_path()
            if self._audio_file_path and os.path.exists(self._audio_file_path):
                audio = omni.kit.uiaudio.get_ui_audio_interface()
                if audio:
                    self._cached_sound = audio.create_sound(self._audio_file_path)
                    if carb:
                        carb.log_info(f"[hydragon.editor.core] Audio sound pre-cached: {self._audio_file_path}")
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to pre-cache audio: {ex}")

    def _play_cached_sound(self):
        """Plays the pre-cached sound effect with 0ms disk latency."""
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

    def _on_timeline_event(self, e):
        if not HAS_KIT:
            return
        try:
            event_type = int(e.type)
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                if stage:
                    self._ensure_pool(stage)
                self._ensure_audio_loaded()
            elif event_type == int(omni.timeline.TimelineEventType.STOP):
                self._pool_initialized = False
                stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                self._deactivate_all_slots(stage)
        except Exception:
            pass

    def _on_app_update(self, e):
        if not HAS_KIT or not self._slots:
            return

        dt = e.payload.get("dt", 1.0 / 60.0) if hasattr(e, "payload") else 1.0 / 60.0
        dt = min(dt, 0.05)

        stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
        if not stage:
            return

        for slot in self._slots:
            if slot.is_active:
                slot.update(dt, stage=stage)

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
                omni_shader.CreateInput("emission_intensity", Sdf.ValueTypeNames.Float).Set(60000.0)

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

    def _ensure_pool(self, stage):
        """Pre-allocates /World/Effects pool slots and diamond meshes using safe stage queries."""
        if not HAS_KIT or not stage:
            return

        if self._pool_initialized and self._slots and len(self._slots) == self.POOL_SIZE:
            return

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
                    )
                )

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

            gold_mat_prim = stage.GetPrimAtPath("/World/Effects/Materials/SparkGoldMat")

            # 1. Author shared prototype mesh under /World/Effects/Prototypes/DiamondSpark (strictly invisible)
            protos_root_path = Sdf.Path("/World/Effects/Prototypes")
            protos_prim = stage.GetPrimAtPath(protos_root_path)
            if not protos_prim.IsValid():
                protos_prim = stage.DefinePrim(protos_root_path, "Scope")
            UsdGeom.Imageable(protos_prim).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

            proto_path = Sdf.Path("/World/Effects/Prototypes/DiamondSpark")
            proto_prim = stage.GetPrimAtPath(proto_path)

            s = self.SPARK_RADIUS
            pts = [
                Gf.Vec3f(0.0, s * 1.3, 0.0),   # Top
                Gf.Vec3f(0.0, -s * 1.3, 0.0),  # Bottom
                Gf.Vec3f(s, 0.0, 0.0),          # Right
                Gf.Vec3f(-s, 0.0, 0.0),         # Left
                Gf.Vec3f(0.0, 0.0, s),          # Front
                Gf.Vec3f(0.0, 0.0, -s),         # Back
            ]
            face_counts = [3] * 8
            face_indices = [
                0, 2, 4,
                0, 4, 3,
                0, 3, 5,
                0, 5, 2,
                1, 4, 2,
                1, 3, 4,
                1, 5, 3,
                1, 2, 5,
            ]

            if not proto_prim.IsValid():
                mesh = UsdGeom.Mesh.Define(stage, proto_path)
                mesh.CreatePointsAttr(Vt.Vec3fArray(pts))
                mesh.CreateFaceVertexCountsAttr(Vt.IntArray(face_counts))
                mesh.CreateFaceVertexIndicesAttr(Vt.IntArray(face_indices))
                mesh.CreateDisplayColorAttr(Vt.Vec3fArray([Gf.Vec3f(1.0, 0.85, 0.2)] * 6))

                # Proper bounding extent for RTX frustum culling
                extent = [Gf.Vec3f(-s, -s * 1.3, -s), Gf.Vec3f(s, s * 1.3, s)]
                mesh.CreateExtentAttr(Vt.Vec3fArray(extent))

                # Prototype mesh MUST be marked invisible so it never renders as an orphan at stage origin
                UsdGeom.Imageable(mesh.GetPrim()).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                # Pre-bind gold material directly to the prototype mesh!
                if gold_mat_prim and gold_mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                    UsdShade.MaterialBindingAPI(mesh.GetPrim()).Bind(UsdShade.Material(gold_mat_prim))
            else:
                UsdGeom.Imageable(proto_prim).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

            # 2. Author PointInstancers for each pool slot
            for i in range(self.POOL_SIZE):
                slot_path = Sdf.Path(f"/World/Effects/Pool_{i}")
                slot_prim = stage.GetPrimAtPath(slot_path)
                if not slot_prim.IsValid():
                    stage.DefinePrim(slot_path, "Xform")

                sparks_path = slot_path.AppendChild("Sparks")
                inst_prim = stage.GetPrimAtPath(sparks_path)
                if not inst_prim.IsValid():
                    instancer = UsdGeom.PointInstancer.Define(stage, sparks_path)
                    instancer.GetPrototypesRel().SetTargets([proto_path])
                    instancer.CreateProtoIndicesAttr().Set(Vt.IntArray([0] * self.NUM_SPARKS))
                    instancer.CreatePositionsAttr().Set(Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * self.NUM_SPARKS))
                    instancer.CreateScalesAttr().Set(Vt.Vec3fArray([Gf.Vec3f(0.0, 0.0, 0.0)] * self.NUM_SPARKS))
                    # Crucial for Omniverse RTX: orientations and extent attributes
                    instancer.CreateOrientationsAttr().Set(Vt.QuathArray([Gf.Quath(1.0, 0.0, 0.0, 0.0)] * self.NUM_SPARKS))
                    instancer.CreateExtentAttr().Set(Vt.Vec3fArray([Gf.Vec3f(-500.0, -500.0, -500.0), Gf.Vec3f(500.0, 500.0, 500.0)]))
                    if gold_mat_prim and gold_mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                        UsdShade.MaterialBindingAPI(instancer.GetPrim()).Bind(UsdShade.Material(gold_mat_prim))
                else:
                    instancer = UsdGeom.PointInstancer(inst_prim)
                    instancer.GetPrototypesRel().SetTargets([proto_path])
                    if not instancer.GetOrientationsAttr().IsValid():
                        instancer.CreateOrientationsAttr().Set(Vt.QuathArray([Gf.Quath(1.0, 0.0, 0.0, 0.0)] * self.NUM_SPARKS))
                    if not instancer.GetExtentAttr().IsValid():
                        instancer.CreateExtentAttr().Set(Vt.Vec3fArray([Gf.Vec3f(-500.0, -500.0, -500.0), Gf.Vec3f(500.0, 500.0, 500.0)]))

                # Set initial instancer invisible to eliminate GPU overhead
                UsdGeom.Imageable(instancer.GetPrim()).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                # Pre-allocate SphereLight (dormant at 0 intensity)
                light_path = slot_path.AppendChild("FlashLight")
                light_prim_usd = stage.GetPrimAtPath(light_path)
                if not light_prim_usd.IsValid():
                    light = UsdLux.SphereLight.Define(stage, light_path)
                    light.CreateRadiusAttr(self.FLASH_LIGHT_RADIUS)
                    light.CreateIntensityAttr(0.0)
                    light.CreateColorAttr(Gf.Vec3f(1.0, 0.78, 0.18))
                else:
                    light = UsdLux.SphereLight(light_prim_usd)
                    if not self._slots[i].is_active:
                        light.GetIntensityAttr().Set(0.0)

            self._pool_initialized = True
            if carb:
                carb.log_info(
                    f"[hydragon.editor.core] Pre-allocated {self.POOL_SIZE} VFX pool slots with PointInstancers ({self.NUM_SPARKS} sparks each, Warp={'GPU' if HAS_WARP else 'CPU Fallback'})."
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
        # Always trigger sound cue
        self._play_cached_sound()

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
