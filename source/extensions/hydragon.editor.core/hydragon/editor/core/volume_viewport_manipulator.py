"""
Hydragon Engine - Volume Viewport Overlay Manipulator

High-performance viewport overlay using `omni.ui.scene` to draw lightweight,
editor-only wireframe bounds for Force Volumes and Kill Volumes (Box, Sphere,
Cylinder, Plane). Zero USD stage pollution and zero RTX raytracing overhead.
"""

import math
import time
from typing import Dict, List, Optional, Set, Tuple

try:
    import carb
    import carb.settings
    import carb.input
    import omni.kit.app
    import omni.usd
    import omni.ui as ui
    from omni.ui import scene as sc
    from omni.ui import color as cl
    from pxr import Usd, UsdGeom, Gf, Sdf, Tf
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    ui = None
    sc = None
    cl = None
    Usd = None
    UsdGeom = None
    Gf = None
    Sdf = None
    Tf = None

from .schemas import HydragonForceVolume, HydragonKillVolume

SETTING_SHOW_VOLUMES = "/persistent/app/hydragon/viewport/showVolumes"

#: How long a click on a volume gizmo keeps defending its selection, in seconds.
#:
#: The native selection is applied by an *asynchronous* raycast: the viewport's selection
#: manipulator calls `viewport_api.request_pick()`, which resolves through a completion callback
#: some frames later. Gesture arbitration is supposed to prevent that gesture from running at all
#: (`_SelectionPreventer` + `priority`), but it demonstrably does not for a third-party scene, so
#: the native write can land after ours. This window exists to outlive that late write; it is
#: cancelled the moment the user starts a new click anywhere.
SELECTION_OVERRIDE_WINDOW_S = 1.0


class VolumeSelectGesture(sc.ClickGesture if (HAS_KIT and sc and hasattr(sc, "ClickGesture")) else object):
    """
    Click gesture that selects the volume prim when its wireframe is clicked.

    The native single-click selection is itself an ``sc.ClickGesture``
    (``omni.kit.manipulator.selection.SelectionClickGesture``) carrying ``priority = -100``, and
    ``omni.ui.scene`` is documented to arbitrate competing gestures through
    ``GestureManager.should_prevent``, which resolves in favour of the highest ``priority``.

    Measured behaviour is that this arbitration does **not** take effect against a third-party
    scene: even with this overlay hosted inside the viewport's own ``sc.SceneView`` through
    ``RegisterScene``, the native gesture still runs, its ``viewport_api.request_pick()`` raycast
    still resolves asynchronously, and its result can still overwrite our selection. The priority
    is kept to express the intended contract, while `HydragonVolumeViewportOverlay` defends the
    outcome through its bounded override window.
    """

    #: Documents the intent: outrank SelectionClickGesture (-100). Not sufficient on its own.
    SELECT_PRIORITY = 100

    def __init__(self, prim_path: str, **kwargs):
        self.prim_path = prim_path
        if HAS_KIT and sc and hasattr(sc, "ClickGesture"):
            try:
                super().__init__(
                    name=kwargs.pop("name", f"HydragonVolumeSelectGesture({prim_path})"),
                    **kwargs,
                )
            except Exception:
                # Never leave the underlying C++ gesture uninitialised just because an
                # optional keyword argument was rejected by this build.
                try:
                    super().__init__()
                except Exception:
                    pass

    @property
    def priority(self):
        return self.SELECT_PRIORITY

    def on_ended(self, *args):
        # Mirror the NVIDIA gesture pattern: a click cancelled by a competing gesture
        # must not commit a selection change. Resolved defensively because the enum is
        # only present in Kit builds.
        if HAS_KIT and sc:
            canceled = getattr(getattr(sc, "GestureState", None), "CANCELED", None)
            if canceled is not None and getattr(self, "state", None) == canceled:
                return
        HydragonVolumeViewportOverlay.select_volume(self.prim_path)


def _create_line(p0, p1, line_color, line_thickness, gesture_obj=None):
    """Constructs an sc.Line and attaches gesture_obj if supported."""
    line = sc.Line(p0, p1, color=line_color, thickness=line_thickness, intersection_thickness=6.0)
    if gesture_obj:
        try:
            line.gestures = [gesture_obj]
        except Exception:
            pass
    return line


# Direct color palette for volumes (Default vs Selected)
if HAS_KIT and cl:
    COLOR_FORCE_DEFAULT = cl(0.18, 0.72, 1.0, 0.85)   # Cyan / Blue
    COLOR_FORCE_SELECTED = cl(0.35, 0.92, 1.0, 1.0)   # Electric Cyan
    COLOR_KILL_DEFAULT = cl(1.0, 0.25, 0.25, 0.85)    # Coral Red
    COLOR_KILL_SELECTED = cl(1.0, 0.55, 0.45, 1.0)    # Hot Neon Red
else:
    COLOR_FORCE_DEFAULT = 0xD9FFB82E
    COLOR_FORCE_SELECTED = 0xFFFFAA00
    COLOR_KILL_DEFAULT = 0xD94040FF
    COLOR_KILL_SELECTED = 0xFF738CFF


