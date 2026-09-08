"""
Hydragon Engine - Trigger and Level Goal Physics Controller System

Component-driven trigger volume system using OpenUSD API schemas
(HydragonTriggerAPI, HydragonGameAPI), PhysX physics simulation, and
agnostic entity tracking for victory and gameplay events.
"""

import math
import os
from typing import Any, Dict, List, Optional, Tuple

try:
    import carb
    import omni.kit.app
    import omni.kit.notification_manager as nm
    import omni.timeline
    import omni.ui as ui
    import omni.usd
    import omni.physx
    try:
        import omni.physx.bindings._physx as physx_bindings
    except ImportError:
        physx_bindings = None
    from omni.physx import get_physx_interface
    from pxr import Usd, UsdGeom, Sdf, Gf, PhysicsSchemaTools
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    nm = None
    ui = None
    PhysicsSchemaTools = None
    physx_bindings = None

from .schemas import HydragonTrigger, HydragonGameManager, HydragonPlayerController


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


class HydragonTriggerZone:
    """
    Component class representing an individual trigger volume entity.
    Encapsulates spatial bounds, faction filtering, and one-shot state.
    """

    def __init__(
        self,
        prim,
        world_pos: Tuple[float, float, float] = (0.0, 0.0, 0.0),
        radius: float = 75.0,
        half_height: float = 50.0,
    ):
        self._prim = prim
        self._prim_path: str = prim.GetPath().pathString if prim and hasattr(prim, "GetPath") else ""
        self._schema: Optional[HydragonTrigger] = HydragonTrigger(prim) if prim else None

        self._world_pos: Tuple[float, float, float] = world_pos
        self._radius: float = radius
        self._half_height: float = half_height
        self._triggered: bool = False
        self._sound_played: bool = False

    @property
    def prim_path(self) -> str:
        return self._prim_path

    @property
    def world_pos(self) -> Tuple[float, float, float]:
        return self._world_pos

    @world_pos.setter
    def world_pos(self, pos: Tuple[float, float, float]):
        self._world_pos = pos

    @property
    def radius(self) -> float:
        return self._radius

    @radius.setter
    def radius(self, val: float):
        self._radius = val

    @property
    def half_height(self) -> float:
        return self._half_height

    @property
    def is_triggered(self) -> bool:
        return self._triggered

    @property
    def is_enabled(self) -> bool:
        if self._triggered:
            return False
        if self._schema:
            return self._schema.is_enabled
        return True

    @property
    def is_one_shot(self) -> bool:
        if self._schema:
            return self._schema.is_one_shot
        return True

    @property
    def event_type(self) -> str:
        if self._schema:
            return self._schema.event_type
        return "OnLevelComplete"

    @property
    def filter_faction(self) -> str:
        if self._schema:
            return self._schema.filter_faction
        return "Player"

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
    def sound_play_once(self) -> bool:
        if self._schema:
            return self._schema.sound_play_once
        return True

    @property
    def sound_played(self) -> bool:
        return self._sound_played

    @sound_played.setter
    def sound_played(self, val: bool):
        self._sound_played = val

    def check_overlap(self, pos: Tuple[float, float, float]) -> bool:
        """
        Evaluates whether a point is within the trigger volume cylindrical envelope.
        Includes a 50cm margin for the player sphere's physical radius so touching
        the boundary of the trigger cylinder cleanly activates it.
        """
        if not self.is_enabled:
            return False

        dx = pos[0] - self._world_pos[0]
        dy = pos[1] - self._world_pos[1]
        dz = pos[2] - self._world_pos[2]

        horiz_dist = math.sqrt(dx * dx + dz * dz)
        margin = 50.0
        if horiz_dist <= (self._radius + margin) and abs(dy) <= (self._half_height + margin):
            return True
        return False

    def on_trigger_entered(self):
        """Marks the trigger as activated in memory for this simulation session."""
        self._triggered = True


