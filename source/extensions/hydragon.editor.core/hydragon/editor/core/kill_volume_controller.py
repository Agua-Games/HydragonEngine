"""
Hydragon Engine - Kill Volume Physics Hazard Simulation System

Implements component-driven hazard volumes (HydragonKillVolumeAPI) supporting:
- Declarative hazard boundaries (bottomless pits, lava, laser grids, arenas)
- Player hazard elimination, score penalties, and checkpoint respawning
- Instant adversary (foe) deactivation via OpenUSD ECS standards (prim.SetActive(False))
- Spatial sound effects and particle bursts on elimination
"""

import time
from typing import Any, Dict, List, Optional, Set, Tuple

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

from .schemas import HydragonKillVolume, HydragonGameManager


class HydragonKillZone:
    """
    Component class representing an individual hazard / kill volume entity.
    Encapsulates oriented spatial boundaries, faction filtering, and checkpoint targets.
    """

    def __init__(self, prim, world_pos: Tuple[float, float, float] = (0.0, 0.0, 0.0)):
        self._prim = prim
        self._prim_path: str = prim.GetPath().pathString if prim and hasattr(prim, "GetPath") else ""
        self._schema: HydragonKillVolume = HydragonKillVolume(prim)

        self._world_pos: Tuple[float, float, float] = world_pos
        self._world_transform: Optional[Any] = None
        self._inv_world_matrix: Optional[Any] = None
        self._half_extents: Tuple[float, float, float] = (500.0, 50.0, 500.0)
        self._radius: float = 500.0
        self._half_height: float = 50.0

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
    def filter_faction(self) -> str:
        if self._schema:
            return self._schema.filter_faction
        return "All"

    @property
    def volume_shape(self) -> str:
        if self._schema:
            return self._schema.volume_shape
        return "Box"

    @property
    def respawn_player(self) -> bool:
        if self._schema:
            return self._schema.respawn_player
        return True

    @property
    def penalty_score(self) -> int:
        if self._schema:
            return self._schema.penalty_score
        return 50

    @property
    def respawn_delay(self) -> float:
        if self._schema:
            return self._schema.respawn_delay
        return 0.5

    @property
    def destroy_foes_instantly(self) -> bool:
        if self._schema:
            return self._schema.destroy_foes_instantly
        return True

    @property
    def sound_asset_path(self) -> str:
        if self._schema:
            return self._schema.sound_asset_path
        return "data/assets/audio/sound_fx_samples/achievement_02.wav"

    @property
    def sound_enabled(self) -> bool:
        if self._schema:
            return self._schema.sound_enabled
        return True

    @property
    def spawn_effects(self) -> bool:
        if self._schema:
            return self._schema.spawn_effects
        return True

    @property
    def respawn_target(self) -> Optional[str]:
        if self._schema:
            return self._schema.respawn_target
        return None

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
        """Extracts world transform matrix and bounds extents from USD."""
        if not HAS_KIT or not self._prim or not hasattr(self._prim, "IsValid") or not self._prim.IsValid():
            return

        try:
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

            # Inspect child guide mesh (Cube / Sphere / Cylinder)
            bounds_prim = self._prim.GetPrimAtPath("volumes/kill_bounds")
            if bounds_prim and bounds_prim.IsValid():
                bxform = UsdGeom.Xformable(bounds_prim)
                b_xf = bxform.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
                cube_geom = UsdGeom.Cube(bounds_prim)
                size = float(cube_geom.GetSizeAttr().Get() or 100.0) if cube_geom.GetSizeAttr() else 100.0
                b_scale = (
                    float(b_xf.GetRow(0).GetLength()),
                    float(b_xf.GetRow(1).GetLength()),
                    float(b_xf.GetRow(2).GetLength()),
                )
                hx = (size * 0.5) * b_scale[0]
                hy = (size * 0.5) * b_scale[1]
                hz = (size * 0.5) * b_scale[2]
                self._half_extents = (hx, hy, hz)
                self._radius = max(hx, hz)
                self._half_height = hy
                b_trans = b_xf.ExtractTranslation()
                self._world_pos = (float(b_trans[0]), float(b_trans[1]), float(b_trans[2]))
                self._inv_world_matrix = b_xf.GetInverse()
            else:
                self._half_extents = (500.0 * scale_vec[0], 50.0 * scale_vec[1], 500.0 * scale_vec[2])
                self._radius = max(self._half_extents[0], self._half_extents[2])
                self._half_height = self._half_extents[1]

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to cache bounds for kill volume {self._prim_path}: {e}")

    def check_overlap(self, point: Tuple[float, float, float]) -> bool:
        """
        Evaluates whether point (world coordinates) is inside this hazard volume.
        Supports Box (OBB), Sphere, and Cylinder.
        """
        if not self.is_enabled:
            return False

        shape = self.volume_shape

        if shape == "Sphere":
            dx = point[0] - self._world_pos[0]
            dy = point[1] - self._world_pos[1]
            dz = point[2] - self._world_pos[2]
            dist_sq = dx * dx + dy * dy + dz * dz
            return dist_sq <= (self._radius * self._radius)

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

    def get_custom_respawn_world_pos(self, stage) -> Optional[Tuple[float, float, float]]:
        """Resolves target checkpoint prim translation from rel kill:respawnTarget."""
        target_path = self.respawn_target
        if not target_path or not stage or not HAS_KIT:
            return None

        try:
            target_prim = stage.GetPrimAtPath(target_path)
            if target_prim and target_prim.IsValid():
                xformable = UsdGeom.Xformable(target_prim)
                world_xf = xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
                t = world_xf.ExtractTranslation()
                return (float(t[0]), float(t[1]), float(t[2]))
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to resolve respawnTarget {target_path}: {e}")
        return None