# Line segment math generator for all 4 volume shapes
def generate_shape_wireframe_segments(
    shape: str,
    half_extents: Tuple[float, float, float] = (100.0, 100.0, 100.0),
    radius: float = 100.0,
    half_height: float = 100.0,
) -> List[Tuple[Tuple[float, float, float], Tuple[float, float, float]]]:
    """
    Computes local-space line segment start and end coordinates.
    Returns: List of ((x0, y0, z0), (x1, y1, z1))
    """
    segments = []

    if shape == "Sphere":
        segs = 32
        r = radius
        # 3 Orthogonal circles: XY (z=0), XZ (y=0), YZ (x=0)
        for i in range(segs):
            t0 = 2.0 * math.pi * (i / segs)
            t1 = 2.0 * math.pi * ((i + 1) / segs)
            c0, s0 = math.cos(t0) * r, math.sin(t0) * r
            c1, s1 = math.cos(t1) * r, math.sin(t1) * r

            # XY
            segments.append(((c0, s0, 0.0), (c1, s1, 0.0)))
            # XZ
            segments.append(((c0, 0.0, s0), (c1, 0.0, s1)))
            # YZ
            segments.append(((0.0, c0, s0), (0.0, c1, s1)))

    elif shape == "Cylinder":
        segs = 32
        r = radius
        h = half_height
        # Top and bottom rings
        for i in range(segs):
            t0 = 2.0 * math.pi * (i / segs)
            t1 = 2.0 * math.pi * ((i + 1) / segs)
            c0, s0 = math.cos(t0) * r, math.sin(t0) * r
            c1, s1 = math.cos(t1) * r, math.sin(t1) * r

            segments.append(((c0, h, s0), (c1, h, s1)))
            segments.append(((c0, -h, s0), (c1, -h, s1)))

        # 4 Axial vertical struts
        for angle in (0.0, math.pi * 0.5, math.pi, math.pi * 1.5):
            cx = math.cos(angle) * r
            cz = math.sin(angle) * r
            segments.append(((cx, -h, cz), (cx, h, cz)))

    elif shape == "Plane":
        hx, hz = half_extents[0], half_extents[2]
        # Perimeter
        segments.append(((-hx, 0.0, -hz), (hx, 0.0, -hz)))
        segments.append(((hx, 0.0, -hz), (hx, 0.0, hz)))
        segments.append(((hx, 0.0, hz), (-hx, 0.0, hz)))
        segments.append(((-hx, 0.0, hz), (-hx, 0.0, -hz)))
        # Diagonals
        segments.append(((-hx, 0.0, -hz), (hx, 0.0, hz)))
        segments.append(((hx, 0.0, -hz), (-hx, 0.0, hz)))

    else:  # Box
        hx, hy, hz = half_extents
        # Bottom rectangle (y = -hy)
        segments.append(((-hx, -hy, -hz), (hx, -hy, -hz)))
        segments.append(((hx, -hy, -hz), (hx, -hy, hz)))
        segments.append(((hx, -hy, hz), (-hx, -hy, hz)))
        segments.append(((-hx, -hy, hz), (-hx, -hy, -hz)))

        # Top rectangle (y = +hy)
        segments.append(((-hx, hy, -hz), (hx, hy, -hz)))
        segments.append(((hx, hy, -hz), (hx, hy, hz)))
        segments.append(((hx, hy, hz), (-hx, hy, hz)))
        segments.append(((-hx, hy, hz), (-hx, hy, -hz)))

        # 4 Vertical edges
        segments.append(((-hx, -hy, -hz), (-hx, hy, -hz)))
        segments.append(((hx, -hy, -hz), (hx, hy, -hz)))
        segments.append(((hx, -hy, hz), (hx, hy, hz)))
        segments.append(((-hx, -hy, hz), (-hx, hy, hz)))

    return segments


BOUNDS_PRIM_REL_PATH = {
    "Force": "volumes/force_bounds",
    "Kill": "volumes/kill_bounds",
}


