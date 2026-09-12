# =============================================================================
# probe_dynamic_texture.py
# THE gate test for the ocean's rendering design, to be run INSIDE the editor.
#
# What it answers, and nothing else: can a GPU-computed buffer reach an MDL
# material as a live texture in THIS build?
#
# The mechanism is `omni.ui.DynamicTextureProvider`, which publishes a GPU
# resource under `dynamic://<name>`, plus `set_bytes_data_from_gpu` to fill it.
# NVIDIA's own `omni.warp` uses exactly this pair (`omni/warp/nodes/_impl/
# OgnTextureWrite.py`) and ships a scene that consumes it
# (`omni.warp-1.14.0/data/scenes/texture_mandelbrot.usda`), so the design rests on
# a shipped precedent - which is precisely why it must be measured here before
# the ocean is built on top of it.
#
# HOW TO RUN
#   Open the editor, then Window > Script Editor, paste this whole file, Run.
#   (Or, from a terminal: repo.bat launch hydragon.editor.kit, then paste.)
#
# WHAT YOU SHOULD SEE
#   A 6 m cube at the origin, painted with two moving colour bands.  Press F with
#   it selected to frame it.  It does NOT need PLAY.
#
# CLEANUP
#   Everything is created under /World/_HydragonProbe and nothing else is
#   touched.  Set CLEANUP = True and run again to remove it.
#
# READING THE RESULT
#   The console prints one line per step, so a failure names its own stage.  The
#   expensive failure is step 4: if the provider is created and uploaded without
#   error but the cube stays BLACK, then this build does not route `dynamic://`
#   into MDL and the ocean must fall back to a different visual strategy.
# =============================================================================

CLEANUP = False

print(f"=== dynamic texture probe: CLEANUP={CLEANUP} ===", flush=True)

import traceback  # noqa: E402

PROBE_ROOT = "/World/_HydragonProbe"


def _cleanup():
    import omni.usd

    stage = omni.usd.get_context().get_stage()
    if stage and stage.GetPrimAtPath(PROBE_ROOT).IsValid():
        stage.RemovePrim(PROBE_ROOT)
        print(f"removed {PROBE_ROOT}", flush=True)


