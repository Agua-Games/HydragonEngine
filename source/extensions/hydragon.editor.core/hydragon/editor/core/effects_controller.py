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


# =============================================================================
# Gameplay VFX Tuning Parameters (Single Source of Truth)
# Adjust these values to tune the explosion visuals and lighting intensity.
# =============================================================================
DEFAULT_FLASH_LIGHT_INTENSITY: float = 5000000.0  # Peak SphereLight intensity on destruction (half intensity)
DEFAULT_FLASH_LIGHT_RADIUS: float = 80.0          # SphereLight radius in centimeters
DEFAULT_FLASH_LIGHT_DURATION: float = 0.22        # Light flash duration in seconds
DEFAULT_BURST_LIFETIME: float = 0.55              # Total particle sparks lifetime in seconds
DEFAULT_NUM_SPARKS: int = 12                      # Number of diamond spark meshes per explosion (optimized for 60 FPS)
DEFAULT_SPARK_EMISSION: float = 60000.0           # Emissive intensity on diamond sparks
DEFAULT_SPARK_RADIUS: float = 9.0                 # Diamond radius in centimeters (9 cm)


class ExplosionPoolSlot:
    """
    Pre-allocated pool slot containing pre-allocated diamond sparks and a SphereLight.
    Eliminates all runtime DefinePrim and RemovePrim calls during gameplay.
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

        # Pre-allocate particle simulation buffers to eliminate GC churn on activation
        self.positions: List[List[float]] = [[0.0, 0.0, 0.0] for _ in range(self.num_sparks)]
        self.velocities: List[List[float]] = [[0.0, 0.0, 0.0] for _ in range(self.num_sparks)]
        self.base_scales: List[float] = [1.0 for _ in range(self.num_sparks)]
        self.phase_offsets: List[float] = [0.0 for _ in range(self.num_sparks)]

        # Cached USD handles to eliminate stage traversal in update loops
        self._cached_slot_trans_op = None
        self._cached_light_prim = None
        self._cached_spark_trans_ops: List = [None] * self.num_sparks
        self._cached_spark_scale_ops: List = [None] * self.num_sparks
        self._cached_spark_imageables: List = [None] * self.num_sparks
        self._cached_sparks_group_imageable = None
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

        if HAS_KIT and stage:
            try:
                # 1. Position pool slot Xform at world_pos using cached trans_op
                if self._cached_slot_trans_op is None:
                    slot_prim = stage.GetPrimAtPath(self.slot_path)
                    if slot_prim and slot_prim.IsValid():
                        xformable = UsdGeom.Xformable(slot_prim)
                        for op in xformable.GetOrderedXformOps():
                            if op.GetOpType() == UsdGeom.XformOp.TypeTranslate:
                                self._cached_slot_trans_op = op
                                break
                        if not self._cached_slot_trans_op:
                            self._cached_slot_trans_op = xformable.AddTranslateOp()

                if self._cached_slot_trans_op:
                    self._cached_slot_trans_op.Set(Gf.Vec3d(float(world_pos[0]), float(world_pos[1]), float(world_pos[2])))

                # 2. Configure and ignite the SphereLight flash (single-source tuning parameters)
                if self._cached_light_prim is None:
                    light_prim = stage.GetPrimAtPath(self.light_path)
                    if light_prim and light_prim.IsValid():
                        self._cached_light_prim = light_prim

                if self._cached_light_prim:
                    light = UsdLux.SphereLight(self._cached_light_prim)
                    light_col = Gf.Vec3f(0.2, 0.85, 1.0) if color_theme == "cyan" else Gf.Vec3f(1.0, 0.78, 0.18)
                    light.GetColorAttr().Set(light_col)
                    light.GetIntensityAttr().Set(self.flash_intensity)
                    light.GetRadiusAttr().Set(self.flash_radius)

                # 3. Make sparks group visible
                if self._cached_sparks_group_imageable is None:
                    group_prim = stage.GetPrimAtPath(self.sparks_group_path)
                    if group_prim and group_prim.IsValid():
                        self._cached_sparks_group_imageable = UsdGeom.Imageable(group_prim)
                if self._cached_sparks_group_imageable:
                    self._cached_sparks_group_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.inherited)

                # 4. Activate sparks: set initial position (0,0,0), scale, and visibility
                # Material binding is pre-bound during pool allocation to prevent runtime RTX pipeline stalls!
                # Only re-bind if theme changes dynamically.
                theme_changed = (self._active_color_theme != color_theme)
                mat_prim = None
                if theme_changed:
                    self._active_color_theme = color_theme
                    mat_path = "/World/Effects/Materials/SparkCyanMat" if color_theme == "cyan" else "/World/Effects/Materials/SparkGoldMat"
                    mat_prim = stage.GetPrimAtPath(mat_path)

                for j in range(self.num_sparks):
                    if self._cached_spark_trans_ops[j] is None:
                        spark_path = f"{self.sparks_group_path}/Spark_{j:02d}"
                        spark_prim = stage.GetPrimAtPath(spark_path)
                        if spark_prim and spark_prim.IsValid():
                            xform = UsdGeom.Xformable(spark_prim)
                            self._cached_spark_trans_ops[j] = self._get_or_add_xform_op(xform, UsdGeom.XformOp.TypeTranslate)
                            self._cached_spark_scale_ops[j] = self._get_or_add_xform_op(xform, UsdGeom.XformOp.TypeScale)
                            self._cached_spark_imageables[j] = UsdGeom.Imageable(spark_prim)

                    trans_op = self._cached_spark_trans_ops[j]
                    scale_op = self._cached_spark_scale_ops[j]
                    imageable = self._cached_spark_imageables[j]

                    if trans_op:
                        trans_op.Set(Gf.Vec3d(0.0, 0.0, 0.0))
                    if scale_op:
                        bs = self.base_scales[j]
                        scale_op.Set(Gf.Vec3f(bs, bs, bs))
                    if imageable:
                        imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.inherited)

                    if theme_changed and mat_prim and mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                        spark_path = f"{self.sparks_group_path}/Spark_{j:02d}"
                        spark_prim = stage.GetPrimAtPath(spark_path)
                        if spark_prim and spark_prim.IsValid():
                            UsdShade.MaterialBindingAPI(spark_prim).Bind(UsdShade.Material(mat_prim))
            except Exception as ex:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to activate pool slot {self.slot_index}: {ex}")

    def update(self, dt: float, stage=None) -> bool:
        """Steps particle positions, decays light flash, and returns False when expired."""
        if not self.is_active:
            return False

        self.elapsed += dt
        t = self.elapsed

        if t >= self.lifetime:
            self.deactivate(stage)
            return False

        # 1. Light Flash Decay with quadratic falloff
        if HAS_KIT and stage and self._cached_light_prim:
            try:
                light = UsdLux.SphereLight(self._cached_light_prim)
                if t < self.light_duration:
                    progress = t / self.light_duration
                    intensity = self.flash_intensity * ((1.0 - progress) ** 2.0)
                    light.GetIntensityAttr().Set(intensity)
                else:
                    light.GetIntensityAttr().Set(0.0)
            except Exception:
                pass

        # 2. Update Spark Kinematics (damping, 3D curl turbulence, and upward float)
        speed_factor = math.exp(-self.drag * t)
        scale_factor = max(0.0, 1.0 - (t / self.lifetime))
        turb_amp = 35.0 * speed_factor

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

            if HAS_KIT and stage:
                try:
                    trans_op = self._cached_spark_trans_ops[i]
                    if trans_op:
                        trans_op.Set(Gf.Vec3d(pos[0], pos[1], pos[2]))
                    scale_op = self._cached_spark_scale_ops[i]
                    if scale_op:
                        s = self.base_scales[i] * scale_factor
                        scale_op.Set(Gf.Vec3f(s, s, s))
                except Exception:
                    pass

        return True

    def deactivate(self, stage=None):
        """Deactivates light and sparks cleanly by setting intensity to zero and visibility to invisible."""
        self.is_active = False
        if HAS_KIT and stage:
            try:
                if self._cached_light_prim:
                    UsdLux.SphereLight(self._cached_light_prim).GetIntensityAttr().Set(0.0)

                # Set group invisible to completely skip GPU rendering overhead
                if self._cached_sparks_group_imageable:
                    self._cached_sparks_group_imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                for j in range(self.num_sparks):
                    scale_op = self._cached_spark_scale_ops[j]
                    if scale_op:
                        scale_op.Set(Gf.Vec3f(0.0, 0.0, 0.0))
                    imageable = self._cached_spark_imageables[j]
                    if imageable:
                        imageable.GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)
            except Exception:
                pass

    def _get_or_add_xform_op(self, xformable, op_type):
        """Finds existing XformOp of given type or creates a new one safely."""
        for op in xformable.GetOrderedXformOps():
            if op.GetOpType() == op_type:
                return op
        if op_type == UsdGeom.XformOp.TypeTranslate:
            return xformable.AddTranslateOp()
        elif op_type == UsdGeom.XformOp.TypeScale:
            return xformable.AddScaleOp()
        elif op_type == UsdGeom.XformOp.TypeRotateXYZ:
            return xformable.AddRotateXYZOp()
        return None


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

            # Diamond mesh definition: derived strictly from self.SPARK_RADIUS (9.0 cm)
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

            for i in range(self.POOL_SIZE):
                slot_path = Sdf.Path(f"/World/Effects/Pool_{i}")
                slot_prim = stage.GetPrimAtPath(slot_path)
                if not slot_prim.IsValid():
                    stage.DefinePrim(slot_path, "Xform")

                sparks_path = slot_path.AppendChild("Sparks")
                sparks_prim = stage.GetPrimAtPath(sparks_path)
                if not sparks_prim.IsValid():
                    sparks_prim = stage.DefinePrim(sparks_path, "Xform")

                # Set initial sparks group invisible to eliminate rasterization overhead
                UsdGeom.Imageable(sparks_prim).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                # Pre-allocate diamond meshes directly in pool slot
                for j in range(self.NUM_SPARKS):
                    spark_path = sparks_path.AppendChild(f"Spark_{j:02d}")
                    spark_prim = stage.GetPrimAtPath(spark_path)
                    if not spark_prim.IsValid():
                        mesh = UsdGeom.Mesh.Define(stage, spark_path)
                        mesh.CreatePointsAttr(Vt.Vec3fArray(pts))
                        mesh.CreateFaceVertexCountsAttr(Vt.IntArray(face_counts))
                        mesh.CreateFaceVertexIndicesAttr(Vt.IntArray(face_indices))
                        mesh.CreateDisplayColorAttr(Vt.Vec3fArray([Gf.Vec3f(1.0, 0.85, 0.2)] * 6))

                        xform = UsdGeom.Xformable(mesh.GetPrim())
                        xform.AddTranslateOp().Set(Gf.Vec3d(0.0, 0.0, 0.0))
                        xform.AddScaleOp().Set(Gf.Vec3f(0.0, 0.0, 0.0))  # Dormant at scale 0

                        # Initial invisible state to free GPU raster overhead
                        UsdGeom.Imageable(mesh.GetPrim()).GetVisibilityAttr().Set(UsdGeom.Tokens.invisible)

                        # Pre-bind material ONCE during pool setup!
                        if gold_mat_prim and gold_mat_prim.IsValid() and hasattr(UsdShade, "MaterialBindingAPI"):
                            UsdShade.MaterialBindingAPI(mesh.GetPrim()).Bind(UsdShade.Material(gold_mat_prim))

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
                carb.log_info(f"[hydragon.editor.core] Pre-allocated {self.POOL_SIZE} VFX pool slots with {self.NUM_SPARKS} diamond meshes each.")
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
