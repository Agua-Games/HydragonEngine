"""
Hydragon Engine - Volume Viewport Overlay Manipulator

High-performance viewport overlay using `omni.ui.scene` to draw lightweight,
editor-only wireframe bounds for Force Volumes and Kill Volumes (Box, Sphere,
Cylinder, Plane). Zero USD stage pollution and zero RTX raytracing overhead.
"""

import math
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
    import omni.kit.viewport.utility as vp_util
    from pxr import Usd, UsdGeom, Gf, Sdf
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    ui = None
    sc = None
    cl = None
    vp_util = None
    Usd = None
    UsdGeom = None
    Gf = None
    Sdf = None

from .schemas import HydragonForceVolume, HydragonKillVolume

SETTING_SHOW_VOLUMES = "/persistent/app/viewport/displayOptions/showHydragonVolumes"


class VolumeSelectGesture(sc.ClickGesture if (HAS_KIT and sc and hasattr(sc, "ClickGesture")) else object):
    """Clean, standard ClickGesture that selects the volume prim on click."""
    def __init__(self, prim_path: str, **kwargs):
        self.prim_path = prim_path
        if HAS_KIT and sc and hasattr(sc, "ClickGesture"):
            try:
                super().__init__()
            except Exception:
                pass

    def on_ended(self):
        HydragonVolumeViewportOverlay.select_volume(self.prim_path)

    def _on_click(self, *args):
        HydragonVolumeViewportOverlay.select_volume(self.prim_path)


# Backward-compatibility aliases and fallbacks
VolumeClickGesture = VolumeSelectGesture
VolumeGestureManager = object
PreventViewportOthers = object

def _is_alt_pressed() -> bool:
    return False

def _attach_gesture(shape_obj, gesture_obj):
    if not shape_obj or not gesture_obj:
        return
    try:
        shape_obj.gestures = [gesture_obj]
    except Exception:
        pass


def _create_line(p0, p1, line_color, line_thickness, gesture_obj=None):
    """Constructs an sc.Line and attaches gesture_obj if supported."""
    if gesture_obj:
        try:
            line = sc.Line(p0, p1, color=line_color, thickness=line_thickness, intersection_thickness=6.0)
            line.gestures = [gesture_obj]
            return line
        except Exception:
            pass
    line = sc.Line(p0, p1, color=line_color, thickness=line_thickness)
    if gesture_obj:
        try:
            line.gestures = [gesture_obj]
        except Exception:
            pass
    return line


def make_color(r: float, g: float, b: float, a: float = 1.0):
    """Produces an omni.ui compatible color object or 32-bit uint color fallback."""
    if cl:
        try:
            return cl(r, g, b, a)
        except Exception:
            pass
        try:
            return cl.color(r, g, b, a)
        except Exception:
            pass
    ir = int(max(0.0, min(1.0, r)) * 255)
    ig = int(max(0.0, min(1.0, g)) * 255)
    ib = int(max(0.0, min(1.0, b)) * 255)
    ia = int(max(0.0, min(1.0, a)) * 255)
    return (ia << 24) | (ib << 16) | (ig << 8) | ir


