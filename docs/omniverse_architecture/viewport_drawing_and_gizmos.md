# Drawing in the viewport: gizmos, manipulators, visual helpers

Three layers exist. They are not interchangeable, and picking the wrong one is what turns a
half-day task into a week.

## 1. The three layers

| layer | API | draws on | clickable? | occluded by geometry? | best for |
|---|---|---|---|---|---|
| **ImGui** | `omni.ui` windows / `omni.ui.Window` | the UI pass, on top of the viewport | UI only — viewport clicks never reach it | no | panels, HUD, property editors, labels anchored to a screen position |
| **Scene 2D** | `omni.ui.scene` (`sc.*`) inside a `ViewportSceneLayer` | the viewport, after the 3D pass | only via *gestures* you register and arbitrate | no | measuring tools, drag handles, guides, transient overlays |
| **Real USD geometry** | `UsdGeom.BasisCurves` / meshes authored into the stage | the 3D scene, via Hydra | **yes** — the native selection picks it like any prim | yes | things the user should select, and anything that must line up exactly with the world |

**[verified]** All three are available in Kit 110. `omni.ui.scene` is a distinct module that resolves
to `omni.ui_scene`; it is not a submodule of `omni.ui` in the usual sense.

### Which one, in one line each

- Need a **button**? ImGui.
- Need a **drag handle** that must not be confused with scene geometry? Scene 2D.
- Need the user to **select the thing** with the normal click? Real geometry. There is no way to
  make the native selection target something that is not rendered.

## 2. How the native viewport click actually resolves

This is the single most consequential fact in this folder.

**[verified]** `SelectionManipulatorItem.__request_pick()`
(`omni.kit.viewport.window/manipulator/selection.py`) calls `viewport_api.request_pick()`, which
reads the **hydra-texture pick buffer** — *not* an RTX raycast. `useRaycastQuery = true` in the
viewport.window config only affects `perform_raycast_query`, i.e. double-click set-COI
(`manipulator/object_click.py`) and camera focal distance. It does **not** change what a single
click selects.

What follows, and each of these was measured:

- **[verified]** The pick is a hit on **rasterized pixels**, so a curve's clickable area is its
  *rasterized tube width*. A hairline wireframe is nice to look at and hard to click. Tune the
  width deliberately.
- **[verified]** `purpose = "guide"` / `"proxy"` prims are **not rasterized by default**, so they
  are not in the pick buffer and are **not clickable**. Only `purpose = "default"` (and `"render"`)
  are displayed by default. See §4.
- **[verified]** The readback is **asynchronous** — one frame. The selection steal lands ~24 ms
  (≈1 frame) after your own write. Any "correct the selection afterwards" hack must therefore be a
  *bounded window*, never a same-frame write. See §3.

### The selection fall-through case (what not to build)

Clicking a Scene-2D gizmo selected the gizmo, then the selection jumped to the geometry *behind* it.
Full root cause, in causal order:

1. Our gesture and the native `SelectionClickGesture` both act on the click, and cross-`SceneView`
   gesture arbitration does not happen (§5), so prevention never occurred.
2. The native pick then resolved asynchronously and wrote the first **geometry** hit — our overlay
   is not geometry, so it wrote the prim behind.

What did **not** work: gesture `priority` (never consulted across scenes), hosting inside the
viewport scene via `RegisterScene` (verified working, still lost), `disable_selection()` (fails
silently), and a 2-frame "fall-through counter".

What did work: a **bounded override window** (0.5 s) that re-asserts the selection, armed *after*
the selection is written and cancelled on the next mouse-down — with the subtlety that it must
**not** disarm on the echo of our own write, because `omni.usd` delivers stage events one update
late and the echo arrives *after* arming.

**The lesson is not "here is a working hack".** It is: this is what you inherit by choosing a
drawing layer that cannot be picked. The overlay was later retired in favour of real geometry, and
the entire class of problem disappeared *by construction*.

## 3. `purpose` — what it does, and what it does not

| does | does not |
|---|---|
| Feed the render settings' `includedPurposes` (`usdRender/schema.usda`; the viewport's switches are `/app/hydra/displayPurpose/{guide,proxy,render}`) | Change **appearance**. A `purpose = "guide"` `UsdGeom.Cube` still rasterizes as a solid shaded mesh. |
| Exclude a prim from the **final render** | Hide a shaded Gprim in the viewport |
| Act as a *semantic* marker ("this is an editor helper") | Act as a *hiding* mechanism |

**[verified]** `purpose = "guide"` did **not** hide a shaded Gprim in the artist's viewport, tested
repeatedly. NVIDIA's own code assumes it does (`omni.physx/scripts/deformableUtils.py` hides
collision meshes that way) — that assumption is only valid when the viewport's display-purpose
filter happens to have `guide` switched off.