class VolumeOverlayEntry:
    """
    Maintains a dedicated `sc.Transform` and wireframe geometry for a single registered volume.
    Reuses existing objects across frames; only rebuilds lines when the shape type or authored dimensions change.
    """

    def __init__(self, prim_path: str, volume_type: str, parent_transform: Optional["sc.Transform"]):
        self.prim_path = prim_path
        self.volume_type = volume_type  # "Force" or "Kill"
        self.cached_shape: Optional[str] = None
        self.cached_half_extents: Optional[Tuple[float, float, float]] = None
        self.cached_transform: Optional[List[float]] = None
        self.is_selected: bool = False
        self.needs_rebuild: bool = False
        self.transform_node: Optional["sc.Transform"] = None

        if HAS_KIT and sc and parent_transform:
            try:
                with parent_transform:
                    self.transform_node = sc.Transform(visible=True)
            except Exception as e:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to create Transform node for {prim_path}: {e}")
                self.transform_node = None

    def destroy(self):
        if self.transform_node:
            try:
                self.transform_node.clear()
                self.transform_node.visible = False
            except Exception:
                pass
            self.transform_node = None

    def get_render_color(self):
        if self.volume_type == "Force":
            return COLOR_FORCE_SELECTED if self.is_selected else COLOR_FORCE_DEFAULT
        else:
            return COLOR_KILL_SELECTED if self.is_selected else COLOR_KILL_DEFAULT

    def get_line_thickness(self) -> float:
        return 2.5 if self.is_selected else 1.5

    def get_target_xform_prim(self, prim):
        if prim and hasattr(prim, "IsValid") and prim.IsValid():
            volumes_prim = prim.GetPrimAtPath("volumes")
            if volumes_prim and volumes_prim.IsValid():
                return volumes_prim
        return prim

    def read_shape(self, prim) -> str:
        shape = "Box"
        attr_name = "force:volumeShape" if self.volume_type == "Force" else "kill:volumeShape"
        if prim.HasAttribute(attr_name):
            attr = prim.GetAttribute(attr_name)
            if attr and attr.IsValid():
                val = attr.Get()
                if val:
                    shape = str(val)
        return shape

    def read_dimensions(self, prim) -> Tuple[Tuple[float, float, float], float, float]:
        target_prim = self.get_target_xform_prim(prim)
        is_sub_volume = (target_prim != prim)
        default_extent = 100.0 if self.volume_type == "Force" else (50.0 if is_sub_volume else 500.0)
        base_hx, base_hy, base_hz = default_extent, default_extent, default_extent

        bounds_rel_path = BOUNDS_PRIM_REL_PATH.get(self.volume_type, "volumes/force_bounds")
        bounds_prim = prim.GetPrimAtPath(bounds_rel_path)
        if bounds_prim and bounds_prim.IsValid():
            ext_attr = bounds_prim.GetAttribute("extent")
            if ext_attr and ext_attr.IsValid():
                ext_val = ext_attr.Get()
                if ext_val and len(ext_val) >= 2:
                    base_hx = max(10.0, max(abs(float(ext_val[0][0])), abs(float(ext_val[1][0]))))
                    base_hy = max(10.0, max(abs(float(ext_val[0][1])), abs(float(ext_val[1][1]))))
                    base_hz = max(10.0, max(abs(float(ext_val[0][2])), abs(float(ext_val[1][2]))))

        half_extents = (base_hx, base_hy, base_hz)
        radius = max(base_hx, base_hz)
        half_height = base_hy
        return half_extents, radius, half_height

    def rebuild_lines(self, prim):
        if not HAS_KIT or not sc or not self.transform_node:
            return

        shape = self.read_shape(prim)
        half_extents, radius, half_height = self.read_dimensions(prim)
        self.cached_shape = shape
        self.cached_half_extents = half_extents

        segments = generate_shape_wireframe_segments(
            shape=shape,
            half_extents=half_extents,
            radius=radius,
            half_height=half_height,
        )

        color = self.get_render_color()
        thickness = self.get_line_thickness()

        select_gesture = VolumeSelectGesture(self.prim_path) if (HAS_KIT and sc and hasattr(sc, "ClickGesture")) else None

        # Center pivot handle: a single short cross. It is billboarded to the camera so it
        # always reads as a clean "+" rather than a 3D star, and the arms are kept small.
        arm = min(radius * 0.12, 18.0)
        cross_segments = [
            ((-arm, 0.0, 0.0), (arm, 0.0, 0.0)),
            ((0.0, -arm, 0.0), (0.0, arm, 0.0)),
        ]

        try:
            self.transform_node.clear()
            with self.transform_node:
                # 1. Outer wireframe cage
                for (x0, y0, z0), (x1, y1, z1) in segments:
                    _create_line([x0, y0, z0], [x1, y1, z1], color, thickness, gesture_obj=select_gesture)

                # 2. Short camera-facing cross at the pivot (also serves as the click handle)
                if arm > 1e-3:
                    cross_thickness = thickness + 1.0
                    look_at_cam = getattr(sc.Transform.LookAt, "CAMERA", None) if hasattr(sc.Transform, "LookAt") else None
                    if look_at_cam is not None:
                        with sc.Transform(look_at=look_at_cam):
                            for (x0, y0, z0), (x1, y1, z1) in cross_segments:
                                _create_line([x0, y0, z0], [x1, y1, z1], color, cross_thickness, gesture_obj=select_gesture)
                    else:
                        for (x0, y0, z0), (x1, y1, z1) in cross_segments:
                            _create_line([x0, y0, z0], [x1, y1, z1], color, cross_thickness, gesture_obj=select_gesture)
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to build lines for {self.prim_path}: {e}")

    def update_transform(self, prim):
        if not HAS_KIT or not prim or not self.transform_node or not UsdGeom:
            return

        try:
            target_prim = self.get_target_xform_prim(prim)
            xformable = UsdGeom.Xformable(target_prim)
            world_xf = xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
            flat_m = [float(world_xf[r][c]) for r in range(4) for c in range(4)]
            # CRITICAL: Only set transform_node.transform if matrix ACTUALLY changed!
            # Re-assigning transform on static volumes every frame dirties the scene graph,
            # causing a 1-frame camera lag / jitter ("dançando") during viewport navigation.
            if self.cached_transform is None or flat_m != self.cached_transform:
                self.cached_transform = flat_m
                self.transform_node.transform = flat_m
        except Exception:
            pass

    def update_per_frame(self, prim, is_visible: bool = True):
        if not HAS_KIT or not prim:
            return

        if self.needs_rebuild:
            self.needs_rebuild = False
            self.rebuild_lines(prim)

        self.update_transform(prim)
        if self.transform_node:
            if is_visible != self.transform_node.visible:
                self.transform_node.visible = is_visible


