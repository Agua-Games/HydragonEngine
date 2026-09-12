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
from typing import Any, Dict, Optional, Set, Tuple

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
    from pxr import Usd, UsdGeom, Gf, Tf, UsdUtils, PhysicsSchemaTools
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    PhysicsSchemaTools = None
    UsdUtils = None
    UsdGeom = None
    Tf = None
    Gf = None

if not HAS_KIT or Gf is None:
    class MockVec3f(tuple):
        def __new__(cls, x, y, z):
            return super().__new__(cls, (float(x), float(y), float(z)))
    class MockGf:
        Vec3f = MockVec3f
    if Gf is None:
        Gf = MockGf

from . import volume_bounds, volume_triggers
from .schemas import HydragonForceVolume, HydragonActor


# NOTE: `update_wireframe_guide()` and `sync_force_volume_wireframe()` used to live here. Neither
# was ever called, and both were harmful if they had been: they emitted `curveVertexCounts = [2] * 12`
# for a Box, which is INVALID for `linear` + `nonperiodic` (the schema requires more than two
# vertices per curve), and they wrote through whatever edit target happened to be current, so the
# geometry could land in a layer that is never saved. Their role is now filled by
# `volume_triggers.ensure_wireframe()`, which generates schema-valid geometry from
# `volume_geometry` and always authors into the root layer, so the volume travels with the stage.


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
        self._half_extents: Tuple[float, float, float] = (100.0, 100.0, 100.0)
        self._radius: float = 100.0
        self._half_height: float = 100.0

        #: The measured region centre, in the volume's OWN local frame. Kept as the record of where
        #: the geometry actually sits, since the wireframe is not necessarily centred on the volume
        #: root - the shipped asset offsets `force_bounds` with xformOp:translate.
        self._local_centre: Tuple[float, float, float] = (0.0, 0.0, 0.0)

        self._impulse_timers: Dict[str, float] = {}

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

    def refresh_bounds(self) -> bool:
        """Re-derives the shape parameters from the volume's own geometry.

        Called after `volume_triggers.ensure_volume()` has regenerated the wireframe, and after a
        USD notice reports that the geometry was edited. There is no per-physics-step polling any
        more: the geometry is validated on change, and overlap detection no longer reads these
        numbers at all - PhysX does, from the trigger collider.
        """
        previous = (self._half_extents, self._radius, self._half_height)
        self._cache_bounds_and_transforms()
        return (self._half_extents, self._radius, self._half_height) != previous

    def _cache_bounds_and_transforms(self):
        """Caches the world transform, the region centre, and the shape parameters.

        The dimensions and the centre are measured from the WIREFRAME in the volume root's own
        frame. Expressing them in that frame is what makes them usable: a WORLD-space measurement
        would apply the volume's own scale a second time. Measured on the shipped asset, that
        mistake made a volume 6x too large in every axis.

        These numbers now serve only the continuous force maths (radial and vortex are polar around
        the centre) - overlap detection itself is PhysX's job, from the trigger collider, which is
        generated from this same measurement.
        """
        if not HAS_KIT or not self._prim or not hasattr(self._prim, "IsValid") or not self._prim.IsValid():
            return

        try:
            world_xf = UsdGeom.Xformable(self._prim).ComputeLocalToWorldTransform(Usd.TimeCode.Default())
            self._world_transform = world_xf

            self._half_extents, self._radius, self._half_height = volume_bounds.measure_shape_parameters(
                self._prim, "Force", default_half_extent=100.0
            )

            # The region is centred on the GEOMETRY, not on the volume root's origin: the shipped
            # asset offsets force_bounds with xformOp:translate = (0, 22, 0).
            bound = volume_bounds.measure_bound(self._prim, "Force")
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
                carb.log_warn(f"[hydragon.editor.core] Failed to cache bounds for force volume {self._prim_path}: {e}")

    # NOTE: `check_shape_overlap()` and `check_overlap()` used to live here. They are gone because
    # overlap is no longer decided in Python: the volume's trigger collider lets PhysX resolve it,
    # on its own threads, against the same shape. A body that reaches `compute_forces` with
    # `in_shape=True` is one PhysX has already reported as inside.
    #
    # One deliberate behaviour change comes with that. The old `check_overlap()` also accepted a
    # body OUTSIDE the drawn volume but within `force:radialRadius` - an uncoupled influence zone.
    # Influence is now bounded by the trigger volume, i.e. by the cage the artist drew and can see,
    # which is the whole point of the migration: the gizmo and the affected region can no longer
    # disagree. A volume that relied on reaching past its own bounds needs its cage enlarged.

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

        Called only for bodies PhysX has reported inside this volume's trigger collider, so the
        shape-dependent terms (linear, turbulence, vortex, dampening) are always in play. The radial
        term keeps its own distance falloff, which is a property of the force, not of the boundary.
        """
        if not self._schema:
            return (0.0, 0.0, 0.0), body_vel, body_ang_vel

        schema = self._schema
        fx, fy, fz = 0.0, 0.0, 0.0
        in_shape = True

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
        self._app_update_sub = None
        #: USD ObjectsChanged listener, so an edit to a volume revalidates its geometry.
        self._stage_notice_listener = None
        #: Reported-once guard. AGENTS.md section E: a defect must cost one log line per session.
        self._trigger_failure_logged: bool = False
        self._report_error_logged: bool = False

        self._active_volumes: Dict[str, HydragonForceVolumeZone] = {}
        #: volume path -> its trigger collider prim, so the per-step read needs no stage lookup.
        self._trigger_prims: Dict[str, Any] = {}
        #: volume path -> {collider path: resolved body record} as of the PREVIOUS physics step.
        #: The enter edge is computed by differencing snapshots rather than taken from a report,
        #: which also catches a body that was already inside when the simulation started.
        self._inside: Dict[str, Dict[str, Dict[str, Any]]] = {}
        #: A USD edit arrived; coalesce it into one resync on the next app update instead of
        #: authoring from inside a USD notice callback.
        self._sync_pending: bool = False
        self._stage_id: int = 0
        self._last_dt: float = 0.0
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
        """Initializes subscriptions to timeline, physics simulation and the app update stream."""
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
                    self._on_app_update, name="HydragonForceVolumeSync"
                )
            self._register_objects_changed_notice()
            # The geometry is authored DATA, so it must be present and correct even before the
            # simulation starts - the artist has to be able to see the cage while editing.
            self._sync_volumes_now()
            if carb:
                carb.log_info("[hydragon.editor.core] HydragonForceVolumeSystem started.")
        except Exception as e:
            if carb:
                carb.log_error(f"[hydragon.editor.core] HydragonForceVolumeSystem startup failed: {e}")

    def shutdown(self):
        """Cleans up every subscription and the cached volume registry."""
        self._is_active = False
        self._is_simulating = False
        self._unregister_objects_changed_notice()
        for volume_path in list(self._active_volumes):
            volume_triggers.forget(volume_path)
        self._inside.clear()
        self._trigger_prims.clear()
        self._active_volumes.clear()
        self._timeline_sub = None
        self._physics_step_sub = None
        self._app_update_sub = None
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
            self._inside.clear()
            self._sync_volumes_now()
        elif event_type in (
            int(omni.timeline.TimelineEventType.STOP),
            int(omni.timeline.TimelineEventType.PAUSE),
        ):
            self._is_simulating = False
            self._inside.clear()
            self._active_volumes.clear()
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
                carb.log_warn(f"[hydragon.editor.core] Could not register the volume USD notice: {error}")

    def _unregister_objects_changed_notice(self):
        if self._stage_notice_listener is not None:
            try:
                self._stage_notice_listener.Revoke()
            except Exception:
                pass
            self._stage_notice_listener = None

    def _on_objects_changed(self, notice, sender):
        """Flags a resync when an edit affects a volume, or IS a volume.

        Setting a flag rather than reacting directly is deliberate: authoring USD from inside a USD
        notice callback is a re-entrancy hazard, and the flag also coalesces a burst of edits into
        one resync.

        BOTH halves matter. Reacting only to paths UNDER a registered volume meant the first volume
        placed in a scene was never seen - nothing was registered yet, so nothing could match - and
        its wireframe only showed up at the next PLAY. That was exactly the reported symptom of
        `Create > Hydragon > Force Volume` producing a bare prim with no `volumes/force_bounds`.
        """
        if not self._is_active:
            return
        try:
            changed_paths = volume_triggers.notice_prim_paths(notice)
            if not changed_paths:
                return
            if volume_triggers.touches_any(changed_paths, self._active_volumes):
                self._sync_pending = True
                return
            if volume_triggers.paths_include_volume(changed_paths, HydragonForceVolume.is_applied):
                self._sync_pending = True
        except Exception:
            pass

    def _on_app_update(self, event):
        """Runs a coalesced resync. One boolean check per frame; no traversal unless an edit came in."""
        if self._sync_pending:
            self._sync_pending = False
            self._sync_volumes_now()

    def _sync_volumes_now(self) -> None:
        """Syncs the registry and the authored geometry against the open stage.

        Guarded, because this is reached from the timeline callback and the per-frame app update
        stream. An exception escaping here would repeat on every event, which is the log-spam failure
        mode of AGENTS.md section E; it is reported once and then stops.
        """
        stage = volume_triggers.current_stage()
        if not stage:
            return
        try:
            self._sync_volumes(stage)
        except Exception as error:
            if not self._report_error_logged:
                self._report_error_logged = True
                if carb:
                    carb.log_error(f"[hydragon.editor.core] Force volume sync failed: {error}")
                    try:
                        import traceback

                        carb.log_error(traceback.format_exc())
                    except Exception:
                        pass

    def _sync_volumes(self, stage) -> None:
        """Authors geometry for every Force Volume and rebuilds the in-memory registry.

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
            # AGENTS.md section C destroys an entity with SetActive(False); an inactive volume must
            # not stay registered, and it must not keep a trigger collider in the simulation.
            if not prim.IsActive():
                continue
            if not HydragonForceVolume.is_applied(prim):
                continue

            volume_path = str(prim.GetPath())
            seen.add(volume_path)

            _wireframe, trigger = volume_triggers.ensure_volume(prim, "Force")
            if trigger is None:
                self._report_trigger_failure(volume_path)
                continue

            self._trigger_prims[volume_path] = trigger

            zone = self._active_volumes.get(volume_path)
            if zone is None:
                zone = HydragonForceVolumeZone(prim)
                self._active_volumes[volume_path] = zone
                if carb:
                    carb.log_info(
                        f"[hydragon.editor.core] Registered Force Volume: {volume_path} "
                        f"(mode={zone.mode}, shape={zone.volume_shape})"
                    )
            else:
                # The geometry was just re-authored, so the cached centre and extents may have moved.
                zone.refresh_bounds()

        for stale_path in [p for p in self._active_volumes if p not in seen]:
            self._active_volumes.pop(stale_path)
            self._inside.pop(stale_path, None)
            self._trigger_prims.pop(stale_path, None)
            volume_triggers.forget(stale_path)
            if carb:
                carb.log_info(f"[hydragon.editor.core] Unregistered Force Volume: {stale_path}")

    def _report_trigger_failure(self, volume_path: str) -> None:
        """Fails loud, once. Without a trigger the volume has no gameplay effect at all.

        Deliberately NOT a silent fallback to the old analytic test: a fallback would hide an
        incomplete migration and quietly reinstate the second source of truth the whole change
        exists to remove.
        """
        if self._trigger_failure_logged:
            return
        self._trigger_failure_logged = True
        if carb:
            carb.log_error(
                "[hydragon.editor.core] Could not author a PhysX trigger for Force Volume "
                f"'{volume_path}'. Force volumes will have NO effect until this is fixed. "
                "Check that UsdPhysics/PhysxSchema are available and that the prim is a valid Gprim."
            )

    def _on_physics_step(self, dt: float):
        """Applies the forces for whatever PhysX currently reports inside each volume.

        The old version scanned every volume against every tracked body, every step. There is no scan
        now, and no Python overlap test either: PhysX resolves the overlap in its own broadphase and
        keeps `physxTrigger:triggeredCollisions` current, so this reads that back and differences it
        against the previous step.

        Reading a SNAPSHOT rather than consuming edges also closes a hole an event model has: a body
        already inside when the simulation starts never produces an enter event, but it is present in
        the first snapshot.
        """
        if not self._is_simulating or not self._active_volumes:
            return

        self._sim_time += dt
        self._last_dt = dt

        for volume_path, zone in list(self._active_volumes.items()):
            trigger_prim = self._trigger_prims.get(volume_path)
            if trigger_prim is None or not trigger_prim.IsValid():
                continue

            previous = self._inside.get(volume_path, {})
            current: Dict[str, Dict[str, Any]] = {}
            for collider_path in volume_triggers.triggered_colliders(trigger_prim):
                record = volume_triggers.resolve_inside_body(collider_path, zone.filter_faction)
                if record is not None:
                    current[collider_path] = record

            for collider_path, record in current.items():
                if zone.mode == "Impulse":
                    # An impulse is the ENTERING EDGE only. A body present in the first snapshot
                    # counts as entering, which is why `previous` starts empty.
                    if collider_path not in previous:
                        self._apply_force_to_body(zone, record["rb_path"], mode="Impulse")
                else:
                    self._apply_force_to_body(zone, record["rb_path"], mode="Force")

            self._inside[volume_path] = current

    def _apply_force_to_body(self, zone, body_path: str, mode: str) -> None:
        """Evaluates a volume's force for one body and hands it to PhysX."""
        if mode == "Impulse" and not zone.can_apply_impulse(body_path, time.time()):
            return

        state = self._body_state(body_path)
        if state is None:
            return

        sim_iface = get_physx_simulation_interface() if HAS_KIT else None
        if sim_iface is None:
            return

        position = state["world_pos"]
        force_vec, damped_v, damped_w = zone.compute_forces(
            position, state["linear_vel"], state["angular_vel"], self._sim_time, self._last_dt
        )

        if any(abs(component) > 1e-3 for component in force_vec):
            sim_iface.apply_force_at_pos(
                self._stage_id,
                state["prim_id"],
                carb.Float3(force_vec[0], force_vec[1], force_vec[2]),
                carb.Float3(position[0], position[1], position[2]),
                mode,
            )

        if mode != "Impulse" and (
            damped_v != state["linear_vel"] or damped_w != state["angular_vel"]
        ):
            self._set_body_velocities(body_path, damped_v, damped_w)

    def _set_body_velocities(self, body_path: str, linear, angular) -> None:
        """Writes damped velocities back to a body.

        Dampening is applied by writing velocities rather than by PhysX damping attributes, so that
        it stops the instant the body leaves the volume.
        """
        physx_iface = get_physx_interface() if HAS_KIT else None
        if not physx_iface:
            return
        try:
            for set_linear in ("set_rigidbody_linear_velocity", "set_linear_velocity"):
                if hasattr(physx_iface, set_linear):
                    getattr(physx_iface, set_linear)(
                        body_path, carb.Float3(linear[0], linear[1], linear[2])
                    )
                    break
            for set_angular in ("set_rigidbody_angular_velocity", "set_angular_velocity"):
                if hasattr(physx_iface, set_angular):
                    getattr(physx_iface, set_angular)(
                        body_path, carb.Float3(angular[0], angular[1], angular[2])
                    )
                    break
        except Exception:
            pass

    def _body_state(self, body_path: str) -> Optional[Dict[str, Any]]:
        """Reads a body's simulated state from PhysX.

        `get_rigidbody_transformation` returns the body's own transform, i.e. its centre of mass.
        That is where AGENTS.md section D requires a force to be applied, and it is a correction:
        the previous implementation applied forces at the prim position reported by the controller,
        which is not the same point for a body with an offset collider.
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

            linear, angular = self._get_body_velocities(physx_iface, body_path)
            return {
                "world_pos": (float(position[0]), float(position[1]), float(position[2])),
                "linear_vel": linear,
                "angular_vel": angular,
                "prim_id": PhysicsSchemaTools.sdfPathToInt(body_path) if PhysicsSchemaTools else 0,
            }
        except Exception:
            return None

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
