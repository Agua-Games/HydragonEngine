# Authoring MDL for this engine

Platform knowledge for writing an MDL module that Kit's RTX renderer will actually compile. Every
rule here was established by reading the shipped modules in
`_build/windows-x86_64/release/kit/mdl/core/**` — or by a compile error. The errors are quoted
verbatim because the messages are the fastest way to recognise the problem again.

Established on **Kit 110.2.0**, `omni.mdl-57.0.3`, `omni.mdl.neuraylib-0.2.22`. The shipped modules
declare `mdl 1.7;`.

## How an extension exposes its own MDL **[verified]**

A module is only found if its folder is on the renderer's MDL search path. The supported call is:

```python
import omni.mdl.neuraylib
registered = omni.mdl.neuraylib.get_neuraylib().addMdlSearchPath(<absolute folder>)
omni.mdl.neuraylib.get_neuraylib().removeMdlSearchPath(registered)
```

- `addMdlSearchPath` returns the registered path, or `''` on failure. Keep it: that is the handle
  `removeMdlSearchPath` wants.
- `RegisterExtensionContent` / `UnregisterExtensionContent` still exist but are
  `@omni.kit.app.deprecated` in favour of the calls above. **[verified]** — read from
  `omni/mdl/neuraylib/scripts/extension_content.py`.
- `omni.mdl/search_paths/omniverse_exts/` is *reserved* for this API, per its own `readme.txt`.
- Kit's own roots come from the app config `/renderer/mdl/searchPaths/{required,templates}`
  (`kit/kernel/config/kit-core.json`) and that key is a semicolon-joined string. **Do not overwrite
  it** to add a folder — you would have to restate `${kit}/mdl/core/...` in full and would lose
  OmniPBR the day the kit version changes. Add a search path at runtime instead.
- Kit loads `omni.mdl.neuraylib` from the RTX renderer, so **nothing orders your extension after
  it**. A registration performed in `on_startup` may find no neuray library. Make the registration
  idempotent and call it again right before the first material that needs it. **[verified]** — the
  editor log shows `[ext: omni.mdl.neuraylib-0.2.22] ... Triggered by: omni.hydra.rtx`.

## The dialect rules that bite **[verified]**

Three assumptions that look obviously true and are not. Each produced a real compile error.

### 1. There are no multi-component swizzles

```
MDLC comp error: HydragonWater.mdl(63,26): C111 type 'float4' has no member 'yz'; did you mean 'z'?
```

`v.yz`, `v.xy`, `v.xyz` are rejected. Access components one at a time and build vectors explicitly:

```mdl
float2 slope = float2(field.y, field.z) * strength;
```

Supporting evidence: in all of `kit/mdl/core/**`, every occurrence of `.xy` / `.yz` / `.xyz` is
**inside a comment**. No shipped module swizzles.

### 2. `state::texture_coordinate` returns `float3`, not `float2`

```
MDLC comp error: HydragonWater.mdl(57,17): C147 no overloaded version of function 'float2(float3)'
```

`float3 uvw = state::texture_coordinate(0);` is how the shipped modules do it
(`core_definitions.mdl`, `gltf/pbr.mdl`). Convert explicitly:

```mdl
float3 coord = state::texture_coordinate(0);
float2 uv = float2(coord.x, coord.y);
```

### 3. Scaling a colour by a float is written `color * color(x)`

`GaussianEmissive.mdl` writes `adjusted_color * emission_intensity` only for `float3` colours.
Where the type really is `color`, the shipped code wraps the scalar:
`emission_color * color(emission_intensity)` (`OmniPBRBase.mdl`). Follow that.

## Constructs copied from shipped modules **[verified]**

Every one of these was read out of an installed module, with the file it came from. Prefer copying
one of these over inventing a form; the compiler is stricter than the language looks.

| construct | shipped source |
|---|---|
| `tex::lookup_float4(tex: t, coord: uv)` | `SimPBR.mdl` |
| `tex::lookup_float3(tex: t, coord: uv)` | `OmniPBR_ClearCoat_Opacity.mdl` |
| `tex::texture_isvalid(t) ? tex::lookup_float4(...) : float4(1)` | `SimPBR.mdl` |
| `state::texture_coordinate(i)` into a `float3` | `core_definitions.mdl` |
| `df::fresnel_layer(ior:, weight:, layer:, base:, normal:)` | `core_definitions.mdl` |
| `df::microfacet_ggx_smith_bsdf(mode:, tint:, roughness_u:, roughness_v:)` | `OmniPBRBase.mdl` |
| `df::diffuse_reflection_bsdf(roughness:, tint:)` | `core_definitions.mdl` |
| `= let { ... } in material(thin_walled:, surface:)` | `OmniEmissive.mdl` |
| `math::saturate(sqrt(1 - dot(v, v)))` to rebuild an omitted component | `OmniUe4Function.mdl` |
| `math::lerp(a, b, t)` for both `float` and `color` | `OmniSurfaceBase.mdl` |
| `anno::display_name / description / in_group / author` | `core_definitions.mdl` |

The conditional texture lookup is lazy, so an unbound `texture_2d()` never reaches the sampler —
which is how a material can take an optional mask with a harmless default.

## Binding an MDL module from USD **[verified]**

A shader prim names the module and the material inside it as two separate things:

```python
shader.CreateIdAttr("HydragonWater")
shader.CreateImplementationSourceAttr(UsdShade.Tokens.sourceAsset)
shader.SetSourceAsset(Sdf.AssetPath("HydragonWater.mdl"), "mdl")
shader.SetSourceAssetSubIdentifier("HydragonWater", "mdl")
```

Both names must match the file: `<file>.mdl` in a registered search path, exporting
`material <name>`. A texture input needs `renderType = "texture_2d"` metadata **on the input
attribute** to be recognised (see `warp_compute_and_tile_fft.md` for the `dynamic://` variant).

Note that `str(Sdf.AssetPath)` is the serialised USD form (`@HydragonWater.mdl@`) — read `.path`
instead when comparing.

## When it fails, this is what you see **[verified]**

```
[rtx.neuraylib.plugin] [MDLC:COMPILER] MDLC comp error: <path>(<line>,<col>): <message>
[omni.usd] USD_MDL (secondary thread): in LoadModule at line 349 ... 'MdlModuleId' for '<path>' is Invalid
[omni.rtx.materials] Unable to find SdrShaderNode for prim: '<material path>/Shader' with identifier: '...'
```

The surface then renders as a **constant flat colour** (measured: a dull red) with no animation —
the renderer's error material, not a shading result. Read the compiler line: it names the file, the
line and the column, and the message names the offending member. That is enough to fix it without
guessing, which is the only way to work here — there is no MDL compiler you can run outside Kit.

## A note on where MDL files live **[verified]**

`source/extensions/hydragon.editor.core/data/shaders/` holds this engine's modules. That folder is
**versioned**; `data/assets/` is not (see `.gitignore`). Anything the material needs at compile time
belongs in the versioned folder; anything it samples at runtime can live with the assets.
