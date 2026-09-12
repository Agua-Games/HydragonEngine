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
    from pxr import Usd, UsdGeom, Gf, Tf, UsdUtils
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    UsdUtils = None
    Tf = None

from . import volume_bounds, volume_triggers
from .schemas import HydragonKillVolume, HydragonGameManager


# NOTE: `sync_kill_volume_wireframe()` used to live here. It was never called, and it delegated to
# `force_volume_controller.update_wireframe_guide()`, which emitted geometry that is INVALID USD
# (`curveVertexCounts = [2]`, when `linear` + `nonperiodic` requires more than two vertices). Its
# role is now filled by `volume_triggers.ensure_wireframe()`, which serves both volume types.


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
        self._half_extents: Tuple[float, float, float] = (500.0, 50.0, 500.0)
        self._radius: float = 500.0
        self._half_height: float = 50.0

        #: The measured region centre, in the volume's OWN local frame. The shape tests compare
        #: against the extents above AROUND THIS CENTRE, not around the local origin.
        self._local_centre: Tuple[float, float, float] = (0.0, 0.0, 0.0)

        self._cache_bounds_and_transforms()

    @property
    def prim_path(self) -> str:
        return self._prim_path

    @property
    def world_pos(self) -> Tuple[float, float, float]:
        return self._world_pos

    @property
    def local_centre(self) -> Tuple[float, float, float]:
        """The region centre in the volume's own local frame, i.e. the frame the shape tests use."""
        return self._local_centre

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

    def refresh_bounds(self) -> bool:
        """Re-derives the shape parameters from the volume's own geometry.

        Called after `volume_triggers.ensure_volume()` has regenerated the wireframe, and after a
        USD notice reports that the geometry was edited. Overlap detection no longer reads these
        numbers - PhysX does, from the trigger collider - so there is no per-step polling any more.
        """
        previous = (self._half_extents, self._radius, self._half_height)
        self._cache_bounds_and_transforms()
        return (self._half_extents, self._radius, self._half_height) != previous

    def _cache_bounds_and_transforms(self):
        """Caches the world transform, the region centre, and the shape parameters.

        Dimensions and centre are measured from the WIREFRAME in the volume root's own frame, which
        is the frame the trigger collider is authored in. A world-space measurement would apply the
        volume's own scale a second time.
        """
        if not HAS_KIT or not self._prim or not hasattr(self._prim, "IsValid") or not self._prim.IsValid():
            return

        try:
            world_xf = UsdGeom.Xformable(self._prim).ComputeLocalToWorldTransform(Usd.TimeCode.Default())
            self._world_transform = world_xf

            # 50.0 keeps the legacy fallback for a kill volume with no wireframe at all; the
            # shipped kill_bounds authors extent +/-50.
            self._half_extents, self._radius, self._half_height = volume_bounds.measure_shape_parameters(
                self._prim, "Kill", default_half_extent=50.0
            )

            # The region is centred on the GEOMETRY, not on the volume root's origin.
            bound = volume_bounds.measure_bound(self._prim, "Kill")
            if bound is not None:
                # `bound[0]` is the centre expressed in the volume root's frame - the very frame
                # `check_shape_overlap` works in - so it can be stored as the local centre as-is.
                self._local_centre = tuple(float(v) for v in bound[0])
                world_centre = world_xf.Transform(Gf.Vec3d(*bound[0]))
            else:
                self._local_centre = (0.0, 0.0, 0.0)
                world_centre = world_xf.ExtractTranslation()
            self._world_pos = (float(world_centre[0]), float(world_centre[1]), float(world_centre[2]))

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to cache bounds for kill volume {self._prim_path}: {e}")

    # NOTE: `check_overlap()` used to live here. It is gone because overlap is no longer decided in
    # Python: the volume's trigger collider lets PhysX resolve it, on its own threads, against the
    # same shape - including the `Plane` case, which is represented by a thin BOX slab because
    # PhysX has no infinite-plane collider. See `volume_geometry.PLANE_TRIGGER_THICKNESS`.

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
    """Simulation system managing all active HydragonKillVolumeAPI hazard zones.

    Strictly follows ECS guidelines: discovers volumes once on PLAY and indexes them in memory. A
    hazard is an instant event, so this system has NO physics step subscription - it reacts purely
    to PhysX trigger reports, which is what removes the old per-step scan over every zone and every
    adversary.
    """

    _instance: Optional["HydragonKillVolumeSystem"] = None

    def __init__(self):
        HydragonKillVolumeSystem._instance = self
        self._is_active: bool = False
        self._is_simulating: bool = False
        self._timeline_sub = None
        self._app_update_sub = None
        self._physics_step_sub = None
        #: USD ObjectsChanged listener, so an edit to a volume revalidates its geometry.
        self._stage_notice_listener = None
        #: Reported-once guards. AGENTS.md section E: a defect must cost one log line, not one per
        #: report for the life of the session.
        self._trigger_failure_logged: bool = False
        self._report_error_logged: bool = False

        self._active_zones: Dict[str, HydragonKillZone] = {}
        #: zone path -> its trigger collider prim, so the per-step read needs no stage lookup.
        self._trigger_prims: Dict[str, Any] = {}
        #: zone path -> {collider path: resolved body record} as of the PREVIOUS physics step.
        #: The entering edge is computed by differencing snapshots rather than taken from a report,
        #: which also catches a body that was already inside when the simulation started.
        self._inside: Dict[str, Dict[str, Dict[str, Any]]] = {}
        #: Expiry timestamps per (zone, rigid body), replacing the old single global player cooldown.
        self._cooldowns: Dict[Tuple[str, str], float] = {}
        #: Cached HydragonGameManager path, so no traversal is needed when a penalty is applied.
        self._game_manager_path: str = ""
        #: A USD edit arrived; coalesce it into one resync on the next app update instead of
        #: authoring from inside a USD notice callback.
        self._sync_pending: bool = False
        self._stage_id: int = 0

    @classmethod
    def get_instance(cls) -> Optional["HydragonKillVolumeSystem"]:
        return cls._instance

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating

    @property
    def active_zone_count(self) -> int:
        return len(self._active_zones)

    def startup(self):
        """Initializes subscriptions to the timeline, the app update stream and USD notices."""
        self._is_active = True
        if not HAS_KIT:
            return

        try:
            timeline = omni.timeline.get_timeline_interface()
            self._timeline_sub = timeline.get_timeline_event_stream().create_subscription_to_pop(
                self._on_timeline_event
            )
            self._subscribe_physics()
            app = omni.kit.app.get_app() if omni.kit and omni.kit.app else None
            if app:
                self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                    self._on_app_update, name="HydragonKillVolumeSync"
                )
            self._register_objects_changed_notice()
            # The geometry is authored DATA, so it must be present and correct even before the
            # simulation starts - the artist has to be able to see the cage while editing.
            self._sync_zones_now()
            if carb:
                carb.log_info("[hydragon.editor.core] HydragonKillVolumeSystem started.")
        except Exception as e:
            if carb:
                carb.log_error(f"[hydragon.editor.core] HydragonKillVolumeSystem startup failed: {e}")

    def shutdown(self):
        """Cleans up every subscription and the cached hazard registry."""
        self._is_active = False
        self._is_simulating = False
        self._unregister_objects_changed_notice()
        for zone_path in list(self._active_zones):
            volume_triggers.forget(zone_path)
        self._cooldowns.clear()
        self._inside.clear()
        self._trigger_prims.clear()
        self._active_zones.clear()
        self._game_manager_path = ""
        self._timeline_sub = None
        self._app_update_sub = None
        self._physics_step_sub = None
        if carb:
            carb.log_info("[hydragon.editor.core] HydragonKillVolumeSystem shut down.")

    def _subscribe_physics(self) -> bool:
        """Subscribes to the physics step.

        The hazard response is driven by differencing consecutive trigger-state SNAPSHOTS. That costs
        a per-step callback, which an event-only design would not have needed - but trigger reports
        identify the actor with a Fabric handle that cannot be resolved to a USD path from Python, so
        an event model cannot tell WHICH entity entered. A snapshot can.
        """
        if not HAS_KIT:
            return False
        try:
            physx_iface = get_physx_interface()
            if physx_iface and hasattr(physx_iface, "subscribe_physics_step_events"):
                self._physics_step_sub = physx_iface.subscribe_physics_step_events(
                    self._on_physics_step
                )
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
            self._cooldowns.clear()
            self._sync_zones_now()
        elif event_type in (
            int(omni.timeline.TimelineEventType.STOP),
            int(omni.timeline.TimelineEventType.PAUSE),
        ):
            self._is_simulating = False
            self._cooldowns.clear()
            self._inside.clear()
            self._active_zones.clear()
            self._trigger_prims.clear()

    def _register_objects_changed_notice(self):
        """Listens for USD edits so a changed volume is revalidated without any polling."""
        try:
            stage = volume_triggers.current_stage()
            if not stage or not Tf or not hasattr(Tf, "Notice"):
                return
            self._stage_notice_listener = Tf.Notice.Register(
                Usd.Notice.ObjectsChanged, self._on_objects_changed, stage
            )
        except Exception as error:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Could not register the kill volume USD notice: {error}")

    def _unregister_objects_changed_notice(self):
        if self._stage_notice_listener is not None:
            try:
                self._stage_notice_listener.Revoke()
            except Exception:
                pass
            self._stage_notice_listener = None

    def _on_objects_changed(self, notice, sender):
        """Flags a resync when an edit affects a hazard, or IS a hazard.

        A flag, not a direct reaction: authoring USD from inside a USD notice callback is a
        re-entrancy hazard, and the flag coalesces a burst of edits into one resync.

        BOTH halves matter. Reacting only to paths UNDER a registered volume meant the first hazard
        placed in a scene was never seen - nothing was registered yet, so nothing could match - and
        its wireframe only showed up at the next PLAY. See the Force controller for the same fix.
        """
        if not self._is_active:
            return
        try:
            changed_paths = volume_triggers.notice_prim_paths(notice)
            if not changed_paths:
                return
            if volume_triggers.touches_any(changed_paths, self._active_zones):
                self._sync_pending = True
                return
            if volume_triggers.paths_include_volume(changed_paths, HydragonKillVolume.is_applied):
                self._sync_pending = True
        except Exception:
            pass

    def _on_app_update(self, event):
        """Runs a coalesced resync. One boolean check per frame; no traversal unless an edit came in."""
        if self._sync_pending:
            self._sync_pending = False
            self._sync_zones_now()

    def _sync_zones_now(self) -> None:
        """Syncs the registry and the authored geometry against the open stage.

        Guarded, because this is reached from the timeline callback and the per-frame app update
        stream. An exception escaping here would repeat on every event, which is the log-spam failure
        mode of AGENTS.md section E; it is reported once and then stops.
        """
        stage = volume_triggers.current_stage()
        if not stage:
            return
        try:
            self._sync_zones(stage)
        except Exception as error:
            if not self._report_error_logged:
                self._report_error_logged = True
                if carb:
                    carb.log_error(f"[hydragon.editor.core] Kill volume sync failed: {error}")
                    try:
                        import traceback

                        carb.log_error(traceback.format_exc())
                    except Exception:
                        pass

    def _sync_zones(self, stage) -> None:
        """Authors geometry for every Kill Volume and rebuilds the in-memory registry.

        Idempotent, because `volume_triggers` skips authoring when nothing changed - which is what
        makes it safe to call after an arbitrary USD edit, and what stops the authoring itself from
        producing an endless stream of further edits.
        """
        if not HAS_KIT or not stage:
            return

        self._stage_id = UsdUtils.StageCache.Get().GetId(stage).ToLongInt() if UsdUtils else 0
        seen: Set[str] = set()

        for prim in stage.Traverse():
            if not prim or not prim.IsValid():
                continue
            # AGENTS.md section C destroys an entity with SetActive(False); an inactive hazard must
            # not stay registered, and must not keep a trigger collider in the simulation.
            if not prim.IsActive():
                continue
            if not HydragonKillVolume.is_applied(prim):
                continue

            zone_path = str(prim.GetPath())
            seen.add(zone_path)

            _wireframe, trigger = volume_triggers.ensure_volume(prim, "Kill")
            if trigger is None:
                self._report_trigger_failure(zone_path)
                continue

            self._trigger_prims[zone_path] = trigger

            zone = self._active_zones.get(zone_path)
            if zone is None:
                zone = HydragonKillZone(prim)
                self._active_zones[zone_path] = zone
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Registered Kill Volume: {zone_path} "
                        f"(shape={zone.volume_shape}, penalty={zone.penalty_score})"
                    )
            else:
                zone.refresh_bounds()

        for stale_path in [p for p in self._active_zones if p not in seen]:
            self._active_zones.pop(stale_path)
            self._inside.pop(stale_path, None)
            self._trigger_prims.pop(stale_path, None)
            volume_triggers.forget(stale_path)
            if carb:
                carb.log_info(f"[hydragon.editor.core] Unregistered Kill Volume: {stale_path}")

    def _report_trigger_failure(self, zone_path: str) -> None:
        """Fails loud, once. Without a trigger the hazard has no gameplay effect at all.

        Deliberately NOT a silent fallback to the old analytic test: a fallback would hide an
        incomplete migration and quietly reinstate the second source of truth this change removes.
        """
        if self._trigger_failure_logged:
            return
        self._trigger_failure_logged = True
        if carb:
            carb.log_error(
                "[hydragon.editor.core] Could not author a PhysX trigger for Kill Volume "
                f"'{zone_path}'. Kill volumes will have NO effect until this is fixed. "
                "Check that UsdPhysics/PhysxSchema are available and that the prim is a valid Gprim."
            )

    def _on_physics_step(self, dt: float):
        """Handles hazards for whatever PhysX currently reports inside each kill volume.

        A hazard is an INSTANT event, so only the ENTERING edge is acted on - but the edge is computed
        here by differencing snapshots, not taken from a trigger report. A report identifies the actor
        with a Fabric handle that cannot be resolved to a USD path from Python, which is why the
        report-driven version could not tell the Player from a Foe.
        """
        if not self._is_simulating or not self._active_zones:
            return

        for zone_path, zone in list(self._active_zones.items()):
            trigger_prim = self._trigger_prims.get(zone_path)
            if trigger_prim is None or not trigger_prim.IsValid():
                continue

            previous = self._inside.get(zone_path, {})
            current: Dict[str, Dict[str, Any]] = {}
            entered: List[str] = []
            for collider_path in volume_triggers.triggered_colliders(trigger_prim):
                record = volume_triggers.resolve_inside_body(collider_path, zone.filter_faction)
                if record is None:
                    continue
                current[collider_path] = record
                if collider_path not in previous:
                    entered.append(collider_path)

            self._inside[zone_path] = current

            if not entered or not zone.is_enabled:
                continue

            stage = volume_triggers.current_stage()
            now = time.time()
            for collider_path in entered:
                record = current[collider_path]
                # Cooldown per (zone, rigid body), not one global timer: a single timer let an
                # unrelated hazard hit silence a second one, and a zone could not tell its own
                # victims apart.
                key = (zone_path, record["rb_path"])
                if now < self._cooldowns.get(key, 0.0):
                    continue
                self._cooldowns[key] = now + max(0.5, zone.respawn_delay)

                if record["faction"] == "Player":
                    self._handle_player_hazard(zone, record["rb_path"], stage)
                elif record["faction"] == "Enemy":
                    self._handle_foe_hazard(zone, record["entity_key"], record["rb_path"], stage)

    def _body_position(self, body_path: str) -> Optional[Tuple[float, float, float]]:
        """The body's simulated position, i.e. its centre of mass, or None.

        Used to place the elimination VFX. Read from PhysX rather than from the entity controller so
        the burst lands on the body that actually entered the volume.
        """
        physx_iface = get_physx_interface() if HAS_KIT else None
        if not physx_iface or not body_path:
            return None
        try:
            transform = physx_iface.get_rigidbody_transformation(body_path)
            position = None
            if isinstance(transform, dict):
                position = transform.get("position")
            elif transform is not None and hasattr(transform, "position"):
                position = transform.position
            if position is None:
                return None
            return (float(position[0]), float(position[1]), float(position[2]))
        except Exception:
            return None

    def _handle_player_hazard(self, zone, player_body_path: str, stage):
        """Executes the penalty, effects and respawn for a player that entered a hazard."""
        try:
            from .player_controller import HydragonPlayerControllerSystem

            player_sys = HydragonPlayerControllerSystem.get_instance()
            if not player_sys or not player_sys.is_active_and_simulating():
                return

            position = self._body_position(player_body_path) or player_sys.get_player_world_pos()
            if carb:
                carb.log_info(
                    f"[hydragon.editor.core] Player entered Kill Volume '{zone.prim_path}' at {position}!"
                )

            penalty = zone.penalty_score
            if penalty > 0:
                self._apply_score_penalty(stage, penalty)

            if zone.spawn_effects and position:
                self._trigger_vfx(stage, position)

            if zone.sound_enabled:
                self._trigger_sfx(zone.sound_asset_path)

            respawn_pos = zone.get_custom_respawn_world_pos(stage) if stage else None
            player_sys.request_respawn(custom_spawn_pos=respawn_pos)

        except Exception as error:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Player kill volume response failed: {error}")

    def _handle_foe_hazard(self, zone, foe_path: str, foe_body_path: str, stage):
        """Destroys an adversary that entered a hazard, via the ECS deactivation path."""
        try:
            from .foes_controller import HydragonFoesControllerSystem

            foes_sys = HydragonFoesControllerSystem.get_instance()
            if not foes_sys or not foes_sys.is_active_and_simulating():
                return

            brain = getattr(foes_sys, "_active_brains", {}).get(foe_path)
            if not brain or not brain.is_alive or not zone.destroy_foes_instantly:
                return

            if carb:
                carb.log_info(
                    f"[hydragon.editor.core] Foe '{foe_path}' destroyed by Kill Volume '{zone.prim_path}'."
                )

            if zone.spawn_effects:
                position = self._body_position(foe_body_path) or brain.current_pos
                if position:
                    self._trigger_vfx(stage, position)

            # Queue clean destruction on the main thread, outside the simulation lock.
            foes_sys.queue_foe_destruction(foe_path, is_stomp=False)

        except Exception as error:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Foe kill volume response failed: {error}")

    def _apply_score_penalty(self, stage, penalty: int):
        """Applies score deduction to HydragonGameManager if present.

        The GameManager path is resolved once and cached. The previous version ran a full
        stage.Traverse() here - i.e. inside the physics step, which AGENTS.md section B forbids - and
        it did so on every hazard hit, so the cost grew with the size of the stage.
        """
        if not stage or not HAS_KIT:
            return

        try:
            if not self._game_manager_path:
                for prim in stage.Traverse():
                    if HydragonGameManager.is_applied(prim):
                        self._game_manager_path = str(prim.GetPath())
                        break
            prim = stage.GetPrimAtPath(self._game_manager_path) if self._game_manager_path else None
            if prim and prim.IsValid():
                gm = HydragonGameManager(prim)
                gm.score = max(0, gm.score - penalty)
                if carb:
                    carb.log_info(f"[hydragon.editor.core] Applied hazard penalty -{penalty} score. New score: {gm.score}")
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
