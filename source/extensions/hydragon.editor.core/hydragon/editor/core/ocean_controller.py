# =============================================================================
# ocean_controller.py
# The spectral ocean as an entity: one patch per prim, driven on the GPU.
#
# Architecture, in the terms this engine uses:
#   * `HydragonOceanAPI` on a prim is the ENTITY.
#   * `HydragonOceanPatch` is the COMPONENT: it owns the GPU solver, the surface
#     mesh and the published texture for that one prim.
#   * `HydragonOceanSystem` is the SYSTEM: it discovers entities once, keeps them
#     in a registry for O(1) lookup, and steps them.
#
# Design decisions worth not re-litigating:
#
#   * NOTHING traverses the stage per frame. Discovery runs on startup, on PLAY
#     and when a stage notice says something relevant changed, exactly like the
#     force and kill volume systems.
#   * The surface mesh is GENERATED, not authored in the asset, because its grid
#     side comes from `ocean:resolution`. Authoring it in the asset would create a
#     second source of truth for the same number.
#   * The mesh is authored ONCE per parameter change and is never rewritten per
#     frame. The animated detail travels as a texture, so the vertex count is
#     decoupled from the wave detail and no per-frame USD write happens at all.
#     This is what keeps the ocean out of the Fabric deformable path and its
#     whole class of desynchronisation bugs.
#   * `extent` is authored from the patch size and refreshed whenever the patch
#     size changes. A stale `extent` makes Hydra cull the surface, and it culls
#     SILENTLY.
#   * The material lives in ONE stage-level `Looks` scope (`/World/Looks`), not in
#     a `Looks` hidden inside the ocean prim. In Omniverse a `Looks` scope is a
#     stage-level resource - one per stage, under the root prim - so that a prim
#     in ANY hierarchy can bind a material from it. A `Looks` nested inside an
#     asset is invisible to everything outside that asset, which is the opposite
#     of the convention.
# =============================================================================

from __future__ import annotations

import os
import time as _time
from typing import Dict, List, Optional, Tuple

try:
    import carb
    import omni.kit.app
    import omni.timeline
    import omni.usd
    from pxr import Gf, Sdf, Tf, Usd, UsdGeom, Vt
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None
    omni = None
    Gf = None
    Sdf = None
    Tf = None
    Usd = None
    UsdGeom = None
    Vt = None

from .schemas import HydragonOcean

try:
    from .ocean_fft_kernels import OceanSolver, preferred_device
    HAS_SOLVER = True
except Exception:  # noqa: BLE001 - warp or numpy missing is a supported state
    OceanSolver = None
    preferred_device = None
    HAS_SOLVER = False

#: Relative path of the generated surface group inside an ocean prim.
SURFACE_GROUP_NAME = "surface"
SURFACE_MESH_NAME = "ocean_surface"
LOOKS_SCOPE_NAME = "Looks"

#: Material name generated under `Looks`.  Kept stable so re-authoring the
#: material on a parameter change replaces it instead of accumulating copies.
#: The prim's path is appended to it, because every ocean publishes its own
#: dynamic texture and therefore needs its own material inside the shared scope.
SURFACE_MATERIAL_NAME = "ocean_surface_mat"

#: Vertices per side of the generated mesh.  Deliberately INDEPENDENT of
#: `ocean:resolution`: the FFT grid sets how much wave detail exists, the mesh
#: sets only how finely the surface is tessellated.  A 128x128 grid is already
#: 16k vertices, which is a lot of geometry for a surface whose shading detail
#: comes from a texture.
MESH_GRID = 128

#: Longest a single frame may advance the simulation, in seconds.  A hitch (a
#: stage load, a shader compile) would otherwise jump the sea forward by the
#: whole stall, which looks like a glitch rather than a slow frame.
MAX_FRAME_DELTA = 0.1

#: The engine's water material, which lives in the extension's own MDL folder.
#: The FILE name and the exported material name inside it are named separately by
#: USD, so both are pinned here and asserted against the .mdl by the tests.
WATER_MDL_MODULE = "HydragonWater"
WATER_MDL_FILE = WATER_MDL_MODULE + ".mdl"

#: The input on the water material that receives the packed surface field.
WATER_MDL_FIELD_INPUT = "surface_field"

#: What `addMdlSearchPath` returned, so it can be removed again on shutdown.
_REGISTERED_MDL_PATH: Optional[str] = None


def _log_info(message: str) -> None:
    if carb:
        carb.log_info(f"[hydragon.ocean] {message}")


def _log_warn(message: str) -> None:
    if carb:
        carb.log_warn(f"[hydragon.ocean] {message}")


def _log_error(message: str) -> None:
    if carb:
        carb.log_error(f"[hydragon.ocean] {message}")


