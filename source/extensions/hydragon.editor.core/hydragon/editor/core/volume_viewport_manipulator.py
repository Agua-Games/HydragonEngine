"""
Hydragon Engine - Volume Viewport Overlay Manipulator

High-performance viewport overlay using `omni.ui.scene` to draw lightweight,
editor-only wireframe bounds for Force Volumes and Kill Volumes (Box, Sphere,
Cylinder, Plane). Zero USD stage pollution and zero RTX raytracing overhead.
"""

import math
from typing import List, Optional, Tuple

try:
    import carb
    import omni.kit.app
    import omni.usd
    import omni.ui as ui
    from omni.ui import scene as sc
    import omni.kit.viewport.utility as vp_util
    from pxr import Usd, UsdGeom, Gf, Sdf
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    ui = None
    sc = None
    vp_util = None
    Usd = None
    UsdGeom = None
    Gf = None
    Sdf = None

from .schemas import HydragonForceVolume, HydragonKillVolume


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


class HydragonVolumeViewportOverlay:
    """
    Editor viewport overlay that renders interactive wireframe cages for active
    Hydragon Force Volumes and Kill Volumes using `omni.ui.scene`.
    
    Adheres strictly to high-performance guidelines:
    - Creates UI scene elements once and reuses them.
    - Updates transform matrices on frame ticks without recreating objects.
    - Only regenerates line geometry when the authored shape or extent changes.
    - Bypasses Hydra RTX BVH raytracing and leaves 0 bytes in the USD stage.
    """

    _instance: Optional["HydragonVolumeViewportOverlay"] = None

    def __init__(self):
        HydragonVolumeViewportOverlay._instance = self
        self._is_active: bool = False

        self._viewport_window = None
        self._scene_view = None
        self._root_transform = None

        self._target_prim_path: Optional[str] = None
        self._target_volume_type: Optional[str] = None  # "Force" or "Kill"
        self._cached_shape: Optional[str] = None
        self._cached_half_extents: Optional[Tuple[float, float, float]] = None

        self._stage_event_sub = None
        self._app_update_sub = None

    @classmethod
    def get_instance(cls) -> Optional["HydragonVolumeViewportOverlay"]:
        return cls._instance

    def startup(self):
        """Attaches SceneView to the active Viewport and initializes listeners."""
        self._is_active = True
        if not HAS_KIT:
            return

        try:
            # 1. Listen to USD selection and stage events
            usd_context = omni.usd.get_context() if omni.usd else None
            if usd_context and hasattr(usd_context, "get_stage_event_stream"):
                self._stage_event_sub = usd_context.get_stage_event_stream().create_subscription_to_pop(
                    self._on_stage_event, name="HydragonVolumeOverlayStageSub"
                )

            # 2. Listen to app update stream for smooth transform updates & viewport sync
            app = omni.kit.app.get_app() if omni.kit and omni.kit.app else None
            if app:
                self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                    self._on_app_update, name="HydragonVolumeOverlayUpdateSub"
                )

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

        self._detach_scene_view()
        self._target_prim_path = None
        self._cached_shape = None
        self._cached_half_extents = None

        if carb:
            carb.log_info("[hydragon.editor.core] HydragonVolumeViewportOverlay shut down.")

    def _attach_scene_view(self):
        """Ensures SceneView is attached to the active viewport window."""
        if not HAS_KIT or not sc or not vp_util:
            return

        if self._scene_view is not None:
            return

        try:
            vp_win = vp_util.get_active_viewport_window()
            if not vp_win or not hasattr(vp_win, "viewport_api") or not vp_win.viewport_api:
                return

            self._viewport_window = vp_win
            self._scene_view = sc.SceneView()

            # Attach to viewport API
            vp_win.viewport_api.add_scene_view(self._scene_view)

            # Create persistent root transform in the scene graph
            with self._scene_view.scene:
                self._root_transform = sc.Transform(visible=False)

        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to attach SceneView to Viewport: {e}")
            self._scene_view = None
            self._root_transform = None

    def _detach_scene_view(self):
        """Removes SceneView from viewport and clears graphics."""
        if self._viewport_window and self._scene_view and hasattr(self._viewport_window, "viewport_api"):
            try:
                self._viewport_window.viewport_api.remove_scene_view(self._scene_view)
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

    def _on_stage_event(self, event):
        """Listens for selection changes and stage open/close."""
        if not HAS_KIT:
            return

        try:
            event_type = event.type
            if hasattr(omni.usd, "StageEventType"):
                if event_type == int(omni.usd.StageEventType.SELECTION_CHANGED):
                    self._update_target_from_selection()
                elif event_type in (
                    int(omni.usd.StageEventType.CLOSED),
                    int(omni.usd.StageEventType.DETACHED),
                ):
                    self._target_prim_path = None
                    self._hide_overlay()
        except Exception:
            pass

    def _update_target_from_selection(self):
        """Resolves whether the currently selected prim is a Force or Kill Volume."""
        if not HAS_KIT:
            return

        usd_context = omni.usd.get_context() if omni.usd else None
        if not usd_context:
            return

        stage = usd_context.get_stage()
        selection = usd_context.get_selection().get_selected_prim_paths() if usd_context.get_selection() else []

        target_prim = None
        volume_type = None

        if stage and selection:
            for path in selection:
                prim = stage.GetPrimAtPath(path)
                if not prim or not prim.IsValid():
                    continue

                # Check prim and ancestors
                curr = prim
                while curr and curr.IsValid() and not curr.IsPseudoRoot():
                    if HydragonForceVolume.is_applied(curr):
                        target_prim = curr
                        volume_type = "Force"
                        break
                    elif HydragonKillVolume.is_applied(curr):
                        target_prim = curr
                        volume_type = "Kill"
                        break
                    curr = curr.GetParent() if hasattr(curr, "GetParent") else None

                if target_prim:
                    break

        if target_prim and volume_type:
            path_str = target_prim.GetPath().pathString if hasattr(target_prim.GetPath(), "pathString") else str(target_prim.GetPath())
            self._target_prim_path = path_str
            self._target_volume_type = volume_type
            self._refresh_geometry(force_rebuild=True)
        else:
            self._target_prim_path = None
            self._target_volume_type = None
            self._hide_overlay()

    def _hide_overlay(self):
        """Hides the wireframe overlay without destroying the scene graph."""
        if self._root_transform:
            try:
                self._root_transform.visible = False
            except Exception:
                pass
        self._cached_shape = None
        self._cached_half_extents = None

    def _refresh_geometry(self, force_rebuild: bool = False):
        """Constructs or updates the line geometry within sc.Transform."""
        if not HAS_KIT or not self._target_prim_path:
            self._hide_overlay()
            return

        self._attach_scene_view()
        if not self._scene_view or not self._root_transform:
            return

        stage = omni.usd.get_context().get_stage() if omni.usd and omni.usd.get_context() else None
        if not stage:
            self._hide_overlay()
            return

        prim = stage.GetPrimAtPath(self._target_prim_path)
        if not prim or not prim.IsValid():
            self._hide_overlay()
            return

        # 1. Read shape attribute
        shape = "Box"
        if self._target_volume_type == "Force":
            if prim.HasAttribute("force:volumeShape"):
                attr = prim.GetAttribute("force:volumeShape")
                if attr and attr.IsValid():
                    shape = str(attr.Get() or "Box")
            color = [0.2, 0.7, 1.0, 0.95]  # Cyan / Blue
        else:
            if prim.HasAttribute("kill:volumeShape"):
                attr = prim.GetAttribute("kill:volumeShape")
                if attr and attr.IsValid():
                    shape = str(attr.Get() or "Box")
            color = [1.0, 0.25, 0.25, 0.95]  # Red

        # 2. Read base extents from prim or child
        base_hx, base_hy, base_hz = 100.0, 100.0, 100.0
        if self._target_volume_type == "Kill":
            base_hx, base_hy, base_hz = 500.0, 50.0, 500.0

        bounds_prim = prim.GetPrimAtPath("volumes/force_bounds") if self._target_volume_type == "Force" else prim.GetPrimAtPath("volumes/kill_bounds")
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

        # 3. Only rebuild line objects if shape or extents changed (efficient!)
        if force_rebuild or shape != self._cached_shape or half_extents != self._cached_half_extents:
            self._cached_shape = shape
            self._cached_half_extents = half_extents

            segments = generate_shape_wireframe_segments(
                shape=shape,
                half_extents=half_extents,
                radius=radius,
                half_height=half_height,
            )

            # Rebuild children inside the root transform
            self._root_transform.clear()
            with self._root_transform:
                for (x0, y0, z0), (x1, y1, z1) in segments:
                    sc.Line([x0, y0, z0], [x1, y1, z1], color=color, thickness=2.0)

        # 4. Update the world transform matrix on the root transform
        self._update_transform_matrix(prim)
        self._root_transform.visible = True

    def _update_transform_matrix(self, prim):
        """Sets the 4x4 local-to-world transform matrix on self._root_transform."""
        if not HAS_KIT or not prim or not self._root_transform or not UsdGeom:
            return

        try:
            xformable = UsdGeom.Xformable(prim)
            world_xf = xformable.ComputeLocalToWorldTransform(Usd.TimeCode.Default())
            # Convert Gf.Matrix4d to flat row-major 16-element list
            flat_m = [float(world_xf[r][c]) for r in range(4) for c in range(4)]
            self._root_transform.transform = flat_m
        except Exception:
            pass

    def _on_app_update(self, event):
        """
        Called every application frame:
        - Keeps root transform synchronized when the volume is translated/rotated/scaled.
        - Checks for property changes in real time (e.g. user toggles volumeShape in UI).
        """
        if not self._is_active or not self._target_prim_path or not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage() if omni.usd and omni.usd.get_context() else None
        if not stage:
            return

        prim = stage.GetPrimAtPath(self._target_prim_path)
        if not prim or not prim.IsValid():
            self._target_prim_path = None
            self._hide_overlay()
            return

        # Check if shape changed via Property Window
        shape = "Box"
        if self._target_volume_type == "Force" and prim.HasAttribute("force:volumeShape"):
            attr = prim.GetAttribute("force:volumeShape")
            if attr and attr.IsValid():
                shape = str(attr.Get() or "Box")
        elif self._target_volume_type == "Kill" and prim.HasAttribute("kill:volumeShape"):
            attr = prim.GetAttribute("kill:volumeShape")
            if attr and attr.IsValid():
                shape = str(attr.Get() or "Box")

        if shape != self._cached_shape:
            # Shape changed! Rebuild wireframe lines immediately
            self._refresh_geometry(force_rebuild=True)
        else:
            # Just update world transform matrix
            self._update_transform_matrix(prim)
            if self._root_transform and not self._root_transform.visible:
                self._root_transform.visible = True