class HydragonVolumeSceneHost(sc.Manipulator if (HAS_KIT and sc and hasattr(sc, "Manipulator")) else object):
    """
    Per-viewport host that places the volume overlay inside the viewport's own scene graph.

    `omni.kit.viewport.window`'s `ViewportSceneLayer` creates a single `sc.SceneView` per
    viewport, calls `add_event_delegation(...)` on it, and instantiates every factory registered
    with `RegisterScene` inside that one scene. Sharing the scene is what makes gesture
    arbitration work at all: the native selection click is a `SelectionClickGesture` carrying
    `priority = -100`, and its `_SelectionPreventer` gesture manager only prevents a gesture in
    favour of a *strictly higher* priority. `VolumeSelectGesture` declares 100, so the native
    selection is prevented instead of running a raycast pick behind the gizmo.

    The previous implementation created a private `sc.SceneView` and handed it to
    `viewport_api.add_scene_view()`. That call only forwards view/projection matrices
    (`omni.kit.widget.viewport/api.py`), so those gestures were never arbitrated at all — which
    is why `priority` had no effect on the fall-through.

    The registry contract requires `name`, `categories`, `visible` and `destroy()`. All remaining
    behaviour (registry, selection sync, notices, transforms) stays in
    `HydragonVolumeViewportOverlay`.
    """

    def __init__(self, viewport_desc: dict, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.viewport_api = viewport_desc.get("viewport_api")
        self.root_transform = None

        overlay = HydragonVolumeViewportOverlay.get_instance()
        if overlay:
            overlay._on_scene_host_created(self)

        if carb:
            carb.log_info("[hydragon.editor.core] Volume scene host created for a viewport.")

    @property
    def name(self) -> str:
        return "HydragonVolumes"

    @property
    def categories(self):
        return ("manipulator",)

    def on_build(self):
        """
        Creates the root transform for this viewport.

        Called by the `omni.ui.scene` framework while the manipulator's scene context is open,
        which is why the overlay rebuilds its per-volume transforms from here.
        """
        if not HAS_KIT or not sc:
            return
        self.root_transform = sc.Transform(visible=True)

        overlay = HydragonVolumeViewportOverlay.get_instance()
        if overlay:
            overlay._on_scene_host_built(self)

    def destroy(self):
        overlay = HydragonVolumeViewportOverlay.get_instance()
        if overlay:
            overlay._on_scene_host_destroyed(self)

        self.root_transform = None
        self.viewport_api = None

        if HAS_KIT:
            try:
                super().destroy()
            except Exception:
                pass


def _create_volume_scene_host(viewport_desc: dict):
    """Factory handed to `RegisterScene`; invoked once per viewport by the viewport layer."""
    return HydragonVolumeSceneHost(viewport_desc)


class HydragonVolumeViewportOverlay:
    """
    Editor viewport overlay that renders interactive wireframe cages for active
    Hydragon Force Volumes and Kill Volumes using `omni.ui.scene`.

    Adheres strictly to high-performance guidelines:
    - Lives inside the viewport's own `sc.SceneView` via `RegisterScene`, so its gestures are
      arbitrated against the native selection click by the scene's gesture manager.
    - Creates UI scene elements once and reuses them.
    - Updates transform matrices on frame ticks without recreating objects.
    - Only regenerates line geometry when the authored shape or extent changes.
    - Bypasses Hydra RTX BVH raytracing and leaves 0 bytes in the USD stage.
    - Fully compliant with Rule 2.B (in-memory registry, zero per-frame stage traversals).
    """

    _instance: Optional["HydragonVolumeViewportOverlay"] = None
    FRAME_ID = "hydragon.editor.core.volume_viewport_overlay"

    def __init__(self, ext_id: str = ""):
        HydragonVolumeViewportOverlay._instance = self
        self._ext_id = ext_id or self.FRAME_ID
        self._is_active: bool = False
        self._is_visible: bool = True

        #: Scene host registered through the viewport registry, plus its root transform.
        self._scene_host = None
        self._scene_host_registration = None
        self._root_transform = None

        # In-memory registry: prim_path -> VolumeOverlayEntry
        self._volumes: Dict[str, VolumeOverlayEntry] = {}
        self._selected_paths: Set[str] = set()
        self._stage_event_sub = None
        self._app_update_sub = None
        self._setting_sub = None
        self._menu_item = None
        self._action = None
        self._hotkey_registered: bool = False
        self._objects_changed_notice_key = None
        self._objects_changed_listener = None

        # Bounded defence window against the native selection's late asynchronous pick.
        self._override_path: Optional[str] = None
        self._override_deadline: float = 0.0
        self._mouse_iface = None
        self._mouse_sub = None

    @classmethod
    def get_instance(cls) -> Optional["HydragonVolumeViewportOverlay"]:
        return cls._instance

    @classmethod
    def select_volume(cls, prim_path: str):
        """
        Selects a volume prim as the result of a viewport click on its gizmo, and arms the
        bounded defence window that outlives the native selection's late asynchronous pick.

        Sharing the viewport's scene was meant to let `_SelectionPreventer` prevent the native
        `SelectionClickGesture` outright; measured behaviour shows it does not, so the pick still
        runs and its result is written a few frames later. This window wins that last write
        instead of trying to race it with a frame counter.
        """
        overlay = cls.get_instance()
        if overlay:
            overlay._select_volume_impl(prim_path)
            overlay._arm_selection_override(prim_path)

    def _arm_selection_override(self, prim_path: str) -> None:
        """Starts defending `prim_path` for SELECTION_OVERRIDE_WINDOW_S seconds."""
        self._override_path = prim_path
        self._override_deadline = time.monotonic() + SELECTION_OVERRIDE_WINDOW_S
        if carb:
            carb.log_info(
                f"[hydragon.editor.core] Selection override armed for {prim_path} "
                f"({SELECTION_OVERRIDE_WINDOW_S:g}s)."
            )

    def _override_target(self) -> Optional[str]:
        """Prim path still defended by the click override, or None when unarmed/expired."""
        if not self._override_path:
            return None
        if time.monotonic() > self._override_deadline:
            self._override_path = None
            return None
        return self._override_path

    def _enforce_selection_override(self) -> bool:
        """
        Re-asserts the clicked volume when the native pick overwrites the selection.

        Returns True when the incoming SELECTION_CHANGED was ours to correct, in which case the
        caller must not fall through to `_update_selection()` (re-entering selection handling
        would rebuild every highlight twice).
        """
        target = self._override_target()
        if target is None:
            return False

        usd_context = omni.usd.get_context() if (omni and hasattr(omni, "usd")) else None
        selection = usd_context.get_selection() if usd_context and hasattr(usd_context, "get_selection") else None
        if selection is None:
            return False

        try:
            current = selection.get_selected_prim_paths()
        except Exception:
            return False

        if current == [target]:
            # Our own write round-tripped; nothing has stolen the selection.
            self._override_path = None
            return False

        try:
            selection.set_selected_prim_paths([target], True)
            if carb:
                carb.log_info(
                    f"[hydragon.editor.core] Volume selection re-asserted over {current!r} "
                    "- the native pick landed after the gizmo click."
                )
        except Exception:
            pass
        return True

    def _select_volume_impl(self, prim_path: str):
        """Sets USD selection directly to the volume prim."""
        if not HAS_KIT:
            return

        usd_context = omni.usd.get_context() if (omni and hasattr(omni, "usd")) else None
        if usd_context and hasattr(usd_context, "get_selection"):
            try:
                usd_context.get_selection().set_selected_prim_paths([prim_path], True)
            except Exception:
                pass

    def _register_mouse_watch(self):
        """
        Subscribes to raw mouse events, read-only.

        The callback never consumes anything - it exists solely to cancel a pending selection
        override the moment the user starts a new click, so the override can never fight a
        selection the user made somewhere else.
        """
        if not HAS_KIT or not carb:
            return
        try:
            import omni.appwindow
            app_window = omni.appwindow.get_default_app_window()
            mouse = app_window.get_mouse() if app_window else None
            if not mouse:
                return
            self._mouse_iface = carb.input.acquire_input_interface()
            if self._mouse_iface:
                self._mouse_sub = self._mouse_iface.subscribe_to_mouse_events(mouse, self._on_mouse_event)
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Could not watch mouse events: {e}")

    def _unregister_mouse_watch(self):
        if self._mouse_iface and self._mouse_sub is not None:
            try:
                import omni.appwindow
                app_window = omni.appwindow.get_default_app_window()
                mouse = app_window.get_mouse() if app_window else None
                if mouse:
                    self._mouse_iface.unsubscribe_to_mouse_events(mouse, self._mouse_sub)
            except Exception:
                pass
        self._mouse_sub = None
        self._mouse_iface = None

    def _on_mouse_event(self, event) -> bool:
        """Never consumes: only clears a pending override when a new click begins."""
        try:
            if event.type == carb.input.MouseEventType.LEFT_BUTTON_DOWN:
                if self._override_path:
                    self._override_path = None
                    if carb:
                        carb.log_info("[hydragon.editor.core] Selection override cancelled by a new click.")
        except Exception:
            pass
        return False

    def startup(self):
        """Registers the viewport scene host and initializes listeners."""
        self._is_active = True
        if not HAS_KIT:
            return

        try:
            # 1. Read or initialize persistent visibility setting
            settings = carb.settings.get_settings() if carb else None
            if settings:
                if settings.get(SETTING_SHOW_VOLUMES) is None:
                    settings.set_bool(SETTING_SHOW_VOLUMES, True)
                self._is_visible = settings.get_as_bool(SETTING_SHOW_VOLUMES)
                try:
                    self._setting_sub = settings.subscribe_to_node_change_events(
                        SETTING_SHOW_VOLUMES, self._on_visibility_setting_changed
                    )
                except Exception:
                    pass

            # 2. Join the viewport's own scene graph. The viewport layer instantiates this
            #    factory per viewport, inside the SceneView it drives with
            #    add_event_delegation(), which is what makes gesture arbitration possible.
            self._register_scene_host()

            # 3. Register Viewport Eye Menu item ("Show By Type") and hotkey Shift+V
            self._register_viewport_menu_item()
            self._register_action_and_hotkey()

            # 4. Listen to USD selection and stage events
            usd_context = omni.usd.get_context() if omni.usd else None
            if usd_context and hasattr(usd_context, "get_stage_event_stream"):
                self._stage_event_sub = usd_context.get_stage_event_stream().create_subscription_to_pop(
                    self._on_stage_event, name="HydragonVolumeOverlayStageSub"
                )

            # 5. Listen for USD ObjectsChanged notices for zero-per-frame-traversal property updates
            self._register_objects_changed_notice()

            # 6. Listen to app update stream for smooth transform updates
            app = omni.kit.app.get_app() if omni.kit and omni.kit.app else None
            if app:
                self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                    self._on_app_update, name="HydragonVolumeOverlayUpdateSub"
                )

            # 7. Watch the mouse so a new click anywhere cancels a pending selection override
            self._register_mouse_watch()

            # 8. Initial discovery of existing volumes in the open stage
            self._scan_stage_volumes()
            self._update_selection()

            if carb:
                carb.log_info("[hydragon.editor.core] HydragonVolumeViewportOverlay started.")
        except Exception as e:
            if carb:
                carb.log_error(f"[hydragon.editor.core] HydragonVolumeViewportOverlay startup failed: {e}")

    def _register_scene_host(self):
        """
        Registers the per-viewport scene host with `omni.kit.viewport.registry`.

        `RegisterScene` is the supported entry point for `omni.ui.scene` content that must live
        inside the viewport's scene - the same mechanism the selection, camera, object-click,
        paint, physics and prim-manipulator scenes use.
        """
        try:
            from omni.kit.viewport.registry import RegisterScene
            self._scene_host_registration = RegisterScene(_create_volume_scene_host, self._ext_id)
            if carb:
                carb.log_info("[hydragon.editor.core] Registered the Hydragon volume scene host with the viewport registry.")
        except Exception as e:
            if carb:
                carb.log_error(f"[hydragon.editor.core] Could not register the volume scene host: {e}")

    def _unregister_scene_host(self):
        """Removes the scene host registration; the viewport layer then destroys the instance."""
        registration, self._scene_host_registration = self._scene_host_registration, None
        if registration is not None:
            try:
                registration.destroy()
            except Exception:
                pass

    def _on_scene_host_created(self, host):
        """Called by the scene host as soon as the viewport layer instantiates it."""
        previous, self._scene_host = self._scene_host, host
        if previous is not None and previous is not host and carb:
            carb.log_warn("[hydragon.editor.core] More than one viewport requested the volume overlay; following the most recent one.")

    def _on_scene_host_built(self, host):
        """Called from the host's `on_build()`, i.e. inside the viewport's scene context."""
        if host is not self._scene_host:
            return

        first_build = self._root_transform is None
        self._root_transform = host.root_transform
        if self._root_transform is not None:
            try:
                self._root_transform.visible = self._is_visible
            except Exception:
                pass
        self._repopulate_scene_graph()

        if first_build and carb:
            carb.log_info("[hydragon.editor.core] Volume scene host built inside the viewport scene.")

    def _on_scene_host_destroyed(self, host):
        """Called from the host's `destroy()`; drops every transform built for that viewport."""
        if host is not self._scene_host:
            return
        self._clear_all_volumes()
        self._root_transform = None
        self._scene_host = None

    def shutdown(self):
        """Unregisters the scene host and cleans up resources."""
        self._is_active = False
        self._stage_event_sub = None
        self._app_update_sub = None

        if self._setting_sub and carb:
            try:
                carb.settings.get_settings().unsubscribe_to_change_events(self._setting_sub)
            except Exception:
                pass
            self._setting_sub = None

        self._deregister_viewport_menu_item()
        self._deregister_action_and_hotkey()
        self._unregister_objects_changed_notice()
        self._unregister_mouse_watch()

        self._clear_all_volumes()
        self._unregister_scene_host()
        self._override_path = None

        # The host instance belongs to the viewport layer; never keep a stale reference.
        self._scene_host = None
        self._root_transform = None

        if carb:
            carb.log_info("[hydragon.editor.core] HydragonVolumeViewportOverlay shut down.")

    def toggle_volumes_visibility(self):
        """Toggles the visibility of all Hydragon volume viewport wireframes."""
        settings = carb.settings.get_settings() if carb else None
        if settings:
            curr = settings.get_as_bool(SETTING_SHOW_VOLUMES)
            settings.set_bool(SETTING_SHOW_VOLUMES, not curr)
        else:
            self.set_visible(not self._is_visible)

    def _on_visibility_setting_changed(self, item, event_type):
        if not carb:
            return
        settings = carb.settings.get_settings()
        new_val = settings.get_as_bool(SETTING_SHOW_VOLUMES)
        self.set_visible(new_val)

    def set_visible(self, visible: bool):
        """Sets visibility state on the root transform node for all registered volumes."""
        self._is_visible = visible
        if self._root_transform:
            try:
                self._root_transform.visible = visible
            except Exception:
                pass

    def _register_viewport_menu_item(self):
        """Registers 'Hydragon Volumes' toggle in the Viewport Eye menu under 'Show By Type'."""
        if not HAS_KIT:
            return
        try:
            import omni.kit.viewport.menubar.display as vp_display
            from omni.kit.viewport.menubar.core import CategoryStateItem
            display_ext = vp_display.get_instance()
            if display_ext and hasattr(display_ext, "register_custom_category_item"):
                self._menu_item = CategoryStateItem(
                    "Hydragon Volumes",
                    setting_path=SETTING_SHOW_VOLUMES,
                    hotkey_text="Shift+V",
                )
                display_ext.register_custom_category_item("Show By Type", self._menu_item)
                if carb:
                    carb.log_info("[hydragon.editor.core] Registered 'Hydragon Volumes' in Viewport 'Show By Type' menu.")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Could not register 'Hydragon Volumes' in display menu: {e}")

    def _deregister_viewport_menu_item(self):
        if not HAS_KIT or not self._menu_item:
            return
        try:
            import omni.kit.viewport.menubar.display as vp_display
            display_ext = vp_display.get_instance()
            if display_ext and hasattr(display_ext, "deregister_custom_category_item"):
                display_ext.deregister_custom_category_item("Show By Type", self._menu_item)
        except Exception:
            pass
        self._menu_item = None

    def _register_action_and_hotkey(self):
        """Registers action in omni.kit.actions.core and hotkey Shift+V in omni.kit.hotkeys.core."""
        if not HAS_KIT:
            return
        try:
            import omni.kit.actions.core as kit_actions
            action_reg = kit_actions.get_action_registry()
            if action_reg and hasattr(action_reg, "register_action"):
                self._action = action_reg.register_action(
                    "hydragon.editor.core",
                    "toggle_volume_visibility",
                    self.toggle_volumes_visibility,
                    display_name="Show/Hide Hydragon Volumes",
                    description="Toggles visibility of Hydragon Force and Kill volume overlays in Viewport.",
                )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Could not register action 'toggle_volume_visibility': {e}")

        try:
            import omni.kit.hotkeys.core as kit_hotkeys
            hotkey_reg = kit_hotkeys.get_hotkey_registry()
            if hotkey_reg and hasattr(hotkey_reg, "register_hotkey"):
                hotkey_reg.register_hotkey(
                    "hydragon.editor.core",
                    "Shift+V",
                    "hydragon.editor.core",
                    "toggle_volume_visibility",
                )
                self._hotkey_registered = True
                if carb:
                    carb.log_info("[hydragon.editor.core] Registered hotkey 'Shift+V' for Hydragon Volumes.")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Could not register hotkey 'Shift+V': {e}")

    def _deregister_action_and_hotkey(self):
        if self._hotkey_registered:
            try:
                import omni.kit.hotkeys.core as kit_hotkeys
                hotkey_reg = kit_hotkeys.get_hotkey_registry()
                if hotkey_reg and hasattr(hotkey_reg, "deregister_hotkey"):
                    hotkey_reg.deregister_hotkey("hydragon.editor.core", "Shift+V")
            except Exception:
                pass
            self._hotkey_registered = False

        if self._action:
            try:
                import omni.kit.actions.core as kit_actions
                action_reg = kit_actions.get_action_registry()
                if action_reg and hasattr(action_reg, "deregister_action"):
                    action_reg.deregister_action("hydragon.editor.core", "toggle_volume_visibility")
            except Exception:
                pass
            self._action = None

    def _register_objects_changed_notice(self):
        """Subscribes to USD ObjectsChanged notices to flag volumes for rebuild without per-frame traversal."""
        if not HAS_KIT:
            return

        try:
            usd_context = omni.usd.get_context() if omni.usd else None
            if not usd_context:
                return
            stage = usd_context.get_stage()
            if not stage:
                return

            # Usd.Notice.ObjectsChanged requires a listener with a callback
            # Use Tf.Notice.Register to listen for stage-level object changes
            if Tf and hasattr(Tf, "Notice"):
                self._objects_changed_listener = Tf.Notice.Register(
                    Usd.Notice.ObjectsChanged,
                    self._on_objects_changed,
                    stage
                )
                if carb:
                    carb.log_info("[hydragon.editor.core] Registered Usd.Notice.ObjectsChanged listener for volume overlay.")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Could not register ObjectsChanged notice: {e}")

    def _unregister_objects_changed_notice(self):
        """Removes the USD ObjectsChanged listener."""
        if self._objects_changed_listener:
            try:
                # Tf.Notice listener uses Revoke() to unregister
                if hasattr(self._objects_changed_listener, "Revoke"):
                    self._objects_changed_listener.Revoke()
                else:
                    self._objects_changed_listener = None
            except Exception:
                pass
            self._objects_changed_listener = None

    def _on_objects_changed(self, notice, sender):
        """Flags registered volumes for rebuild when their properties change (zero per-frame traversal)."""
        if not self._is_active or not HAS_KIT:
            return

        try:
            # Usd.Notice.ObjectsChanged yields Sdf.Path objects. Sdf.Path has no GetPath()
            # (that belongs to Usd.Prim), so the previous code raised AttributeError here and the
            # bare except below swallowed it, silently disabling this listener altogether.
            changed_paths = set()
            for getter_name in ("GetResyncedPaths", "GetChangedInfoOnlyPaths"):
                getter = getattr(notice, getter_name, None)
                if getter is None:
                    continue
                for path in getter():
                    changed_paths.add(str(path))

            if not changed_paths:
                return

            # Flag any registered volume whose path or sub-path changed
            for path_str, entry in self._volumes.items():
                for changed in changed_paths:
                    if changed == path_str or changed.startswith(path_str + "/"):
                        entry.needs_rebuild = True
                        break
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] ObjectsChanged handling failed: {e}")

    def _repopulate_scene_graph(self):
        """Reconstructs transform nodes for all currently registered volumes under the root transform."""
        if not HAS_KIT or not self._root_transform:
            return

        usd_context = omni.usd.get_context() if omni.usd else None
        stage = usd_context.get_stage() if usd_context else None
        if not stage:
            return

        for path, entry in list(self._volumes.items()):
            prim = stage.GetPrimAtPath(path)
            if not prim or not prim.IsValid() or not prim.IsActive():
                continue
            entry.destroy()
            try:
                with self._root_transform:
                    entry.transform_node = sc.Transform(visible=self._is_visible)
                entry.cached_transform = None
                entry.rebuild_lines(prim)
                entry.update_transform(prim)
            except Exception:
                pass

    def _on_stage_event(self, event):
        """Listens for selection changes and stage open/close/reload."""
        if not HAS_KIT:
            return

        try:
            event_type = event.type
            if hasattr(omni.usd, "StageEventType"):
                if event_type == int(omni.usd.StageEventType.SELECTION_CHANGED):
                    # The native async pick writes the selection a few frames after our click;
                    # correct it before doing anything else with the incoming selection.
                    if self._enforce_selection_override():
                        return
                    self._update_selection()
                elif event_type in (
                    int(omni.usd.StageEventType.OPENED),
                    int(omni.usd.StageEventType.ASSETS_LOADED),
                ):
                    self._scan_stage_volumes()
                    self._update_selection()
                elif event_type in (
                    int(omni.usd.StageEventType.CLOSED),
                    int(omni.usd.StageEventType.DETACHED),
                ):
                    self._clear_all_volumes()
        except Exception:
            pass

    def _scan_stage_volumes(self):
        """Discovers all Force Volumes and Kill Volumes on stage load (Rule 2.B: once on stage open)."""
        if not HAS_KIT:
            return

        usd_context = omni.usd.get_context() if omni.usd else None
        if not usd_context:
            return
        stage = usd_context.get_stage()
        if not stage:
            return

        found_paths: Set[str] = set()
        try:
            for prim in stage.Traverse():
                if not prim.IsValid() or not prim.IsActive():
                    continue
                path_str = str(prim.GetPath())
                if HydragonForceVolume.is_applied(prim):
                    found_paths.add(path_str)
                    if path_str not in self._volumes:
                        self._register_volume(prim, "Force")
                elif HydragonKillVolume.is_applied(prim):
                    found_paths.add(path_str)
                    if path_str not in self._volumes:
                        self._register_volume(prim, "Kill")
        except Exception:
            pass

        to_remove = [p for p in self._volumes if p not in found_paths]
        for p in to_remove:
            self._unregister_volume(p)

    def _update_selection(self):
        """Updates selection highlights for registered volumes and auto-discovers newly selected volumes."""
        if not HAS_KIT:
            return

        usd_context = omni.usd.get_context() if omni.usd else None
        if not usd_context:
            return

        stage = usd_context.get_stage()
        raw_sel = usd_context.get_selection().get_selected_prim_paths() if usd_context.get_selection() else []
        new_selected: Set[str] = set()

        if stage and raw_sel:
            for path in raw_sel:
                prim = stage.GetPrimAtPath(path)
                if not prim or not prim.IsValid():
                    continue

                curr = prim
                while curr and curr.IsValid() and not curr.IsPseudoRoot():
                    curr_path = str(curr.GetPath())
                    if HydragonForceVolume.is_applied(curr):
                        new_selected.add(curr_path)
                        if curr_path not in self._volumes:
                            self._register_volume(curr, "Force")
                        # If a child prim of the volume was picked (e.g. volumes), auto-elevate selection to root
                        if curr != prim:
                            usd_context.get_selection().set_selected_prim_paths([curr_path], True)
                            return
                        break
                    elif HydragonKillVolume.is_applied(curr):
                        new_selected.add(curr_path)
                        if curr_path not in self._volumes:
                            self._register_volume(curr, "Kill")
                        # If a child prim of the volume was picked, auto-elevate selection to root
                        if curr != prim:
                            usd_context.get_selection().set_selected_prim_paths([curr_path], True)
                            return
                        break
                    curr = curr.GetParent() if hasattr(curr, "GetParent") else None

        self._selected_paths = new_selected

        # Update selection state on all entries and rebuild lines if changed
        for path, entry in self._volumes.items():
            is_sel = path in self._selected_paths
            if entry.is_selected != is_sel:
                entry.is_selected = is_sel
                if stage:
                    p = stage.GetPrimAtPath(path)
                    if p and p.IsValid():
                        entry.rebuild_lines(p)

    def _register_volume(self, prim, volume_type: str):
        path_str = str(prim.GetPath())
        if path_str in self._volumes:
            return

        entry = VolumeOverlayEntry(
            prim_path=path_str,
            volume_type=volume_type,
            parent_transform=self._root_transform,
        )
        entry.is_selected = (path_str in self._selected_paths)
        entry.rebuild_lines(prim)
        entry.update_transform(prim)
        self._volumes[path_str] = entry

        if carb:
            carb.log_info(f"[hydragon.editor.core] Registered {volume_type} Volume overlay for {path_str}")

    def _unregister_volume(self, path_str: str):
        entry = self._volumes.pop(path_str, None)
        if entry:
            entry.destroy()
            if carb:
                carb.log_info(f"[hydragon.editor.core] Unregistered Volume overlay for {path_str}")

    def _clear_all_volumes(self):
        for entry in self._volumes.values():
            entry.destroy()
        self._volumes.clear()
        self._selected_paths.clear()
        if self._root_transform:
            try:
                self._root_transform.clear()
            except Exception:
                pass

    def _on_app_update(self, event):
        """
        Called every application frame:
        - Ensures SceneView is attached to the Viewport frame.
        - Synchronizes transform matrices for all registered volumes.
        - Checks for property changes in real time (e.g. user toggles volumeShape in UI).
        - Fully compliant with Rule 2.B (iterates in-memory registry, zero stage traversals).
        """
        if not self._is_active or not HAS_KIT:
            return

        # The viewport layer owns the scene host; there is nothing to update until it is built.
        if not self._root_transform:
            return

        if not self._volumes:
            return

        usd_context = omni.usd.get_context() if omni.usd else None
        stage = usd_context.get_stage() if usd_context else None
        if not stage:
            return

        dead_paths = []
        for path, entry in self._volumes.items():
            prim = stage.GetPrimAtPath(path)
            if not prim or not prim.IsValid() or not prim.IsActive():
                dead_paths.append(path)
                continue

            entry.update_per_frame(prim, is_visible=self._is_visible)

        for p in dead_paths:
            self._unregister_volume(p)