class HydragonKillVolumeSystem:
    """
    Simulation system managing all active HydragonKillVolumeAPI hazard zones.
    Strictly follows ECS guidelines: discovers volumes once on PLAY and indexes in memory.
    """

    _instance: Optional["HydragonKillVolumeSystem"] = None

    def __init__(self):
        HydragonKillVolumeSystem._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False
        self._physics_step_sub = None
        self._timeline_sub = None

        self._active_zones: Dict[str, HydragonKillZone] = {}
        self._player_cooldown: float = 0.0

    @classmethod
    def get_instance(cls) -> Optional["HydragonKillVolumeSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

    @property
    def active_zone_count(self) -> int:
        return len(self._active_zones)

    def startup(self):
        """Initializes subscriptions to timeline and physics step."""
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
                carb.log_info("[hydragon.editor.core] HydragonKillVolumeSystem started.")
        except Exception as e:
            if carb:
                carb.log_error(f"[hydragon.editor.core] HydragonKillVolumeSystem startup failed: {e}")

    def shutdown(self):
        """Cleans up subscriptions and cached hazard registry."""
        self._is_active = False
        self._is_simulating = False
        self._active_zones.clear()
        self._timeline_sub = None
        self._physics_step_sub = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonKillVolumeSystem shut down.")

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
            self._player_cooldown = 0.0
            self._discover_zones_on_play()
        elif event_type in (
            int(omni.timeline.TimelineEventType.STOP),
            int(omni.timeline.TimelineEventType.PAUSE),
        ):
            self._is_simulating = False
            self._active_zones.clear()

    def _discover_zones_on_play(self):
        """Discovers and caches all prims with HydragonKillVolumeAPI applied."""
        self._active_zones.clear()
        if not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
        if not stage:
            return

        for prim in stage.Traverse():
            if not prim or not prim.IsValid():
                continue
            if HydragonKillVolume.is_applied(prim):
                zone = HydragonKillZone(prim)
                self._active_zones[zone.prim_path] = zone
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Registered Kill Volume: {zone.prim_path} (shape={zone.volume_shape}, penalty={zone.penalty_score})"
                    )

    def _on_physics_step(self, dt: float):
        if not self._is_simulating or not self._active_zones:
            return

        if self._player_cooldown > 0.0:
            self._player_cooldown = max(0.0, self._player_cooldown - dt)

        stage = omni.usd.get_context().get_stage() if HAS_KIT and omni.usd.get_context() else None

        # 1. Process Player Hazard Entry
        self._process_player_hazard(stage)

        # 2. Process Adversaries (Foes) Hazard Entry
        self._process_foes_hazard(stage)

    def _process_player_hazard(self, stage):
        """Evaluates whether player touched any kill volume and executes respawn / penalty."""
        if self._player_cooldown > 0.0:
            return

        try:
            from .player_controller import HydragonPlayerControllerSystem
            player_sys = HydragonPlayerControllerSystem.get_instance()
            if not player_sys or not player_sys.is_active_and_simulating():
                return

            player_pos = player_sys.get_player_world_pos()
            if not player_pos:
                return

            for zone in self._active_zones.values():
                if not zone.is_enabled:
                    continue
                if zone.filter_faction not in ("All", "Player"):
                    continue

                if zone.check_overlap(player_pos):
                    # Hazard activated!
                    self._player_cooldown = max(0.5, zone.respawn_delay)
                    if carb:
                        carb.log_info(f"[hydragon.editor.core] Player entered Kill Volume '{zone.prim_path}' at {player_pos}!")

                    # Deduct score penalty via GameManager
                    penalty = zone.penalty_score
                    if penalty > 0:
                        self._apply_score_penalty(stage, penalty)

                    # Trigger VFX spark burst
                    if zone.spawn_effects:
                        self._trigger_vfx(stage, player_pos)

                    # Play sound
                    if zone.sound_enabled:
                        self._trigger_sfx(zone.sound_asset_path)

                    # Resolve custom respawn target or fallback to default
                    respawn_pos = zone.get_custom_respawn_world_pos(stage) if stage else None

                    # Dispatch safe respawn
                    player_sys.request_respawn(custom_spawn_pos=respawn_pos)
                    break

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Player kill volume check failed: {e}")

    def _process_foes_hazard(self, stage):
        """Evaluates whether any dynamic adversary entered a kill volume and destroys it cleanly."""
        try:
            from .foes_controller import HydragonFoesControllerSystem
            foes_sys = HydragonFoesControllerSystem.get_instance()
            if not foes_sys or not foes_sys.is_active_and_simulating():
                return

            active_brains = getattr(foes_sys, "_active_brains", {})
            for foe_path, brain in list(active_brains.items()):
                if not brain or not brain.is_alive:
                    continue

                foe_pos = brain.current_pos
                for zone in self._active_zones.values():
                    if not zone.is_enabled:
                        continue
                    if zone.filter_faction not in ("All", "Enemy"):
                        continue

                    if zone.check_overlap(foe_pos):
                        if zone.destroy_foes_instantly:
                            if carb:
                                carb.log_info(f"[hydragon.editor.core] Foe '{foe_path}' destroyed by Kill Volume '{zone.prim_path}'.")

                            if zone.spawn_effects:
                                self._trigger_vfx(stage, foe_pos)

                            # Queue clean destruction on main thread outside simulation lock
                            foes_sys.queue_foe_destruction(foe_path, is_stomp=False)
                        break

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Foes kill volume check failed: {e}")

    def _apply_score_penalty(self, stage, penalty: int):
        """Applies score deduction to HydragonGameManager if present."""
        if not stage or not HAS_KIT:
            return

        try:
            for prim in stage.Traverse():
                if HydragonGameManager.is_applied(prim):
                    gm = HydragonGameManager(prim)
                    gm.score = max(0, gm.score - penalty)
                    if carb:
                        carb.log_info(f"[hydragon.editor.core] Applied hazard penalty -{penalty} score. New score: {gm.score}")
                    break
        except Exception:
            pass

        try:
            from .game_hud import HydragonGameHUD
            hud = HydragonGameHUD.get_instance()
            if hud:
                hud.show_score_popup(world_pos=(0.0, 0.0, 0.0), points=-penalty)
        except Exception:
            pass

    def _trigger_vfx(self, stage, pos: Tuple[float, float, float]):
        """Spawns particle spark burst at death position."""
        try:
            from .effects_controller import HydragonEffectsSystem
            eff = HydragonEffectsSystem.get_instance()
            if eff:
                eff.spawn_foe_destruction_vfx(stage=stage, world_pos=pos, color_theme="crimson")
        except Exception:
            pass

    def _trigger_sfx(self, sound_path: str):
        """Plays hazard sound effect."""
        try:
            from .soundtrack_controller import HydragonSoundtrackSystem
            sound_sys = HydragonSoundtrackSystem.get_instance()
            if sound_sys:
                sound_sys.play_sfx(sound_path)
        except Exception:
            pass