So treat **look** and **semantics** as orthogonal: `BasisCurves` for the look, `purpose` for the
meaning. Do not try to hide something with `purpose`.

**[inferred]** NVIDIA's convention *is* `purpose = "guide"` for editor helpers — `omni.physx.demos`
authors trigger volumes that way, and `omni.curve.manipulator` ships
`persistent.exts."omni.curve.manipulator".creation.purpose = "guide"`. It is the right *semantic*
choice; it just is not a hiding mechanism.

## 4. Hiding a helper prim

| mechanism | verdict |
|---|---|
| `purpose = "guide"` | **[verified] rejected.** Does not hide a shaded Gprim; relies on the user's viewport filter. |
| `primvars:isVolume` | **[verified] rejected.** Removes the shaded *surface* (an `omni.kit.property.geometry` shading hint, RTX-only) but the prim **keeps writing to the pick buffer**, so it still steals clicks over an area it no longer draws. |
| `visibility = "invisible"` | **[verified] correct.** Not rasterized ⇒ nothing in the pick buffer ⇒ no clickable area at all. |
| `SetActive(False)` | Destroys the prim. Use for entity destruction, not for a helper you still need. |

**[verified]** An `invisible` prim is **still cooked by PhysX** and still reports overlaps. This was
not established by a probe but by an artist-facing bug: an invisible trigger collider produced
trigger reports and wrongly won the level, which is only possible if PhysX cooked it. So
"invisible" is safe for a physics-only prim, and it is the mechanism to use when the visual and the
collider are **two separate prims** — which is the pattern to aim for anyway.

## 5. Gesture arbitration in `omni.ui.scene`

**[verified]** Arbitration is **per scene graph**. The `GestureManager` docstring says "typically
each scene has a default manager", and cross-`SceneView` prevention does not happen. So declaring a
`priority` on a gesture inside your own private `SceneView` has **no effect** on the native
selection.

**[verified]** `viewport_api.add_scene_view()` only forwards view/projection matrices. It registers
no gestures, no widget parent, and no `GestureManager`, so a private `sc.SceneView` created that way
**never participates in arbitration at all**.

To be arbitrated, register with `omni.kit.viewport.registry.RegisterScene(factory, factory_id)`.
`ViewportSceneLayer` then instantiates every registered factory inside the **one** `SceneView` it
drives, with `add_event_delegation(...)` — all scenes share one arbiter. Same pattern as
`omni.kit.manipulator.prim.core`, `omni.paint.system.core`, `omni.physx.supportui`,
`omni.usdphysics.ui`, `omni.curve.manipulator`, and `omni.kit.viewport.window` itself.

The `RegisterScene` contract, all points verified by breaking them:

- `factory(viewport_desc)` is called **once per viewport**.
- `viewport_desc` = `{usd_context_name, layer_provider, viewport_api}` — there is **no**
  `viewport_window` key.
- The returned object **must** expose `visible`, `name`, `categories`, `destroy()`. Missing any one
  makes `ViewportSceneLayer` log `Error loading <factory>` and the overlay silently never renders.
- `on_build()` is called automatically at least once, with the scene context already open.
- Lifetime = the object returned by `RegisterScene(...)`. `handle.destroy()` unregisters, and
  `__del__` does too — so hold the reference.

Also **[verified]**: `omni.ui.scene` has **no** `consume()` / `stop_propagation`. Conflict resolution
is `GestureManager.should_prevent()` plus an ad-hoc `priority` attribute on the gesture. And the
native selection uses `SelectionClickGesture` (`omni.kit.manipulator.selection`) with
`priority = -100`; its `_SelectionPreventer` only yields to a **strictly higher** priority.

## 6. Bounds: `UsdGeom.BBoxCache`

All of the following is **[verified]** by measurement, and every one of them has bitten us.

- Signature in this build: `BBoxCache(time, includedPurposes[, useExtentsHint[, ignoreVisibility]])`.
  `includedPurposes` is **required** and the arguments are **positional**.
- **A default-purpose `BBoxCache` excludes `guide` and `proxy` geometry.** `guide` can be opted into
  by listing it. Several Kit extensions (`physicsViewportOverlayManager`, `stage export_utils`,
  `object_click`) pass `includedPurposes=[Tokens.default_]`, so **guide prims are invisible to
  those caches**.
- The token is `Tokens.default_` — with a **trailing underscore**. `Tokens.default` does not exist.
- **`ignoreVisibility=True` is mandatory** when deriving a size from a bound. With the default
  (`False`) an `invisible` child contributes nothing and the range comes back **empty** — i.e. a
  visibility toggle would collapse the measurement to zero.
- **A long-lived cache returns STALE bounds after a geometry edit.** Measured: authoring an
  `extent` had no effect on a reused cache; a fresh cache reflected it immediately. Build a fresh
  cache (or `Clear()`) whenever geometry may have changed.