class HydragonTriggerSystem:
    """
    Manages trigger zones, win-condition evaluation, and game session state.
    Strictly follows ECS guidelines: discovers triggers once on PLAY and indexes in memory.
    """

    _instance: Optional["HydragonTriggerSystem"] = None

    def __init__(self):
        HydragonTriggerSystem._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False
        self._physics_step_sub = None
        self._timeline_sub = None
        self._app_update_sub = None

        self._active_triggers: Dict[str, HydragonTriggerZone] = {}
        self._cached_game_manager_path: Optional[str] = None
        self._level_completed: bool = False
        self._trigger_report_sub_id = None
        self._victory_window = None
        self._cached_sounds: Dict[str, Any] = {}
        self._mock_sound_play_count: int = 0

    @classmethod
    def get_instance(cls) -> Optional["HydragonTriggerSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

    @property
    def is_level_completed(self) -> bool:
        return self._level_completed

    def startup(self):
        """Initializes subscriptions to timeline, physics step, and app updates."""
        if not HAS_KIT:
            return

        self._subscribe_timeline()
        self._subscribe_app_update()
        self._subscribe_physics()
        self._is_active = True

        if carb:
            carb.log_info("[hydragon.editor.core] HydragonTriggerSystem started.")

    def shutdown(self):
        """Releases subscriptions and clears cached trigger data."""
        self._is_active = False
        self._is_simulating = False
        self._active_triggers.clear()
        self._cached_game_manager_path = None
        self._level_completed = False
        self._cached_sounds.clear()

        if hasattr(self, "_victory_window") and self._victory_window:
            try:
                self._victory_window.visible = False
            except Exception:
                pass
            self._victory_window = None

        if hasattr(self, "_trigger_report_sub_id") and self._trigger_report_sub_id is not None:
            try:
                from omni.physx import get_physx_simulation_interface
                sim_iface = get_physx_simulation_interface()
                if sim_iface and hasattr(sim_iface, "unsubscribe_physics_trigger_report_events"):
                    sim_iface.unsubscribe_physics_trigger_report_events(self._trigger_report_sub_id)
            except Exception:
                pass
            self._trigger_report_sub_id = None

        if not HAS_KIT:
            HydragonTriggerSystem._instance = None
            return

        if self._physics_step_sub:
            try:
                from omni.physx import get_physx_interface
                physx_iface = get_physx_interface()
                if physx_iface and hasattr(physx_iface, "unsubscribe_physics_step_events"):
                    physx_iface.unsubscribe_physics_step_events(self._physics_step_sub)
            except Exception:
                pass
            self._physics_step_sub = None
        if self._app_update_sub:
            self._app_update_sub = None
        if self._timeline_sub:
            self._timeline_sub = None

        HydragonTriggerSystem._instance = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonTriggerSystem shutdown.")

    def _resolve_sound_path(self, asset_path: str) -> str:
        """Resolves relative audio asset path dynamically without absolute paths."""
        if not asset_path:
            return ""
        clean_path = str(asset_path).strip("@").replace("\\", "/")
        if os.path.isabs(clean_path) and os.path.exists(clean_path):
            return clean_path

        candidates = []

        # 1. Preferred: Via Kit Extension Manager if available
        if HAS_KIT:
            try:
                em = omni.kit.app.get_app().get_extension_manager()
                ext_id_path = em.get_extension_path_by_pkg_id("hydragon.editor.core")
                if ext_id_path:
                    candidates.append(os.path.join(ext_id_path, clean_path))
                    candidates.append(os.path.join(ext_id_path, "data", "assets", "audio", "sound_fx_samples", os.path.basename(clean_path)))
            except Exception:
                pass

        # 2. Relative to extension root via __file__ (development fallback)
        ext_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
        candidates.append(os.path.join(ext_root, clean_path))
        candidates.append(os.path.join(ext_root, "data", "assets", "audio", "sound_fx_samples", os.path.basename(clean_path)))

        candidates.append(os.path.abspath(os.path.join(os.getcwd(), "source", "extensions", "hydragon.editor.core", clean_path)))
        candidates.append(os.path.abspath(os.path.join(os.getcwd(), "source", "extensions", "hydragon.editor.core", "data", "assets", "audio", "sound_fx_samples", os.path.basename(clean_path))))

        for c in candidates:
            if os.path.exists(c):
                return os.path.abspath(c).replace("\\", "/")
        return ""

    def _play_trigger_sound(self, zone: HydragonTriggerZone):
        """Plays trigger sound effect according to sound_enabled and sound_play_once."""
        if not zone.sound_enabled:
            return
        if zone.sound_play_once and zone.sound_played:
            return

        zone.sound_played = True

        if not HAS_KIT:
            self._mock_sound_play_count += 1
            return

        try:
            resolved = self._resolve_sound_path(zone.sound_asset_path)
            if not resolved or not os.path.exists(resolved):
                return

            import omni.kit.uiaudio
            audio = omni.kit.uiaudio.get_ui_audio_interface()
            if not audio:
                return

            if resolved not in self._cached_sounds:
                snd = audio.create_sound(resolved)
                if snd:
                    self._cached_sounds[resolved] = snd

            sound = self._cached_sounds.get(resolved)
            if sound:
                audio.play_sound(sound)
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to play trigger sound: {ex}")

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
                carb.log_warn(f"[hydragon.editor.core] Trigger system timeline subscription failed: {e}")

    def _subscribe_app_update(self):
        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                self._on_app_update
            )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Trigger system app update subscription failed: {e}")

    def _subscribe_physics(self) -> bool:
        if self._physics_step_sub is not None:
            return True
        try:
            physx_iface = get_physx_interface()
            if physx_iface:
                self._physics_step_sub = physx_iface.subscribe_physics_step_events(self._on_physics_step)
                if carb:
                    carb.log_info("[hydragon.editor.core] Trigger system subscribed to PhysX steps.")
        except Exception:
            pass

        try:
            from omni.physx import get_physx_simulation_interface
            sim_iface = get_physx_simulation_interface()
            if sim_iface and hasattr(sim_iface, "subscribe_physics_trigger_report_events"):
                self._trigger_report_sub_id = sim_iface.subscribe_physics_trigger_report_events(
                    self._on_physx_trigger_report
                )
                if carb:
                    carb.log_info("[hydragon.editor.core] Trigger system subscribed to PhysX trigger reports.")
        except Exception:
            pass

        return self._physics_step_sub is not None

    def _decode_prim_path(self, val) -> str:
        """Agnostically resolves a collider/actor identifier (int, SdfPath, or string) to a USD path string."""
        if not val:
            return ""
        if isinstance(val, str):
            return val
        if isinstance(val, int):
            try:
                if PhysicsSchemaTools and hasattr(PhysicsSchemaTools, "intToSdfPath"):
                    return str(PhysicsSchemaTools.intToSdfPath(val))
            except Exception:
                return ""
        if hasattr(val, "pathString"):
            return val.pathString
        return str(val)

    def _on_physx_trigger_report(self, data):
        """Direct PhysX callback when a collider enters a PhysxTriggerAPI volume."""
        if not self._is_simulating or self._level_completed or not self._active_triggers:
            return
        try:
            # 1. Inspect event_type: only trigger on enter, strictly ignore leave
            event_type = getattr(data, "event_type", None)
            if event_type is not None:
                event_str = str(event_type).upper()
                if "LEAVE" in event_str:
                    return
                if physx_bindings and hasattr(physx_bindings, "TriggerEventType"):
                    if event_type == physx_bindings.TriggerEventType.TRIGGER_ON_LEAVE:
                        return

            stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
            if not stage:
                return

            # 2. Decode trigger collider and other collider paths
            trigger_val = (
                getattr(data, "trigger_collider", None)
                or getattr(data, "trigger_collider_prim_id", None)
                or getattr(data, "trigger_prim_path", None)
            )
            other_val = (
                getattr(data, "other_collider", None)
                or getattr(data, "other_collider_prim_id", None)
                or getattr(data, "other_prim_path", None)
            )

            trigger_path = self._decode_prim_path(trigger_val)
            other_path = self._decode_prim_path(other_val)

            # 3. If other_path is known, strictly verify it matches the player
            player_rb = self._resolve_player_rb_path(stage)
            player_root = None
            try:
                from .player_controller import HydragonPlayerControllerSystem
                p_sys = HydragonPlayerControllerSystem.get_instance()
                if p_sys:
                    player_root = p_sys.get_player_root_path()
            except Exception:
                pass

            if other_path:
                is_player = False
                if player_rb and (other_path == player_rb or other_path.startswith(player_rb + "/")):
                    is_player = True
                elif player_root and (other_path == player_root or other_path.startswith(player_root + "/")):
                    is_player = True
                if not is_player:
                    # Non-player entity (e.g. foe or debris) touched the trigger; ignore
                    return

            # 4. If trigger_path is known, verify it matches one of our registered active triggers
            if trigger_path:
                matched_zone = None
                for t_path, zone in self._active_triggers.items():
                    if trigger_path == t_path or trigger_path.startswith(t_path + "/") or t_path.startswith(trigger_path + "/"):
                        matched_zone = zone
                        break
                if not matched_zone:
                    return
                if not matched_zone.is_enabled:
                    return
                matched_zone.on_trigger_entered()
                self._play_trigger_sound(matched_zone)
                if matched_zone.event_type == "OnLevelComplete":
                    self._handle_level_complete(stage)
                return

            # If paths could not be decoded from data, do not guess; rely on deterministic spatial overlap in _on_physics_step
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] PhysX trigger report error: {ex}")

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
                self._level_completed = False
                if hasattr(self, "_victory_window") and self._victory_window:
                    try:
                        self._victory_window.visible = False
                    except Exception:
                        pass
                    self._victory_window = None
                self._subscribe_physics()
                stage = omni.usd.get_context().get_stage()
                if stage:
                    self._discover_entities_once(stage)
                    try:
                        from .player_controller import HydragonPlayerControllerSystem
                        p_sys = HydragonPlayerControllerSystem.get_instance()
                        if p_sys:
                            p_prim = p_sys.find_player_prim(stage)
                            if p_prim:
                                HydragonPlayerController(p_prim).input_enabled = True
                    except Exception:
                        pass
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Play mode started. Registered {len(self._active_triggers)} triggers."
                    )
            elif event_type in (int(omni.timeline.TimelineEventType.STOP), int(omni.timeline.TimelineEventType.PAUSE)):
                self._is_simulating = False
                # Reset sound_played state on all triggers
                for zone in self._active_triggers.values():
                    zone.sound_played = False

                stage = omni.usd.get_context().get_stage() if omni.usd.get_context() else None
                if stage:
                    # Restore any trigger prims that had is_enabled = False authored
                    for prim_path in list(self._active_triggers.keys()):
                        try:
                            prim = stage.GetPrimAtPath(prim_path)
                            if prim and prim.IsValid() and HydragonTrigger.is_applied(prim):
                                HydragonTrigger(prim).is_enabled = True
                        except Exception:
                            pass
                    # Reset Game Manager on STOP
                    if self._cached_game_manager_path:
                        try:
                            gm_prim = stage.GetPrimAtPath(self._cached_game_manager_path)
                            if gm_prim and gm_prim.IsValid() and HydragonGameManager.is_applied(gm_prim):
                                gm = HydragonGameManager(gm_prim)
                                gm.state = "Playing"
                                gm.score = 0
                                gm.foes_destroyed = 0
                                gm.elapsed_time = 0.0
                        except Exception:
                            pass
                    # Restore player input
                    try:
                        from .player_controller import HydragonPlayerControllerSystem
                        p_sys = HydragonPlayerControllerSystem.get_instance()
                        if p_sys:
                            p_prim = p_sys.find_player_prim(stage)
                            if p_prim:
                                HydragonPlayerController(p_prim).input_enabled = True
                    except Exception:
                        pass
                self._active_triggers.clear()
                self._cached_game_manager_path = None
                self._level_completed = False
                if hasattr(self, "_victory_window") and self._victory_window:
                    try:
                        self._victory_window.visible = False
                    except Exception:
                        pass
                    self._victory_window = None
                if carb:
                    carb.log_info("[hydragon.editor.core] Play mode stopped. Trigger registry cleared.")
        except Exception as ex:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Trigger system timeline event error: {ex}")

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
    def _resolve_trigger_geometry_prim(self, trigger_prim) -> Optional[object]:
        """Finds the actual geometry detector prim (e.g. cylinder or mesh) under the trigger root."""
        if not HAS_KIT or not trigger_prim or not trigger_prim.IsValid():
            return None
        # Check standard subpaths (e.g. hydragon_goal_hole asset)
        for subpath in ("trigger_volume/detector", "geometry/detector", "detector"):
            try:
                det = trigger_prim.GetPrimAtPath(subpath)
                if det and det.IsValid():
                    return det
            except Exception:
                pass
        # Search all children for PhysxTriggerAPI, Cylinder, or CollisionAPI
        for child in trigger_prim.GetAllChildren():
            if child.IsValid():
                if child.HasAPI("PhysxTriggerAPI") or child.HasAPI("PhysicsCollisionAPI"):
                    return child
                if UsdGeom and hasattr(UsdGeom, "Cylinder") and UsdGeom.Cylinder(child):
                    return child
        return None

    def _discover_entities_once(self, stage):
        """
        Discovers all trigger prims and game managers once on simulation start.
        Avoids per-frame traversals entirely.
        """
        self._active_triggers.clear()
        self._cached_game_manager_path = None
        self._level_completed = False

        if not stage:
            return

        for prim in stage.Traverse():
            if not prim.IsValid() or not prim.IsActive():
                continue

            # Check for Trigger schema or native trigger / goal prims
            is_trigger = HydragonTrigger.is_applied(prim)
            if not is_trigger:
                prim_name_lower = prim.GetName().lower()
                if (prim.HasAPI("PhysxTriggerAPI") or "goal" in prim_name_lower) and not (
                    HydragonPlayerController.is_applied(prim) or prim.HasAPI("HydragonChaserAIAPI")
                ):
                    is_trigger = True

            if is_trigger:
                prim_path = prim.GetPath().pathString
                if any(prim_path.startswith(p + "/") for p in self._active_triggers):
                    continue
                # Ensure trigger is enabled on USD prim if previously disabled
                if HydragonTrigger.is_applied(prim):
                    try:
                        trig_schema = HydragonTrigger(prim)
                        trig_schema.is_enabled = True
                    except Exception:
                        pass
                geo_prim = self._resolve_trigger_geometry_prim(prim)
                eval_prim = geo_prim if geo_prim else prim
                world_pos = self._compute_prim_world_pos(eval_prim)
                radius, half_height = self._extract_trigger_geometry_bounds(prim)

                zone = HydragonTriggerZone(
                    prim=prim,
                    world_pos=world_pos,
                    radius=radius,
                    half_height=half_height,
                )
                self._active_triggers[prim_path] = zone

            # Discover Game Manager
            if self._cached_game_manager_path is None:
                if HydragonGameManager.is_applied(prim):
                    self._cached_game_manager_path = prim.GetPath().pathString
                    try:
                        gm = HydragonGameManager(prim)
                        gm.state = "Playing"
                        gm.score = 0
                        gm.foes_destroyed = 0
                        gm.elapsed_time = 0.0
                    except Exception:
                        pass

    def _compute_prim_world_pos(self, prim) -> Tuple[float, float, float]:
        """Calculates world translation of a USD prim."""
        if not HAS_KIT or not prim or not prim.IsValid():
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

    def _extract_trigger_geometry_bounds(self, trigger_prim) -> Tuple[float, float]:
        """
        Extracts radius and half-height from trigger volume geometry descendants
        or provides robust defaults for centimeter-scale scene.
        """
        default_radius = 75.0
        default_half_height = 50.0

        if not HAS_KIT or not trigger_prim or not trigger_prim.IsValid():
            return default_radius, default_half_height

        try:
            # Check cylinder detector child (e.g. goal_hole asset)
            for child in trigger_prim.GetAllChildren():
                if UsdGeom and hasattr(UsdGeom, "Cylinder"):
                    cyl = UsdGeom.Cylinder(child)
                    if cyl:
                        rad_attr = cyl.GetRadiusAttr()
                        ht_attr = cyl.GetHeightAttr()
                        r = float(rad_attr.Get()) if rad_attr and rad_attr.IsValid() else default_radius
                        h = float(ht_attr.Get()) if ht_attr and ht_attr.IsValid() else (default_half_height * 2.0)
                        return r, h * 0.5

                for grandchild in child.GetAllChildren():
                    if UsdGeom and hasattr(UsdGeom, "Cylinder"):
                        cyl = UsdGeom.Cylinder(grandchild)
                        if cyl:
                            rad_attr = cyl.GetRadiusAttr()
                            ht_attr = cyl.GetHeightAttr()
                            r = float(rad_attr.Get()) if rad_attr and rad_attr.IsValid() else default_radius
                            h = float(ht_attr.Get()) if ht_attr and ht_attr.IsValid() else (default_half_height * 2.0)
                            return r, h * 0.5
        except Exception:
            pass

        return default_radius, default_half_height

    # -------------------------------------------------------------------------
    # Physics Update Step (O(N) iteration - ZERO per-frame traversals)
    # -------------------------------------------------------------------------
    def _on_physics_step(self, dt: float):
        if not self._is_simulating or not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage()
        if not stage:
            return

        # 1. Update Game Manager session time
        if self._cached_game_manager_path and not self._level_completed:
            gm_prim = stage.GetPrimAtPath(self._cached_game_manager_path)
            if gm_prim and gm_prim.IsValid() and HydragonGameManager.is_applied(gm_prim):
                gm = HydragonGameManager(gm_prim)
                gm.elapsed_time += dt

        if not self._active_triggers:
            return

        # 2. Get Player real-time simulated world position
        player_pos: Optional[Tuple[float, float, float]] = None
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys:
                player_pos = p_sys.get_player_world_pos()
        except Exception:
            pass

        if player_pos is None or (player_pos[0] == 0.0 and player_pos[1] == 0.0 and player_pos[2] == 0.0):
            # Check player rigid body directly if player_sys pos is default
            player_rb = self._resolve_player_rb_path(stage)
            if player_rb:
                player_prim = stage.GetPrimAtPath(player_rb)
                player_pos = self._get_prim_realtime_pos(player_prim, player_rb)

        if player_pos is None:
            return

        # 3. Check triggers overlap
        triggered_paths: List[str] = []

        for prim_path, zone in list(self._active_triggers.items()):
            if not zone.is_enabled:
                continue

            if zone.check_overlap(player_pos):
                zone.on_trigger_entered()
                triggered_paths.append(prim_path)

                self._play_trigger_sound(zone)

                if zone.event_type == "OnLevelComplete":
                    self._handle_level_complete(stage)

        # Remove deactivated one-shot triggers
        for path in triggered_paths:
            zone = self._active_triggers.get(path)
            if zone and not zone.is_enabled:
                self._active_triggers.pop(path, None)

    def _handle_level_complete(self, stage):
        """Triggers victory state on Game Manager, awards score, disables player input, and presents victory UI."""
        if self._level_completed:
            return

        self._level_completed = True

        score = 0
        elapsed = 0.0
        foes = 0

        # 1. Update Game Manager
        if self._cached_game_manager_path and stage:
            try:
                gm_prim = stage.GetPrimAtPath(self._cached_game_manager_path)
                if gm_prim and gm_prim.IsValid() and HydragonGameManager.is_applied(gm_prim):
                    gm = HydragonGameManager(gm_prim)
                    gm.trigger_victory()
                    gm.score += 500  # Bonus for completing level
                    score = gm.score
                    elapsed = gm.elapsed_time
                    foes = gm.foes_destroyed
            except Exception as e:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Error updating game manager victory: {e}")

        # 2. Disable player controller input and halt marble velocities
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys and stage:
                player_prim = p_sys.find_player_prim(stage)
                if player_prim:
                    ctl = HydragonPlayerController(player_prim)
                    ctl.input_enabled = False
                    rb_prim = p_sys.get_rigid_body_prim(player_prim)
                    if rb_prim and rb_prim.IsValid() and hasattr(rb_prim, "GetAttribute"):
                        v_attr = rb_prim.GetAttribute("physics:velocity")
                        if v_attr and v_attr.IsValid():
                            v_attr.Set(Gf.Vec3f(0.0, 0.0, 0.0))
                        w_attr = rb_prim.GetAttribute("physics:angularVelocity")
                        if w_attr and w_attr.IsValid():
                            w_attr.Set(Gf.Vec3f(0.0, 0.0, 0.0))
        except Exception:
            pass

        # 3. Post notification in Omniverse Kit Notification Manager
        try:
            if HAS_KIT and nm and hasattr(nm, "post_notification"):
                msg = f"🏆 MISSION ACCOMPLISHED!\nFinal Score: {score} | Foes Defeated: {foes} | Time: {elapsed:.1f}s"
                nm.post_notification(msg, duration=15, status=nm.NotificationStatus.INFO)
        except Exception:
            pass

        # 4. Display on-screen victory HUD overlay
        try:
            from .game_hud import HydragonGameHUD
            hud = HydragonGameHUD.get_instance()
            if hud:
                hud.show_victory(score=score, elapsed=elapsed, foes=foes)
            else:
                self._show_victory_ui_overlay(score, elapsed, foes)
        except Exception:
            self._show_victory_ui_overlay(score, elapsed, foes)

        # 5. Transition soundtrack to victory track if available
        try:
            from .soundtrack_controller import HydragonSoundtrackSystem
            st_sys = HydragonSoundtrackSystem.get_instance()
            if st_sys:
                st_sys.play_track("victory", fade=True)
        except Exception:
            pass

        if carb:
            carb.log_info("================================================================")
            carb.log_info(f"*** HYDRAGON VICTORY! Level completed in {elapsed:.2f}s! Final Score: {score} (Foes: {foes}) ***")
            carb.log_info("================================================================")

    def _show_victory_ui_overlay(self, score: int, elapsed: float, foes: int):
        """Displays a clean, centered victory HUD overlay in Kit."""
        if not HAS_KIT or ui is None:
            return
        try:
            if hasattr(self, "_victory_window") and self._victory_window:
                try:
                    self._victory_window.visible = False
                except Exception:
                    pass
                self._victory_window = None

            window = ui.Window(
                "HydragonVictoryOverlay",
                width=420,
                height=160,
                flags=ui.WINDOW_FLAGS_NO_TITLE_BAR
                | ui.WINDOW_FLAGS_NO_RESIZE
                | ui.WINDOW_FLAGS_NO_MOVE
                | ui.WINDOW_FLAGS_NO_SCROLLBAR
                | ui.WINDOW_FLAGS_NO_COLLAPSE,
            )
            with window.frame:
                with ui.VStack(spacing=6):
                    ui.Spacer(height=6)
                    ui.Label(
                        "MISSION ACCOMPLISHED!",
                        style={"color": 0xFF20D040, "font_size": 26, "alignment": ui.Alignment.CENTER},
                    )
                    ui.Label(
                        f"FINAL SCORE: {score}",
                        style={"color": 0xFFFFFFFF, "font_size": 20, "alignment": ui.Alignment.CENTER},
                    )
                    ui.Label(
                        f"Time: {elapsed:.1f}s   |   Foes Defeated: {foes}",
                        style={"color": 0xFFDDDDDD, "font_size": 15, "alignment": ui.Alignment.CENTER},
                    )
                    ui.Spacer(height=6)

            self._victory_window = window
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to build victory UI overlay: {e}")

    def _resolve_player_rb_path(self, stage) -> Optional[str]:
        """Finds player rigid body path agnostically without hardcoded paths."""
        try:
            from .player_controller import HydragonPlayerControllerSystem
            p_sys = HydragonPlayerControllerSystem.get_instance()
            if p_sys:
                rb = p_sys.get_player_rb_path()
                if rb:
                    return rb
        except Exception:
            pass

        if stage:
            for prim in stage.Traverse():
                if prim.IsValid() and prim.IsActive() and HydragonPlayerController.is_applied(prim):
                    rb = _find_rigid_body_descendant(prim)
                    if rb:
                        return rb.GetPath().pathString
        return None

    def _get_prim_realtime_pos(self, prim, path_str: str) -> Optional[Tuple[float, float, float]]:
        """Queries PhysX live transform or USD fallback."""
        if prim and hasattr(prim, "IsValid") and prim.IsValid() and prim.IsActive() and _is_rigid_body(prim):
            try:
                physx_iface = get_physx_interface()
                if physx_iface and hasattr(physx_iface, "get_rigidbody_transformation"):
                    rb_data = physx_iface.get_rigidbody_transformation(path_str)
                    if rb_data and rb_data.get("ret_val", False):
                        p = rb_data.get("position")
                        if p is not None:
                            return (float(p[0]), float(p[1]), float(p[2]))
            except Exception:
                pass

        return self._compute_prim_world_pos(prim)