def mdl_search_path() -> str:
    """Absolute path of the extension's own MDL folder, `<ext>/data/shaders`."""
    here = os.path.dirname(os.path.abspath(__file__))
    return os.path.normpath(os.path.join(here, "..", "..", "..", "data", "shaders"))


def water_mdl_file() -> str:
    """Absolute path of the water material itself."""
    return os.path.join(mdl_search_path(), WATER_MDL_FILE)


def register_mdl_library() -> bool:
    """Make the engine's own MDL modules resolvable by the renderer.

    `omni.mdl.neuraylib.get_neuraylib().addMdlSearchPath` is the supported call;
    its predecessor `RegisterExtensionContent` is deprecated.  The path is kept so
    `unregister_mdl_library` can remove it again.

    The file checks are not paranoia: a material whose module cannot be resolved
    renders as an unshaded surface and reports nothing about why, so the engine
    says what is missing instead of leaving a grey ocean to be interpreted.

    IDEMPOTENT, and called again from `_ensure_texture`, because nothing orders
    this extension after `omni.mdl.neuraylib`: it is started by the RTX renderer,
    and a startup that runs first would find no neuray library and lose the path
    for the whole session.
    """
    global _REGISTERED_MDL_PATH

    if _REGISTERED_MDL_PATH:
        return True

    path = mdl_search_path()
    if not os.path.isdir(path):
        _log_error(f"the MDL folder does not exist: {path}")
        return False
    module_file = water_mdl_file()
    if not os.path.isfile(module_file):
        _log_error(f"the water material is missing: {module_file}")
        return False

    try:
        import omni.mdl.neuraylib

        registered = omni.mdl.neuraylib.get_neuraylib().addMdlSearchPath(path)
    except Exception as exc:  # noqa: BLE001 - outside Kit there is no neuraylib
        _log_warn(f"MDL search path not registered ({exc}); the water material will "
                  f"not resolve in this process")
        return False

    if not registered:
        _log_error(f"the renderer refused the MDL search path {path}")
        return False

    _REGISTERED_MDL_PATH = registered
    _log_info(f"MDL search path registered: {registered}")
    return True


def unregister_mdl_library() -> None:
    """Remove the search path again, leaving no state behind for the next load."""
    global _REGISTERED_MDL_PATH

    if not _REGISTERED_MDL_PATH:
        return
    registered, _REGISTERED_MDL_PATH = _REGISTERED_MDL_PATH, None
    try:
        import omni.mdl.neuraylib

        if not omni.mdl.neuraylib.get_neuraylib().removeMdlSearchPath(registered):
            _log_warn(f"the renderer did not unregister the MDL search path {registered}")
    except Exception as exc:  # noqa: BLE001
        _log_warn(f"could not unregister the MDL search path: {exc}")


def paths_include_schema(paths: set, is_applied, stage=None) -> bool:
    """Whether any changed path IS a prim carrying `is_applied`'s schema.

    This is the half that catches the FIRST entity created in an empty scene: the
    registry is empty, so a containment test cannot see it.  Shared with the water
    body system, which had the same hole.
    """
    if not stage:
        return False
    for path in paths:
        prim = stage.GetPrimAtPath(path)
        if prim and prim.IsValid() and is_applied(prim):
            return True
    return False


def looks_scope_path(prim_path: str) -> str:
    """The stage-level `Looks` scope that a prim's material belongs in.

    `/World/Ocean` -> `/World/Looks`.  A `Looks` scope is a STAGE-level resource,
    the Omniverse convention being one per stage under the root prim, so a prim
    anywhere in any hierarchy can bind a material from it.  Hiding a `Looks`
    inside each asset does the opposite: it produces one invisible scope per
    asset, empty from the point of view of anything outside it.

    A prim that is itself a direct child of the stage root has no parent scope to
    share, so its material goes to a root-level `/Looks` instead.
    """
    parts = [part for part in prim_path.strip("/").split("/") if part]
    if len(parts) < 2:
        return "/" + LOOKS_SCOPE_NAME
    return f"/{parts[0]}/{LOOKS_SCOPE_NAME}"