def main():
    import numpy as np
    import omni.kit.app
    import omni.ui as ui
    import omni.usd
    import warp as wp
    from pxr import Sdf, UsdGeom, UsdShade

    if CLEANUP:
        _cleanup()
        return

    print("step 1: imports OK, initialising warp", flush=True)
    wp.init()
    device = "cuda:0" if wp.get_cuda_devices() else "cpu"
    print(f"  device = {device}, mathdx fft = {wp.config.enable_mathdx_fft}", flush=True)

    if device == "cpu":
        print(
            "  FAIL: warp found no CUDA device. `set_bytes_data_from_gpu` is given a\n"
            "  device pointer and the provider expects CUDA device 0, so a host array\n"
            "  would upload garbage. Stopping rather than reporting a misleading\n"
            "  result: the whole design rests on the GPU path.",
            flush=True,
        )
        return

    RES = 64
    print(f"step 2: building a {RES}x{RES} RGBA float payload ON THE GPU", flush=True)

    # `res` is passed in rather than closed over.  Warp does resolve free
    # variables from an enclosing function scope, but a gate test should not
    # rest on that.  Argument order matters: `wp.launch` concatenates `inputs`
    # then `outputs` and requires the total to match the kernel signature exactly
    # (warp/_src/context.py: `fwd_args.extend(inputs)` then `.extend(outputs)`).
    @wp.kernel
    def fill(res: int, phase: float, img: wp.array2d(dtype=wp.vec4)):
        i, j = wp.tid()
        inv = 1.0 / float(res - 1)
        u = float(i) * inv
        v = float(j) * inv
        r = 0.5 + 0.5 * wp.sin(6.283185307 * (u + phase))
        g = 0.5 + 0.5 * wp.sin(6.283185307 * (v - phase * 1.7))
        img[i, j] = wp.vec4(r, g, 0.15, 1.0)

    image = wp.zeros((RES, RES), dtype=wp.vec4, device=device)
    print("  allocated", flush=True)

    print("step 3: creating the DynamicTextureProvider", flush=True)
    name = "hydragon_probe"
    provider = ui.DynamicTextureProvider(name)
    print(f"  provider created for dynamic://{name}", flush=True)

    print("step 4: authoring a cube with an OmniPBR material bound to it", flush=True)
    stage = omni.usd.get_context().get_stage()
    if stage is None:
        print("  FAIL: no stage is open. Open or create one and re-run.", flush=True)
        return

    # Idempotent: a previous run may have left the root behind, and redefining
    # over it would silently keep stale attributes.
    if stage.GetPrimAtPath(PROBE_ROOT).IsValid():
        stage.RemovePrim(PROBE_ROOT)
        print(f"  removed a previous {PROBE_ROOT}", flush=True)

    UsdGeom.Xform.Define(stage, Sdf.Path(PROBE_ROOT))

    # Sized through the Cube's own `size` attribute rather than a scale xform op.
    # `UsdGeomXformable.AddScaleOp()` does NOT return an existing op: if the op is
    # already in `xformOpOrder` it RAISES, and `Cube.Define` authors one. This is
    # the trap already recorded in AGENTS.md and in
    # `volume_triggers.author_trigger_transform`, which calls ClearXformOpOrder()
    # for exactly this reason. Avoiding the op entirely is cleaner than clearing
    # it, and the default size of a Cube is 2, so 600 units is a 6 m box.
    cube = UsdGeom.Cube.Define(stage, Sdf.Path(PROBE_ROOT + "/Box"))
    cube.CreateSizeAttr(600.0)
    mesh = cube

    material = UsdShade.Material.Define(stage, Sdf.Path(PROBE_ROOT + "/Mat"))
    shader = UsdShade.Shader.Define(stage, Sdf.Path(PROBE_ROOT + "/Mat/Shader"))
    shader.CreateIdAttr("OmniPBR")
    shader.CreateImplementationSourceAttr(UsdShade.Tokens.sourceAsset)
    shader.SetSourceAsset(Sdf.AssetPath("OmniPBR.mdl"), "mdl")
    shader.SetSourceAssetSubIdentifier("OmniPBR", "mdl")

    texture_input = shader.CreateInput("diffuse_texture", Sdf.ValueTypeNames.Asset)
    texture_input.Set(Sdf.AssetPath(f"dynamic://{name}"))
    # `renderType` is metadata on the input's ATTRIBUTE, not on the
    # `UsdShade.Input` wrapper - the wrapper only forwards SDR metadata
    # (`SetSdrMetadata`), which is why `SetMetadata` does not exist on it.  Get
    # the attribute first.  NVIDIA's shipped scene does author it
    # (`omni.warp-1.14.0/data/scenes/texture_mandelbrot.usda`: `renderType =
    # "texture_2d"` on `inputs:diffuse_texture`), and it is what tells the
    # material browser and the renderer that this asset input is a 2D texture.
    # It is a UI/renderer hint and not part of the GPU path, so a failure here is
    # reported and the test continues.
    try:
        texture_input.GetAttr().SetMetadata("renderType", "texture_2d")
    except Exception as exc:  # noqa: BLE001
        print(f"  note: could not author the renderType hint ({exc}); continuing", flush=True)
    shader.CreateOutput("out", Sdf.ValueTypeNames.Token)
    material.CreateSurfaceOutput("mdl").ConnectToSource(shader.ConnectableAPI(), "out")
    UsdShade.MaterialBindingAPI.Apply(mesh.GetPrim()).Bind(material)
    print(f"  bound @dynamic://{name}@ to OmniPBR diffuse_texture", flush=True)

    # Select the probe so the camera can be framed on it with F, and so it is
    # obvious in the Stage window.  A probe nobody can see is not a test.
    try:
        omni.usd.get_context().get_selection().set_selected_prim_paths(
            [PROBE_ROOT + "/Box"], True
        )
    except Exception:
        pass
    print(
        "  the cube is selected: press F in the viewport to frame it",
        flush=True,
    )

    print("step 5: uploading one frame, then animating for ~20 s", flush=True)
    state = {"phase": 0.0, "frames": 0, "sub": None}

    def upload(phase):
        wp.launch(fill, dim=(RES, RES), inputs=[RES, phase], outputs=[image], block_dim=64)
        # `strict=True` on purpose: the default logs a malformed upload and
        # SILENTLY SKIPS it, which would show up as a black cube and be misread as
        # "dynamic:// does not reach MDL".  A gate test must not be able to lie.
        provider.set_bytes_data_from_gpu(
            image.ptr,
            [RES, RES],
            format=ui.TextureFormat.RGBA32_SFLOAT,
            strict=True,
        )

    upload(0.0)
    print("  first upload done - THE CUBE SHOULD NOW BE COLOURED, NOT BLACK", flush=True)

    app = omni.kit.app.get_app()

    def on_update(event):
        state["phase"] += 0.004
        state["frames"] += 1
        try:
            upload(state["phase"])
        except Exception:
            print("  upload FAILED mid-animation:", flush=True)
            print(traceback.format_exc(), flush=True)
            state["sub"] = None
            return
        if state["frames"] % 300 == 0:
            print(
                f"  {state['frames']} frames uploaded, still animating "
                f"(phase {state['phase']:.2f})",
                flush=True,
            )

    state["sub"] = app.get_update_event_stream().create_subscription_to_pop(
        on_update, name="HydragonDynamicTextureProbe"
    )
    # Keep the provider and the device array alive: the provider holds only a
    # device pointer, and a collected warp array would free the pixels under it.
    globals()["_probe_keepalive"] = (provider, image, state)

    print(
        "=== PASS so far: no exception. NOW LOOK AT THE VIEWPORT. ===\n"
        "  coloured and animating -> the dynamic texture path works; the ocean can use it.\n"
        "  created without error but BLACK -> the build does not route dynamic:// into\n"
        "  MDL. Stop here and report it; the ocean needs a different visual strategy.\n"
        f"  To stop: set CLEANUP = True and re-run. /World/_HydragonProbe is the only\n"
        "  thing this script created.",
        flush=True,
    )


try:
    main()
except Exception:
    print("=== PROBE FAILED ===", flush=True)
    print(traceback.format_exc(), flush=True)
