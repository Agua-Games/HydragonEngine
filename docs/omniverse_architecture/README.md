# Omniverse architecture notes

Platform-level knowledge about NVIDIA Omniverse Kit that this repository had to establish the hard
way. It exists so that the next person — or the next AI assistant — does not have to re-derive it
from a live editor, a 900 MB log, and a stack of `extscache` stubs.

This is **not** Hydragon's own architecture. For that, see `docs/architecture/`.

## Evidence convention

Every claim below is tagged, because in this codebase the distinction has repeatedly mattered:

- **[verified]** — reproduced here, by a probe, a test, or a quoted error. The source is named.
- **[inferred]** — follows from the API surface or shipped source but was **not** reproduced.
  Treat it as a hypothesis to confirm before building on it.

The tag matters most for anything about *rendering* or *cooking*. An API that "should" hide a prim
may not, and a collider that "should" be excluded from a query may not be. The only way to know is
to look.

Established on **Kit 110**: `omni.physx-110.1.1+110.1.0.wx64.r.cp312.u7f4`,
`omni.usd.schema.physx-110.1.1+110.1.0`, Python 3.12. Omniverse changes fast — re-verify before
relying on a specific name or flag.

## Contents

| document | covers |
|---|---|
| [viewport_drawing_and_gizmos.md](viewport_drawing_and_gizmos.md) | the three ways to draw in a viewport, how picking really works, `purpose`, hiding helpers, gesture arbitration, bounds |
| [collision_triggers_and_scene_queries.md](collision_triggers_and_scene_queries.md) | trigger volumes, the two ways to read overlap (one of them unusable), and why scene queries have no filter |
| [warp_compute_and_tile_fft.md](warp_compute_and_tile_fft.md) | Warp launch cost, the rules of `wp.tile_fft` (including a silently-wrong-results trap), and how to test GPU kernels headlessly with no editor |
| [debugging_and_tooling.md](debugging_and_tooling.md) | the live debug link to the editor, log locations, the `_build` junction, MCP servers |

## Choosing how to draw something

| what you want | approach | why |
|---|---|---|
| A panel, HUD, labels, property widgets | `omni.ui` (ImGui layer) | Full widget toolkit. Not part of the 3D scene, so it cannot be clicked in the viewport and cannot be occluded. |
| A 2D overlay drawn *over* the viewport (rulers, handles, badges) | `omni.ui.scene`, hosted via `viewport.registry.RegisterScene` | Immediate-mode drawing in viewport space. Participates in gesture arbitration. |
| Something the user must be able to **click**, and that the **native selection** must pick | **real USD geometry** (`UsdGeom.BasisCurves`, mesh, …) | Only rasterized prims reach the pick buffer. This is the whole reason the overlay was retired. |
| Something that must **never** be clickable | a prim with `visibility = "invisible"` | Not rasterized ⇒ writes nothing to the pick buffer ⇒ has no clickable area *by construction*. |
| A region for physics to test overlap against | a USD collider (`CollisionAPI`) + `PhysxTriggerAPI` + `PhysxTriggerStateAPI` | See the collision document. |

### If you take one thing from this folder

**The native viewport click is resolved by reading the pick buffer, not by raycasting.** The
selection is whatever *geometry was rasterized under the cursor*. Consequences that follow directly:

- A prim that does not render cannot be clicked, no matter what schemas it carries.
- A prim that renders *will* be clicked over the area it draws, even if it is "just a helper".
- Anything that renders but that you do not want to be clicked has to be made *not render*, or the
  selection has to be corrected after the fact (fragile — see the "selection fall-through" case in
  `viewport_drawing_and_gizmos.md`).