class HydragonOceanPatch:
    """The GPU simulation, surface mesh and published texture for one ocean prim."""

    def __init__(self, prim):
        self._prim = prim
        self._path = str(prim.GetPath()) if prim else ""
        self._ocean = HydragonOcean(prim)
        self._solver = None
        self._parameters = None
        self._fingerprint: Optional[tuple] = None
        self._mesh: Optional[object] = None
        self._mesh_grid: int = 0
        self._texture_name: Optional[str] = None
        self._provider = None
        self._provider_retained = None
        self._simulated_time = 0.0
        self._reported_failure = False

    # -- identity -------------------------------------------------------------

    @property
    def prim(self):
        return self._prim

    @property
    def path(self) -> str:
        return self._path

    @property
    def is_enabled(self) -> bool:
        return self._ocean.is_enabled

    @property
    def texture_name(self) -> Optional[str]:
        """The `dynamic://` name this patch publishes under, if any."""
        return self._texture_name

    def describe(self) -> Dict[str, object]:
        return {
            "path": self._path,
            "enabled": self.is_enabled,
            "resolution": self._ocean.resolution,
            "patch_size": self._ocean.patch_size,
            "wind_speed": self._ocean.wind_speed,
            "simulated_time": self._simulated_time,
            "texture": self._texture_name,
            "solver": bool(self._solver),
        }

    # -- lifecycle ------------------------------------------------------------

    def _fingerprint_now(self) -> tuple:
        """Everything that forces the solver and the mesh to be rebuilt.

        `ocean:choppiness` belongs here: it is consumed by the solver, so leaving
        it out would make the attribute a control that does nothing.  `timeScale`
        does NOT belong here - it is applied per step, not baked into the solver.
        """
        return (
            self._ocean.resolution,
            round(self._ocean.patch_size, 4),
            round(self._ocean.wind_speed, 4),
            round(self._ocean.wind_direction, 6),
            round(self._ocean.significant_height, 4),
            round(self._ocean.choppiness, 4),
            self._ocean.seed,
            round(self._ocean.foam_threshold, 4),
            round(self._ocean.foam_bias, 4),
            MESH_GRID,
        )

    def refresh(self) -> bool:
        """Rebuild the solver and the mesh if, and only if, something changed.

        Returns True when the patch is usable.  The fingerprint is what keeps a
        per-frame sync from rebuilding the spectrum, re-uploading the initial
        field and re-authoring a 16k-vertex mesh on every tick: those run on an
        actual edit and at no other time.
        """
        if not HAS_KIT:
            return False

        current = self._fingerprint_now()
        if current == self._fingerprint and self._solver is not None:
            return True

        if not self._ocean.is_enabled:
            return False

        if not HAS_SOLVER:
            self._report_once("warp is not available, so no ocean can be simulated")
            return False

        try:
            parameters = self._ocean.to_parameters()
        except Exception as exc:  # noqa: BLE001 - a bad attribute value, report once
            self._report_once(f"invalid ocean parameters: {exc}")
            return False

        try:
            self._solver = OceanSolver(
                parameters,
                foam_threshold=self._ocean.foam_threshold,
                foam_bias=self._ocean.foam_bias,
            )
        except Exception as exc:  # noqa: BLE001
            self._report_once(f"could not build the simulation: {exc}")
            return False

        try:
            self._ensure_surface_mesh(parameters.patch_size)
        except Exception as exc:  # noqa: BLE001
            self._report_once(f"could not author the surface mesh: {exc}")
            return False

        try:
            self._ensure_texture()
        except Exception as exc:  # noqa: BLE001 - the ocean still simulates without it
            self._report_once(f"could not publish the surface texture: {exc}")

        self._parameters = parameters
        self._fingerprint = current
        frame_mode = "cuda-graph" if self._solver.graph_captured else "kernel-by-kernel"
        _log_info(
            f"{self._path}: ready, {parameters.resolution}x{parameters.resolution} grid, "
            f"{parameters.patch_size:.0f} m patch, device {self._solver.device}, "
            f"significant height {parameters.target_significant_height:.2f} m, "
            f"frame {frame_mode}"
        )
        # A failed capture is CORRECT but an order of magnitude slower, so a frame
        # time measured on it means nothing.  Say so rather than let the number be
        # read as the cost of the design.
        if self._solver.graph_error:
            _log_warn(
                f"{self._path}: CUDA graph capture failed, the frame runs kernel by "
                f"kernel (correct, but far slower): {self._solver.graph_error}"
            )
        return True

    def _report_once(self, message: str) -> None:
        """Report a failure once per patch.

        A per-frame callback that raises prints a full traceback on every frame:
        that pattern produced 893 MB of Kit log in a single session elsewhere in
        this repository.  Report loudly, then stay quiet until the patch is
        rebuilt.
        """
        if self._reported_failure:
            return
        self._reported_failure = True
        _log_error(f"{self._path}: {message}. This patch is disabled until it is edited.")

    def release(self) -> None:
        if self._solver is not None:
            try:
                self._solver.release()
            except Exception:  # noqa: BLE001
                pass
        self._solver = None
        self._provider = None
        self._provider_retained = None
        self._mesh = None
        self._mesh_grid = 0
        self._fingerprint = None
        self._parameters = None

    # -- per-frame ------------------------------------------------------------

    def step(self, delta_seconds: float) -> bool:
        """Advance the simulation and publish one frame of the surface."""
        if self._solver is None or not self._ocean.is_enabled:
            return False

        delta = min(max(delta_seconds, 0.0), MAX_FRAME_DELTA) * self._ocean.time_scale
        self._simulated_time += delta

        try:
            self._solver.run(self._simulated_time)
            self._publish()
        except Exception as exc:  # noqa: BLE001
            self._report_once(f"simulation step failed: {exc}")
            return False
        return True

    def _publish(self) -> None:
        """Upload the packed payload to the dynamic texture, if one exists.

        The upload is a whole-frame copy of an RGBA float texture at the FFT
        resolution. It is the one per-frame cost that scales with
        `ocean:resolution`, and it is a GPU to GPU copy, not a round trip through
        the host.
        """
        if self._provider is None or self._solver is None:
            return

        try:
            import omni.ui as ui
        except ImportError:
            return

        # The provider requires the payload to live on CUDA device 0.
        # `strict=True` so that a malformed upload raises here and is reported by
        # `_report_once`, instead of being logged and skipped by omni.ui - a
        # silently skipped upload is indistinguishable from a black ocean.
        resolution = self._solver.resolution
        array = self._solver.packed
        self._provider.set_bytes_data_from_gpu(
            array.ptr,
            [resolution, resolution],
            format=ui.TextureFormat.RGBA32_SFLOAT,
            strict=True,
        )
        # Warp arrays are freed when Python drops the last reference, and the
        # provider holds only a device pointer, so the array must be kept alive.
        self._provider_retained = array

    # -- surface --------------------------------------------------------------

    def _ensure_surface_mesh(self, patch_size: float) -> None:
        """Author the surface grid, its UVs and its extent.  Once per change."""
        if not HAS_KIT or not self._prim:
            return

        stage = self._prim.GetStage()
        if not stage:
            return

        group_path = self._path + "/" + SURFACE_GROUP_NAME
        group = UsdGeom.Xform.Define(stage, Sdf.Path(group_path))

        mesh_path = group_path + "/" + SURFACE_MESH_NAME
        mesh = UsdGeom.Mesh.Define(stage, Sdf.Path(mesh_path))

        grid = MESH_GRID
        # The stage is in centimetres and `patchSize` is in metres, so a 1000 m
        # patch spans 100000 units.  Getting this wrong does not error: it draws
        # an ocean a hundred times too small.
        half = (patch_size * 100.0) * 0.5
        step = (half * 2.0) / float(grid - 1)

        points = Vt.Vec3fArray(
            (grid * grid),
            [
                Gf.Vec3f(
                    -half + column * step,
                    0.0,
                    -half + row * step,
                )
                for row in range(grid)
                for column in range(grid)
            ],
        )
        mesh.CreatePointsAttr(points)

        counts = []
        indices = []
        for row in range(grid - 1):
            for column in range(grid - 1):
                base = row * grid + column
                counts.append(4)
                indices.extend(
                    (base, base + 1, base + grid + 1, base + grid)
                )
        mesh.CreateFaceVertexCountsAttr(Vt.IntArray(counts))
        mesh.CreateFaceVertexIndicesAttr(Vt.IntArray(indices))

        # `none` because the generated grid is already the final tessellation.
        # Catmull-Clark would smooth the flat plane and achieve nothing, and the
        # refinement level is an RTX cost, not a quality setting for this.
        mesh.CreateSubdivisionSchemeAttr(UsdGeom.Tokens.none)

        # A surface with no authored extent is culled by Hydra, silently.  The
        # bound covers the vertical displacement the texture produces, with room
        # to spare, and is refreshed only when the patch size changes.
        vertical = max(patch_size * 0.5, 1.0) * 100.0
        mesh.CreateExtentAttr(
            Vt.Vec3fArray(
                [Gf.Vec3f(-half, -vertical, -half), Gf.Vec3f(half, vertical, half)]
            )
        )

        # UVs LAST, after the topology, on purpose: a failure here must leave a
        # valid, untextured mesh rather than the half-authored one that made
        # rtx.hydra.geometrystreaming log "Failed to load geometry ... storage
        # data" for a Mesh with points but no faces.
        #
        # `CreatePrimvar` is NOT a method of `UsdGeom.Mesh`: it lives on
        # `UsdGeom.PrimvarsAPI`.  The per-vertex interpolation token in this build
        # is `UsdGeom.Tokens.vertex`; `Tokens.vertexInterpolation`, the name the
        # USD C++ docs use, does NOT exist here (measured: `dir(Tokens)` offers
        # `vertex`, `faceVaryingLinearInterpolation` and `interpolateBoundary`).
        # Both mistakes raise, and the caller only logs "could not author the
        # surface mesh".  Without `st` every vertex samples texel (0, 0), i.e. one
        # flat colour over the whole sea.
        st = []
        for row in range(grid):
            for column in range(grid):
                st.append(Gf.Vec2f(column / float(grid - 1), row / float(grid - 1)))
        UsdGeom.PrimvarsAPI(mesh.GetPrim()).CreatePrimvar(
            "st",
            Sdf.ValueTypeNames.TexCoord2fArray,
            UsdGeom.Tokens.vertex,
        ).Set(Vt.Vec2fArray(st))

        self._mesh = mesh
        self._mesh_grid = grid
        del group

    # -- texture --------------------------------------------------------------

    def _ensure_texture(self) -> None:
        """Create the dynamic texture provider and bind it into the material.

        `dynamic://<name>` is a texture URI the renderer resolves from a named GPU
        resource, so the material samples live pixels and nothing touches disk.
        The name is derived from the prim path so that two oceans in one stage
        cannot publish over each other.
        """
        if not HAS_KIT or not self._prim or self._solver is None:
            return

        try:
            import omni.ui as ui
        except ImportError:
            self._report_once("omni.ui is unavailable, so the surface cannot be shaded")
            return

        name = "hydragon_ocean_" + self._path.strip("/").replace("/", "_")
        # Retried here rather than trusted from startup: this is the first moment
        # the MDL has to exist for anything to work, and by now the renderer and
        # its neuray library are certainly up.  Idempotent.
        register_mdl_library()
        self._provider = ui.DynamicTextureProvider(name)
        self._texture_name = name
        self._bind_material(name)

    def _bind_material(self, texture_name: str) -> None:
        """Author the material that samples the dynamic surface field.

        The shader is the ENGINE's own MDL, `data/shaders/HydragonWater.mdl`, which
        reads the packed `(height, normal.x, normal.z, foam)` payload directly and
        reconstructs the normal. No stock material can: `OmniSurface` has no
        normal-map input at all (`geometry_normal_image` wants a TANGENT-space
        normal map and has no place for the height or the foam), and `OmniPBR`'s
        `normalmap_texture` has the same problem. It is registered as an MDL search
        path by `register_mdl_library`, called once from the extension's startup.

        If the module cannot be resolved the surface still renders, unshaded, and
        the renderer's own log names the MDL error - which is why the search path
        registration reports the missing file itself rather than staying silent.
        """
        from pxr import UsdShade

        stage = self._prim.GetStage()
        # One SHARED, stage-level `Looks` scope (see `looks_scope_path`), never a
        # `Looks` nested inside the ocean prim.
        looks_path = looks_scope_path(self._path)
        UsdGeom.Scope.Define(stage, Sdf.Path(looks_path))

        # Named after the prim: each ocean publishes its own dynamic texture, so
        # two oceans cannot share one material inside the shared scope.
        material_name = f"{SURFACE_MATERIAL_NAME}_{self._path.strip('/').replace('/', '_')}"
        material_path = f"{looks_path}/{material_name}"

        # The path is OURS (derived from the ocean prim), so when it already holds
        # a material bound to something else, that is an earlier version of this
        # function - a stock material bound before the engine had its own MDL.
        # USD would happily keep its inputs alongside the new ones
        # (`inputs:diffuse_texture` next to `inputs:surface_field`), and the
        # MDL-to-USD mapping would then have to explain an input the module does
        # not declare. Removing our own generated material is the clean start.
        existing = stage.GetPrimAtPath(material_path)
        if existing and existing.IsValid():
            asset_attr = existing.GetAttribute("info:mdl:sourceAsset")
            value = (
                asset_attr.Get()
                if asset_attr and asset_attr.HasAuthoredValue()
                else None
            )
            if (getattr(value, "path", "") or "") != WATER_MDL_FILE:
                stage.RemovePrim(material_path)

        material = UsdShade.Material.Define(stage, Sdf.Path(material_path))
        shader = UsdShade.Shader.Define(stage, Sdf.Path(material_path + "/Shader"))
        shader.CreateIdAttr(WATER_MDL_MODULE)
        shader.CreateImplementationSourceAttr(UsdShade.Tokens.sourceAsset)
        shader.SetSourceAsset(Sdf.AssetPath(WATER_MDL_FILE), "mdl")
        shader.SetSourceAssetSubIdentifier(WATER_MDL_MODULE, "mdl")

        # `renderType` is metadata on the input's ATTRIBUTE, not on the
        # `UsdShade.Input` wrapper - the wrapper only carries SDR metadata
        # (`SetSdrMetadata`), so calling `SetMetadata` on it raises. NVIDIA's
        # shipped scene authors the same hint on an identical `dynamic://` asset
        # input (`omni.warp-1.14.0/data/scenes/texture_mandelbrot.usda`), and it
        # is what makes the renderer and the material browser treat the asset as a
        # 2D texture. It is a hint, so failing to author it must not abort the
        # binding: report once and carry on with the material.
        field = shader.CreateInput(WATER_MDL_FIELD_INPUT, Sdf.ValueTypeNames.Asset)
        field.Set(Sdf.AssetPath(f"dynamic://{texture_name}"))
        try:
            field.GetAttr().SetMetadata("renderType", "texture_2d")
        except Exception as exc:  # noqa: BLE001
            self._report_once(f"could not author the renderType hint: {exc}")

        shader.CreateOutput("out", Sdf.ValueTypeNames.Token)

        material.CreateSurfaceOutput("mdl").ConnectToSource(shader.ConnectableAPI(), "out")
        material.CreateDisplacementOutput("mdl").ConnectToSource(
            shader.ConnectableAPI(), "out"
        )

        mesh_path = f"{self._path}/{SURFACE_GROUP_NAME}/{SURFACE_MESH_NAME}"
        mesh_prim = stage.GetPrimAtPath(mesh_path)
        if mesh_prim and mesh_prim.IsValid():
            UsdShade.MaterialBindingAPI.Apply(mesh_prim).Bind(material)

    # -- host-side query ------------------------------------------------------

    def sample_height(self, world_x: float, world_z: float) -> Optional[float]:
        """World-space water height at (x, z), or None if outside the patch.

        Serves the gameplay systems, which cannot read a GPU buffer cheaply.  The
        value comes from the same FFT as the rendered texture, downsampled to
        `HOST_GRID` per side, so the physics and the picture describe one sea and
        not two.

        The query uses the MEAN height over each host cell, which is the right
        answer for buoyancy: a body floats on the average of the surface under its
        footprint, not on the single sample nearest its origin.
        """
        if self._solver is None or self._parameters is None or not HAS_KIT:
            return None

        local = self._world_to_local(world_x, world_z)
        if local is None:
            return None

        half = self._parameters.patch_size * 0.5
        if abs(local[0]) > half or abs(local[1]) > half:
            return None

        # Local position to host-grid coordinates, clamped to the last cell so a
        # sample exactly on the edge reads a real cell instead of running off.
        side = self._solver.host_grid
        u = (local[0] / self._parameters.patch_size + 0.5) * float(side - 1)
        v = (local[1] / self._parameters.patch_size + 0.5) * float(side - 1)
        u = min(max(u, 0.0), float(side - 1))
        v = min(max(v, 0.0), float(side - 1))

        try:
            grid = self._solver.host_height_numpy()
        except Exception as exc:  # noqa: BLE001
            self._report_once(f"could not read the height grid back: {exc}")
            return None

        u0 = int(u)
        v0 = int(v)
        u1 = min(u0 + 1, side - 1)
        v1 = min(v0 + 1, side - 1)
        fu = u - u0
        fv = v - v0

        h00 = float(grid[v0, u0])
        h10 = float(grid[v0, u1])
        h01 = float(grid[v1, u0])
        h11 = float(grid[v1, u1])
        return (
            h00 * (1.0 - fu) * (1.0 - fv)
            + h10 * fu * (1.0 - fv)
            + h01 * (1.0 - fu) * fv
            + h11 * fu * fv
        )

    def _world_to_local(self, world_x: float, world_z: float) -> Optional[Tuple[float, float]]:
        """Map a world position onto the patch plane, in the prim's own frame."""
        if not HAS_KIT or not self._prim:
            return None
        prim = self._prim
        to_world = UsdGeom.Xformable(prim).ComputeLocalToWorldTransform(
            Usd.TimeCode.Default()
        )
        try:
            to_local = to_world.GetInverse()
        except Exception:  # noqa: BLE001 - a singular transform has no local frame
            return None
        point = to_local.Transform(Gf.Vec3d(world_x, 0.0, world_z))
        return (float(point[0]), float(point[2]))