# Color palette for volumes (Default vs Selected)
COLOR_FORCE_DEFAULT = make_color(0.18, 0.72, 1.0, 0.85)   # Cyan / Blue
COLOR_FORCE_SELECTED = make_color(0.35, 0.92, 1.0, 1.0)   # Electric Cyan
COLOR_KILL_DEFAULT = make_color(1.0, 0.25, 0.25, 0.85)    # Coral Red
COLOR_KILL_SELECTED = make_color(1.0, 0.55, 0.45, 1.0)    # Hot Neon Red


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

        if self.volume_type == "Force":
            base_hx, base_hy, base_hz = 100.0, 100.0, 100.0
            bounds_prim = prim.GetPrimAtPath("volumes/force_bounds")
            if bounds_prim and bounds_prim.IsValid():
                ext_attr = bounds_prim.GetAttribute("extent")
                if ext_attr and ext_attr.IsValid():
                    ext_val = ext_attr.Get()
                    if ext_val and len(ext_val) >= 2:
                        base_hx = max(10.0, max(abs(float(ext_val[0][0])), abs(float(ext_val[1][0]))))
                        base_hy = max(10.0, max(abs(float(ext_val[0][1])), abs(float(ext_val[1][1]))))
                        base_hz = max(10.0, max(abs(float(ext_val[0][2])), abs(float(ext_val[1][2]))))
        else:  # Kill
            bounds_prim = prim.GetPrimAtPath("volumes/kill_bounds")
            if bounds_prim and bounds_prim.IsValid():
                ext_attr = bounds_prim.GetAttribute("extent")
                if ext_attr and ext_attr.IsValid():
                    ext_val = ext_attr.Get()
                    if ext_val and len(ext_val) >= 2:
                        base_hx = max(10.0, max(abs(float(ext_val[0][0])), abs(float(ext_val[1][0]))))
                        base_hy = max(10.0, max(abs(float(ext_val[0][1])), abs(float(ext_val[1][1]))))
                        base_hz = max(10.0, max(abs(float(ext_val[0][2])), abs(float(ext_val[1][2]))))
                    else:
                        base_hx, base_hy, base_hz = 50.0, 50.0, 50.0
                else:
                    base_hx, base_hy, base_hz = 50.0, 50.0, 50.0
            elif is_sub_volume:
                base_hx, base_hy, base_hz = 50.0, 50.0, 50.0
            else:
                base_hx, base_hy, base_hz = 500.0, 50.0, 500.0

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

        # Central 3D star/diamond glyph for intuitive clicking at the volume's center pivot
        arm = min(radius * 0.3, 35.0)
        center_segments = [
            ((-arm, 0.0, 0.0), (arm, 0.0, 0.0)),
            ((0.0, -arm, 0.0), (0.0, arm, 0.0)),
            ((0.0, 0.0, -arm), (0.0, 0.0, arm)),
        ]
        d = arm * 0.5
        center_segments.extend([
            ((-d, 0.0, 0.0), (0.0, d, 0.0)),
            ((0.0, d, 0.0), (d, 0.0, 0.0)),
            ((d, 0.0, 0.0), (0.0, -d, 0.0)),
            ((0.0, -d, 0.0), (-d, 0.0, 0.0)),
            ((-d, 0.0, 0.0), (0.0, 0.0, d)),
            ((0.0, 0.0, d), (d, 0.0, 0.0)),
            ((d, 0.0, 0.0), (0.0, 0.0, -d)),
            ((0.0, 0.0, -d), (-d, 0.0, 0.0)),
        ])

        try:
            self.transform_node.clear()
            with self.transform_node:
                # 1. Outer wireframe cage
                for (x0, y0, z0), (x1, y1, z1) in segments:
                    _create_line([x0, y0, z0], [x1, y1, z1], color, thickness, gesture_obj=select_gesture)

                # 2. Central star/diamond glyph (makes selecting the volume in the viewport easy and intuitive)
                cross_thickness = thickness + 1.5
                for (x0, y0, z0), (x1, y1, z1) in center_segments:
                    _create_line([x0, y0, z0], [x1, y1, z1], color, cross_thickness, gesture_obj=select_gesture)

                # 3. Solid center pivot disk (Arc): facing camera, clean visual marker and reliable hit target
                if HAS_KIT and sc and hasattr(sc, "Arc") and select_gesture:
                    try:
                        look_at_cam = getattr(sc.Transform.LookAt, "CAMERA", None) if hasattr(sc.Transform, "LookAt") else None
                        if look_at_cam is not None:
                            with sc.Transform(look_at=look_at_cam):
                                try:
                                    disk = sc.Arc(16.0, tesselation=24, color=color)
                                except TypeError:
                                    disk = sc.Arc(16.0, color=color)
                                disk.gestures = [select_gesture]
                        else:
                            try:
                                disk = sc.Arc(16.0, tesselation=24, color=color)
                            except TypeError:
                                disk = sc.Arc(16.0, color=color)
                            disk.gestures = [select_gesture]
                    except Exception:
                        pass
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

        shape = self.read_shape(prim)
        if shape != self.cached_shape:
            self.rebuild_lines(prim)

        self.update_transform(prim)
        if self.transform_node and is_visible and not self.transform_node.visible:
            self.transform_node.visible = True
        elif self.transform_node and not is_visible and self.transform_node.visible:
            self.transform_node.visible = False