- **An authored `extent` attribute OVERRIDES** the points+widths computation, and a *stale* authored
  `extent` silently lies (geometry scaled ×3, bound still reported the old size). `useExtentsHint`
  does not affect this — that governs `extentsHint` *metadata* on Xforms, not the Boundable
  `extent` attribute.
- A childless / geometry-less parent yields an **empty** range. Always guard.
- A **flat** (zero-thickness) bound is **not** reported as empty — a Plane is measurable.

### Frames (this is where the bugs live)

Measured on `/A` (scale 3) → `/A/V` (scale 5) → cube of side 60:

| call | result | frame |
|---|---|---|
| `ComputeWorldBound` | 900 | world |
| `ComputeLocalBound` | 300 | parent (own transform applied, ancestors excluded) |
| `ComputeRelativeBound(prim, prim)` | 60 | **the prim's own frame** (own transform excluded) |

`ComputeRelativeBound(prim, prim)` is the frame a collider wants, because the collider inherits the
prim's own transform.

**And the trap that follows:** a size derived from a bound is usually written back into the geometry
that the bound is computed from, making the data flow a *cycle*. The recovery rule must therefore be
a **fixed point** of the composition. `max(half_extents)` **is** one. The circumscribed
half-diagonal is **not**: a sphere generated as three orthogonal circles has a cube-shaped AABB of
side 2r, so its half-diagonal is r·√3 — a sphere grown ×1.732 per sync (×27 in 6 syncs), silently,
every frame. Do not "improve" such a rule without re-checking convergence.

## 7. `UsdGeomXformable::AddXformOp` is not idempotent

**[verified]** `AddTranslateOp()` / `AddScaleOp()` do **not** hand back an existing op. If the op is
already in `xformOpOrder` they **raise**:

```
The xformOp 'xformOp:translate' already exists in xformOpOrder [[xformOp:translate, xformOp:scale]].
```

(`usdGeom/xformable.cpp:173`.) Only the internal `_AddXformOp(..., foundExistingOp)` variant
tolerates it.

This matters for any **generated** prim, because generated prims persist in the stage layer: every
re-authoring pass is a second pass. It fires on an extension reload, a size edit, or any resync —
it is **not** reload-specific. Fix: `ClearXformOpOrder()` first; `Add*Op` reuses the existing
attributes, so nothing accumulates in the layer.

## 8. `UsdGeom.BasisCurves` imaging rules

From `usdGeom/schema.usda`, and the reason a wireframe looked wrong before it looked right:

- **widths only → TUBES**, radius = width/2, view-independent. **This is what a gizmo wants.**
- **widths + normals → RIBBONS**: flat, oriented by the normals, and they **vanish edge-on**. Do not
  use ribbons for a gizmo, however much "thin line" sounds like the goal.
- `type` defaults to **`cubic`**. Set `UsdGeom.Tokens.linear` explicitly for straight edges.
- **Schema validation: `linear` + `nonperiodic` requires `curveVertexCounts[i] > 2`.** A 2-vertex
  linear nonperiodic curve is **invalid**. USD is lenient enough to render it anyway, which is why
  this bug survives in existing assets.
- Cubic `basis`: bezier vstep 3; catmullRom / bspline vstep 1 (vstep only matters for cubic).

## 9. There is no way to consume a viewport mouse click

**[verified]** `get_mouse_clicked_event_stream` **does not exist** in Kit 110 / Viewport Next. For
raw mouse input use `carb.input.acquire_input_interface().subscribe_to_mouse_events(...)` (return
`True` to consume). `carb.events.IEventStream.create_subscription_to_pop(fn, order=0, name='')`
supports ordering (lower runs first; negatives allowed), and `IEvent.consume()` stops propagation.

**[verified]** `omni.kit.viewport.utility.disable_selection(viewport_or_window, disable_click=True)`
exists and returns an RAII handle, but it **fails silently** when the layer is not found:
`_DisableViewportWindowLayer` has no `else` branch when `_find_viewport_layer(name, category)`
returns `None`, and it only toggles `visible`. Treat it as a timing-dependent workaround, not a fix.

## 10. Prim discovery: do not identify entities by a mechanism schema

Not a rendering fact, but it belongs with the gizmo work because it was found there.

**[verified]** A generated helper collider carrying `PhysxTriggerAPI` was discovered by an unrelated
system as a *game entity*, because that system accepted the presence of the mechanism schema as an
entity marker. It defaulted to "level complete" and walking into a volume won the level.

**Rule:** a mechanism schema says what a prim **does** ("this collider reports instead of
blocking"), never what it **is**. Identify entities by their own schema. Before adding any new
schema-carrying prim to the stage, read the existing discoverers and check how they decide.