class HydragonOceanSystem:
    """Discovers ocean entities once, steps them, and answers water-height queries."""

    _instance: Optional["HydragonOceanSystem"] = None

    def __init__(self):
        self._is_active = False
        self._is_simulating = False
        self._patches: Dict[str, HydragonOceanPatch] = {}
        self._sync_pending = False
        #: Paths named by the last stage notices, not yet judged.  See
        #: `_evaluate_notice_paths` for why the judgement cannot happen in the
        #: notice itself.
        self._notice_paths: set = set()
        self._last_update_time: Optional[float] = None
        self._stage_id: Optional[int] = None
        self._timeline_sub = None
        self._app_update_sub = None
        self._notice = None
        self._failures_reported = set()

    # -- plumbing -------------------------------------------------------------

    @classmethod
    def get_instance(cls) -> Optional["HydragonOceanSystem"]:
        return cls._instance

    @property
    def patch_count(self) -> int:
        return len(self._patches)

    def is_active_and_simulating(self) -> bool:
        return self._is_active and self._is_simulating and bool(self._patches)

    def startup(self) -> None:
        HydragonOceanSystem._instance = self
        self._is_active = True
        if not HAS_KIT:
            return

        try:
            timeline = omni.timeline.get_timeline_interface()
            self._timeline_sub = timeline.get_timeline_event_stream().create_subscription_to_pop(
                self._on_timeline_event, name="HydragonOceanTimeline"
            )
        except Exception as exc:  # noqa: BLE001
            _log_warn(f"could not subscribe to the timeline: {exc}")

        try:
            app = omni.kit.app.get_app()
            self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                self._on_app_update, name="HydragonOceanUpdate"
            )
        except Exception as exc:  # noqa: BLE001
            _log_warn(f"could not subscribe to the update stream: {exc}")

        self._register_objects_changed_notice()
        # Geometry is DATA: a freshly created ocean should show up without needing
        # PLAY.  Discovery itself is cheap and runs once here, never per frame.
        self._sync_patches_now()

    def shutdown(self) -> None:
        self._unregister_objects_changed_notice()
        for patch in list(self._patches.values()):
            patch.release()
        self._patches.clear()
        # Dropping the references is how every other system in this extension
        # cancels its subscriptions.
        self._timeline_sub = None
        self._app_update_sub = None
        self._sync_pending = False
        self._notice_paths = set()
        self._is_active = False
        if HydragonOceanSystem._instance is self:
            HydragonOceanSystem._instance = None

    # -- discovery ------------------------------------------------------------

    def _current_stage(self):
        if not HAS_KIT:
            return None
        try:
            return omni.usd.get_context().get_stage()
        except Exception:  # noqa: BLE001
            return None

    def _register_objects_changed_notice(self) -> None:
        """Watch stage edits so a new or modified ocean is picked up.

        The notice handler does NOT author anything: it raises a flag and the
        per-frame callback coalesces the work.  Authoring inside a notice
        re-enters the notice and is how the volume system lost a day.
        """
        stage = self._current_stage()
        if not stage or not Tf or not Usd:
            return
        try:
            self._notice = Tf.Notice.Register(
                Usd.Notice.ObjectsChanged, self._on_objects_changed, stage
            )
            self._stage_id = stage.GetStageId() if hasattr(stage, "GetStageId") else None
        except Exception as exc:  # noqa: BLE001
            _log_warn(f"could not register the stage notice: {exc}")

    def _unregister_objects_changed_notice(self) -> None:
        if self._notice is not None:
            try:
                self._notice.Revoke()
            except Exception:  # noqa: BLE001
                pass
        self._notice = None

    @staticmethod
    def notice_prim_paths(notice) -> set:
        """Prim paths named by an ObjectsChanged notice.

        A schema stamp reports `.../Ocean.apiSchemas`, which is a PROPERTY path.
        Trimming at the first dot turns it back into the prim it belongs to; the
        volume system needed the same fix for the same reason.
        """
        paths = set()
        try:
            for path in list(notice.GetResyncedPaths()) + list(
                notice.GetChangedInfoOnlyPaths()
            ):
                paths.add(str(path).split(".", 1)[0])
        except Exception:  # noqa: BLE001
            pass
        return paths

    def _paths_include_ocean(self, paths: set, stage=None) -> bool:
        """Whether any changed path IS an ocean, not merely under one."""
        return paths_include_schema(paths, HydragonOcean.is_applied, stage or self._current_stage())

    def _evaluate_notice_paths(self, paths: set, stage=None) -> bool:
        """Whether a batch of changed paths means the registry must be rebuilt.

        Deliberately NOT called from inside the notice handler.  `AddPayload`
        authors the payload ARC; the payload then composes, which is when the
        prim finally carries `HydragonOceanAPI` and its children.  At notice
        time a freshly created ocean is still a bare Xform, so judging it there
        answers "not an ocean" - and the prim stayed undiscovered until something
        else forced a resync.  Reported as: the mesh and the material only appear
        after the first PLAY.

        `stage` is injectable so a test can drive this with an in-memory stage.
        """
        if not paths:
            return False
        if self._paths_include_ocean(paths, stage):
            return True
        for registered in self._patches:
            for path in paths:
                if path == registered or path.startswith(registered + "/"):
                    return True
        return False

    def _on_objects_changed(self, notice, sender) -> None:
        """Record the changed paths and nothing else.

        `sender` is REQUIRED.  `Tf.Notice.Register` calls the handler with
        `(notice, sender)`, not with the notice alone, so declaring only `notice`
        makes every single notice raise TypeError.  That failure is quiet in the
        worst way: the handler never runs, nothing is ever discovered, and a newly
        created ocean silently has no mesh and no material.  The force and kill
        volume systems declare the same two parameters for exactly this reason.

        The paths are only RECORDED here; they are judged on the next frame by
        `_evaluate_notice_paths`, which is also the only place that reads USD.
        Recording is not authoring, so the notice stays re-entrancy-safe.
        """
        try:
            if not self._is_active:
                return
            paths = self.notice_prim_paths(notice)
            if paths:
                self._notice_paths |= paths
        except Exception as exc:  # noqa: BLE001
            key = ("notice", str(exc))
            if key not in self._failures_reported:
                self._failures_reported.add(key)
                _log_error(f"stage notice handler failed: {exc}")

    def _sync_patches_now(self) -> None:
        """Rebuild the registry from the stage.  Called on edits and on PLAY."""
        stage = self._current_stage()
        if not stage:
            return

        seen = set()
        try:
            for prim in stage.Traverse():
                if not prim.IsActive():
                    continue
                if not HydragonOcean.is_applied(prim):
                    continue
                path = str(prim.GetPath())
                seen.add(path)
                patch = self._patches.get(path)
                if patch is None:
                    patch = HydragonOceanPatch(prim)
                    self._patches[path] = patch
                patch.refresh()
        except Exception as exc:  # noqa: BLE001
            _log_error(f"ocean discovery failed: {exc}")

        for stale in [p for p in self._patches if p not in seen]:
            self._patches.pop(stale).release()

        if self._patches:
            _log_info(f"tracking {len(self._patches)} ocean patch(es)")

    # -- callbacks ------------------------------------------------------------

    def _on_timeline_event(self, event) -> None:
        if not HAS_KIT:
            return
        try:
            event_type = event.type
            if event_type == int(omni.timeline.TimelineEventType.PLAY):
                self._is_simulating = True
                # Time restarts so playback is reproducible from the same frame.
                for patch in self._patches.values():
                    patch._simulated_time = 0.0
                self._last_update_time = None
                self._sync_pending = True
            elif event_type in (
                int(omni.timeline.TimelineEventType.STOP),
                int(omni.timeline.TimelineEventType.PAUSE),
            ):
                self._is_simulating = False
        except Exception as exc:  # noqa: BLE001
            _log_error(f"timeline handler failed: {exc}")

    def _on_app_update(self, event) -> None:
        """One gated check per frame, no per-frame traversal, no per-frame authoring.

        The simulation advances whenever a patch is enabled, not only on PLAY: an
        artist needs to see the sea move while authoring it.  On PLAY the clock
        restarts so a capture is reproducible.
        """
        try:
            if not self._is_active:
                return

            # Notices are judged HERE, one frame after they arrive, never inside
            # the handler: `AddPayload` authors the arc before the payload
            # composes, so at notice time a new ocean is still a bare Xform (see
            # `_evaluate_notice_paths`).
            if self._notice_paths:
                paths = self._notice_paths
                self._notice_paths = set()
                if self._evaluate_notice_paths(paths):
                    self._sync_pending = True

            if self._sync_pending:
                self._sync_pending = False
                self._sync_patches_now()
            if not self._patches:
                return

            now = _time.perf_counter()
            previous = self._last_update_time
            self._last_update_time = now
            if previous is None:
                return
            delta = now - previous

            for patch in self._patches.values():
                patch.step(delta)
        except Exception as exc:  # noqa: BLE001
            # A per-frame callback that raises prints a traceback on EVERY frame.
            # Report once with the traceback, then stop stepping until something
            # re-enables the system, rather than filling the log.
            key = ("update", str(exc))
            if key not in self._failures_reported:
                self._failures_reported.add(key)
                import traceback

                _log_error(f"ocean update failed, stepping disabled:\n{traceback.format_exc()}")
            self._is_active = False

    # -- queries --------------------------------------------------------------

    def sample_height(self, world_x: float, world_z: float) -> Optional[float]:
        """Highest water surface at a world position, or None if no ocean covers it.

        The highest is taken so that overlapping patches behave the way a person
        expects: the visible surface is the one on top.
        """
        best: Optional[float] = None
        for patch in self._patches.values():
            if not patch.is_enabled:
                continue
            height = patch.sample_height(world_x, world_z)
            if height is None:
                continue
            if best is None or height > best:
                best = height
        return best

    def sync_now(self) -> None:
        """Force rediscovery.  Used by tests and by tools that edit the stage."""
        self._sync_patches_now()