class HydragonVolumeViewportOverlay:
    """
    Editor viewport overlay that renders interactive wireframe cages for active
    Hydragon Force Volumes and Kill Volumes using `omni.ui.scene`.
    
    Adheres strictly to high-performance guidelines:
    - Creates UI scene elements once and reuses them.
    - Attached via `viewport_window.get_frame(...)` to guarantee correct viewport rendering.
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

        self._viewport_window = None
        self._scene_view = None
        self._root_transform = None

        # In-memory registry: prim_path -> VolumeOverlayEntry
        self._volumes: Dict[str, VolumeOverlayEntry] = {}
        self._selected_paths: Set[str] = set()
        # Consumable selection target to guard against native viewport raycast fall-through
        self._just_clicked_volume: Optional[str] = None
        self._fallthrough_counter: int = 0

        self._stage_event_sub = None
        self._app_update_sub = None
        self._setting_sub = None
        self._menu_item = None
        self._action = None
        self._hotkey_registered: bool = False
        self._keyboard_sub = None

    @classmethod
    def get_instance(cls) -> Optional["HydragonVolumeViewportOverlay"]:
        return cls._instance

    @classmethod
    def select_volume(cls, prim_path: str):
        """Class method to trigger selection of a volume prim."""
        overlay = cls.get_instance()
        if overlay:
            overlay._select_volume_impl(prim_path)

    @classmethod
    def lock_selection(cls, prim_path: str):
        """Backward-compatibility alias for select_volume."""
        cls.select_volume(prim_path)

    def _select_volume_impl(self, prim_path: str):
        """Selects prim_path and sets consumable guard against native viewport raycast fall-through."""
        self._just_clicked_volume = prim_path
        self._fallthrough_counter = 2

        if not HAS_KIT:
            return

        usd_context = omni.usd.get_context() if (omni and hasattr(omni, "usd")) else None
        if usd_context and hasattr(usd_context, "get_selection"):
            try:
                usd_context.get_selection().set_selected_prim_paths([prim_path], True)
            except Exception:
                pass

    def startup(self):
        """Attaches SceneView to the active Viewport and initializes listeners."""
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

            self._ensure_attached()

            # 2. Register Viewport Eye Menu item ("Show By Type") and hotkey Shift+V
            self._register_viewport_menu_item()
            self._register_action_and_hotkey()

            # 3. Listen to USD selection and stage events
            usd_context = omni.usd.get_context() if omni.usd else None
            if usd_context and hasattr(usd_context, "get_stage_event_stream"):
                self._stage_event_sub = usd_context.get_stage_event_stream().create_subscription_to_pop(
                    self._on_stage_event, name="HydragonVolumeOverlayStageSub"
                )

            # 4. Listen to app update stream for smooth transform updates & viewport sync
            app = omni.kit.app.get_app() if omni.kit and omni.kit.app else None
            if app:
                self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                    self._on_app_update, name="HydragonVolumeOverlayUpdateSub"
                )

            # 5. Initial discovery of existing volumes in the open stage
            self._scan_stage_volumes()
            self._update_selection()

            if carb:
                carb.log_info("[hydragon.editor.core] HydragonVolumeViewportOverlay started.")
        except Exception as e:
            if carb:
                carb.log_error(f"[hydragon.editor.core] HydragonVolumeViewportOverlay startup failed: {e}")

    def shutdown(self):
        """Detaches SceneView from viewport and cleans up resources."""
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

        self._just_clicked_volume = None
        self._fallthrough_counter = 0

        self._clear_all_volumes()
        self._detach_scene_view()

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
        # 1. Register Action
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

        # 2. Register Hotkey via omni.kit.hotkeys.core
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

        # 3. Fallback keyboard event subscription via carb.input
        try:
            if carb and hasattr(carb, "input"):
                import omni.appwindow
                appwindow = omni.appwindow.get_default_app_window()
                input_iface = carb.input.acquire_input_interface()
                if appwindow and input_iface:
                    keyboard = appwindow.get_keyboard()
                    if keyboard:
                        self._keyboard_sub = input_iface.subscribe_to_keyboard_events(
                            keyboard, self._on_keyboard_event
                        )
        except Exception:
            pass

    def _deregister_action_and_hotkey(self):
        # 1. Fallback keyboard sub
        if self._keyboard_sub and carb and hasattr(carb, "input"):
            try:
                import omni.appwindow
                appwindow = omni.appwindow.get_default_app_window()
                input_iface = carb.input.acquire_input_interface()
                if appwindow and input_iface:
                    keyboard = appwindow.get_keyboard()
                    if keyboard:
                        input_iface.unsubscribe_to_keyboard_events(keyboard, self._keyboard_sub)
            except Exception:
                pass
            self._keyboard_sub = None

        # 2. Hotkey
        if self._hotkey_registered:
            try:
                import omni.kit.hotkeys.core as kit_hotkeys
                hotkey_reg = kit_hotkeys.get_hotkey_registry()
                if hotkey_reg and hasattr(hotkey_reg, "deregister_hotkey"):
                    hotkey_reg.deregister_hotkey("hydragon.editor.core", "Shift+V")
            except Exception:
                pass
            self._hotkey_registered = False

        # 3. Action
        if self._action:
            try:
                import omni.kit.actions.core as kit_actions
                action_reg = kit_actions.get_action_registry()
                if action_reg and hasattr(action_reg, "deregister_action"):
                    action_reg.deregister_action("hydragon.editor.core", "toggle_volume_visibility")
            except Exception:
                pass
            self._action = None

    def _on_keyboard_event(self, event):
        if not self._is_active or not carb or not hasattr(carb, "input"):
            return True
        try:
            if event.type == carb.input.KeyboardEventType.KEY_PRESS:
                if event.key == carb.input.KeyboardInput.V:
                    input_iface = carb.input.acquire_input_interface()
                    mods = input_iface.get_global_modifier_flags(carb.input.KEYBOARD_MODIFIER_FLAG_SHIFT)
                    if bool(mods & carb.input.KEYBOARD_MODIFIER_FLAG_SHIFT):
                        if not self._hotkey_registered:
                            self.toggle_volumes_visibility()
        except Exception:
            pass
        return True

    def _ensure_attached(self) -> bool:
        """Ensures SceneView is created within the viewport frame and registered with viewport_api."""
        if not HAS_KIT or not sc or not vp_util:
            return False

        try:
            current_vp = vp_util.get_active_viewport_window()
            if not current_vp:
                return False

            # If already attached to the same window and scene_view is live, all good
            if self._viewport_window is current_vp and self._scene_view is not None:
                return True

            # If window changed or needs fresh attach, detach first
            if self._viewport_window is not None:
                self._detach_scene_view()

            self._viewport_window = current_vp

            # 1. Open the frame container on the viewport window (required for viewport UI rendering)
            frame = current_vp.get_frame(self._ext_id) if hasattr(current_vp, "get_frame") else None
            if frame:
                with frame:
                    self._scene_view = sc.SceneView()
                    with self._scene_view.scene:
                        self._root_transform = sc.Transform(visible=self._is_visible)
            else:
                self._scene_view = sc.SceneView()
                with self._scene_view.scene:
                    self._root_transform = sc.Transform(visible=self._is_visible)

            # 2. Register SceneView with the Viewport API
            if hasattr(current_vp, "viewport_api") and current_vp.viewport_api and hasattr(current_vp.viewport_api, "add_scene_view"):
                current_vp.viewport_api.add_scene_view(self._scene_view)
            elif hasattr(current_vp, "add_scene_view"):
                current_vp.add_scene_view(self._scene_view)

            # 3. Rebuild existing volumes in the new root transform
            self._repopulate_scene_graph()

            if carb:
                carb.log_info(f"[hydragon.editor.core] SceneView successfully attached to Viewport frame '{self._ext_id}'.")
            return True

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to attach SceneView to Viewport: {e}")
            self._scene_view = None
            self._root_transform = None
            return False

    def _detach_scene_view(self):
        """Removes SceneView from viewport and clears graphics."""
        if self._viewport_window and self._scene_view:
            try:
                if hasattr(self._viewport_window, "viewport_api") and self._viewport_window.viewport_api and hasattr(self._viewport_window.viewport_api, "remove_scene_view"):
                    self._viewport_window.viewport_api.remove_scene_view(self._scene_view)
                elif hasattr(self._viewport_window, "remove_scene_view"):
                    self._viewport_window.remove_scene_view(self._scene_view)
            except Exception:
                pass

        if self._scene_view:
            try:
                self._scene_view.scene.clear()
            except Exception:
                pass

        self._scene_view = None
        self._root_transform = None
        self._viewport_window = None

    def _repopulate_scene_graph(self):
        """Reconstructs transform nodes for all currently registered volumes under the root transform."""
        if not self._root_transform:
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
                    if self._just_clicked_volume:
                        target = self._just_clicked_volume
                        usd_context = omni.usd.get_context() if omni.usd else None
                        if usd_context and hasattr(usd_context, "get_selection"):
                            sel = usd_context.get_selection().get_selected_prim_paths()
                            if sel != [target]:
                                # Spurious fall-through from native viewport raycast; restore volume selection
                                usd_context.get_selection().set_selected_prim_paths([target], True)
                                return
                            else:
                                self._just_clicked_volume = None
                                self._fallthrough_counter = 0
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

        if self._fallthrough_counter > 0:
            self._fallthrough_counter -= 1
            if self._fallthrough_counter == 0:
                self._just_clicked_volume = None

        # Ensure attached to viewport (e.g. if viewport initialized after extension startup)
        if not self._ensure_attached():
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
