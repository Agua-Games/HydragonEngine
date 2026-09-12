# Collision: triggers, scene queries, and the traps

Everything here was established while converting gameplay volumes from a Python overlap test to real
PhysX colliders. Two of the traps are not in any NVIDIA documentation and cost a full debugging
cycle each.

`omni.physx` version: `110.1.1+110.1.0`. Bindings file cited throughout:
`extscache/omni.physx-*/omni/physx/bindings/_physx.pyi`.

## 1. Making a prim an overlap-reporting volume

Three schemas, three different jobs — apply all three:

| schema | attributes | job |
|---|---|---|
| `UsdPhysics.CollisionAPI` | `physics:collisionEnabled` | Makes the Gprim a **collider** at all. Without it nothing else matters. |
| `PhysxSchema.PhysxTriggerAPI` | *none* — a pure marker | Says "**report** overlaps, do not **resolve** them" — the collider stops pushing things. |
| `PhysxSchema.PhysxTriggerStateAPI` | `physxTrigger:triggeredCollisions` (a relationship) | The **readable record** of what is currently inside. See §4. |

**[verified]** `PhysxTriggerStateAPI` is needed if you want to *poll* overlap; NVIDIA's own
`omni.physx.demos` `TriggerStateAPIDemo.py` applies it alongside the other two.

Forces and impulses from such a volume must be applied at the rigid body's **true simulated centre
of mass** (`physx_iface.get_rigidbody_transformation(path)["position"]`), never at the world origin.

## 2. Reading overlap: two APIs, and only one of them works

| | report subscription | state relationship |
|---|---|---|
| API | `subscription = sim_iface.subscribe_physics_trigger_report_events(fn, stage_id=0, prim_id=0)` | `PhysxTriggerStateAPI(prim).GetTriggeredCollisionsRel().GetTargets()` |
| delivers | `TriggerEventData` with **opaque integers** | **real `SdfPath`s** |
| model | enter / leave **edges** | current **snapshot** |
| usable? | **NO** — see §3 | yes |

**[verified] A body already inside when the simulation starts never produces an ENTER report, but it
IS present in the first snapshot.** So a snapshot is also the *semantically* better model — it is not
only a workaround. Derive enter/leave by differencing consecutive snapshots, and this failure mode
disappears for free.

## 3. TRAP: trigger report ids are Fabric handles, not encoded `SdfPath`s

This is the one to remember, because the API surface actively misleads you.

```python
def subscribe_physics_trigger_report_events(
    self,
    trigger_report_fn: typing.Callable[[TriggerEventData], None],
    stage_id: int = 0,
    prim_id: int = 0,
) -> int
```

and `TriggerEventData` carries:

| field | declared type | docstring |
|---|---|---|
| `event_type` | `TriggerEventType` | enter or leave |
| `other_body_prim_id` | `int` | "USD Path of the body containing the other collider prim entering trigger" |
| `other_collider_prim_id` | `int` | "USD Path of other prim entering trigger" |
| `trigger_body_prim_id` | `int` | "USD Path of the body containing the collider prim representing the trigger" |
| `trigger_collider_prim_id` | `int` | "USD Path of prim representing the trigger" |

**The docstrings say "USD Path". They are not.** They are opaque integer ids, and in this build they
are **Fabric handles**.

### Why the obvious decoding is wrong

The documented `SdfPath` encoding in this build is a **pair** of ints:

- `SimulationEvent.CONTACT_FOUND` documents `int2` fields and tells you to call
  `PhysicsSchemaTools.decodeSdfPath`.
- `PhysxSceneQuery.overlap_shape(meshPath0: int, meshPath1: int, ...)` wants a path "encoded into
  **two integers**. Use `PhysicsSchemaTools.encodeSdfPath`".

`TriggerEventData` gives you **one** int, and `PhysicsSchemaTools` (in
`omni.usd.schema.physx-*/pxr/PhysicsSchemaTools/`) only exposes `intToSdfPath`, `sdfPathToInt`,
`encodeSdfPath`, `decodeSdfPath` — no Fabric bridge. There is **no Python binding for
`omni::fabric::handleToSdfPath` anywhere in the installed extscache** (verified by searching every
`.pyi`).

### The evidence

`extscache/omni.physx-*/…/OgnPhysXOnTriggerCollider.cpp` — the Omniverse graph node for trigger
colliders — resolves the very same fields with the **Fabric** helpers:

```cpp
// :254, :256 - reading a trigger report
omni::fabric::handleToSdfPath(tdata.triggerColliderPrimId)
omni::fabric::handleToSdfPath(tdata.otherColliderPrimId)
// :134 - writing one
omni::fabric::sdfPathToHandle(primPath)
```

### The symptom, so you recognise it

Decoding with `PhysicsSchemaTools.intToSdfPath(single_int)` returns a **non-empty but meaningless
path**. Non-empty matters: a `if other_path:` sanity guard does **not** catch it. The consumer then
asks PhysX about a prim that does not exist, once per step, forever:

```
[Error] [omni.physx.plugin] SimulationInterface function could did not locate any objects at the specified path.
[Error] [omni.physx.plugin] Error executing getRigidBodyTransformation.
```

At ~50 ms intervals this is a log flood, and the feature silently does nothing.

### The fix

Stop decoding. Poll the **state relationship** instead, which yields real `SdfPath`s:

```python
state_api = PhysxSchema.PhysxTriggerStateAPI(trigger_prim)
targets = {str(t) for t in state_api.GetTriggeredCollisionsRel().GetTargets()}
```

**But note the next trap**, which this does not by itself solve: `targets` names **colliders**, while
the rest of the engine usually indexes **rigid bodies**. The two differ by a level in the common
case, so the match must be a tolerant prefix test in *both* directions, and the path the registry
already knows is the one to act on. Looking the entity up in the engine's own registry — rather than
resolving an id into a path — is what removes the need to decode anything at all.

## 4. Scene queries have no filter, and that is a design constraint

`omni.physx`'s `PhysxSceneQuery` interface, in full for what matters here:

| call | signature |
|---|---|
| `raycast_all` | `(origin: Float3, dir: Float3, distance: float, reportFn, bothSides: bool = False) -> bool` |
| `raycast_any` | `(origin: Float3, dir: Float3, distance: float, bothSides: bool = False) -> bool` |
| `raycast_closest` | `(origin: Float3, dir: Float3, distance: float, bothSides: bool = False) -> dict` |
| `overlap_box` | `(halfExtent: Float3, pos: Float3, rot: Float4, reportFn, anyHit: bool = False) -> int` |
| `overlap_sphere` | `(radius: float, pos: Float3, reportFn, anyHit: bool = False) -> int` |
| `overlap_shape` / `overlap_mesh` | `(pathInt0: int, pathInt1: int, reportFn, anyHit: bool = False) -> int` |

(**Note the argument orders differ** — `overlap_box` takes the extent *before* the position;
`overlap_sphere` takes the radius *before* the position. Also `raycast_all` returns a `bool` while the
overlaps return a hit **count**.)

**[verified] There is no group, channel, or filter argument anywhere in that list**, and
`_physx.pyi` exposes **no `PxQueryFilterCallback`** at all. The only collision-group entry in the
bindings is `SETTING_SHOW_COLLISION_GROUPS_WINDOW`, which is a debug UI setting.

**Consequence:** even if `UsdPhysics.CollisionGroup` / `physics:filteredGroups` could filter a PhysX
query, there is **no way to name a group from this API**. `omni.physx`'s own
`docs/dev_guide/scene_queries.rst` documents no filtering either.

**So: filter in your reporting callback.** Return `True` to continue (do not act on the hit), or
`False` to abort the traversal early. Keep a set of the paths you want to ignore — an `O(1)` lookup,
maintained when you author the prims, not per frame.

### Collision groups filter the simulation, not your queries

`UsdPhysics.CollisionGroup` + `filteredGroups` is Omniverse's simulation-level filter:
`omni.physx/scripts/physicsUtils.py` has `add_collision_to_collision_group` /
`remove_collision_from_collision_group` / `is_in_collision_group` operating on the collection's
include relationship, and `omni/physx/scripts/utils.py` has `addCollisionGroup`. Use it to stop two
bodies from **pushing** each other. Do not expect it to hide anything from a raycast — see above.

## 5. Scene query semantics you must design around

- **[verified]** Scene queries **only work after simulation has begun**. "All data (including
  collider data) is not fully initialized until after that point." A query before PLAY returns
  nothing — which looks exactly like "my collider is broken".
- **[verified]** **A ray that starts *inside* a shape reports the exit point**, not the origin. This
  is the mechanism behind the camera bug described below: a probe ray starting inside a large trigger
  cage reports the cage's far wall, and the distance to it is arbitrary from the code's point of
  view.
- **[verified]** `raycast_all` results are **not sorted** by distance. Sort in client code.
- **[verified]** `raycast_closest` returns a **`dict` with camelCase keys** (`hit`, `collision`,
  `rigidBody`, `distance`, `position`, …) while the callback hit **objects** use snake_case
  (`rigid_body`, `collision`, `position`). Easy to get wrong when switching between them.
- **[verified]** A hit object exposes `collision`, `collision_encoded`, `rigid_body`,
  `rigid_body_encoded`, `protoIndex` — and **nothing that says whether the collider is a trigger**.
  So you cannot classify a hit; you can only compare its path against a set you maintain.
- **[inferred]** Trigger shapes participate in scene queries (PhysX `eSCENE_QUERY_SHAPE`). Not
  established from the SDK, but reproduced indirectly: see §6.

## 6. Adding a collider affects **every** scene query in the engine

This is the lesson of the exercise, and it is cheap to apply.

**[verified]** After gameplay volumes became real colliders, three separate systems that raycast were
wrong at the same time, in three different ways:

| site | failure |
|---|---|
| camera anti-clipping probe | the arm collapsed to its minimum, yanking the camera onto the player, the moment the player entered a volume |
| player ground probe (downward ray) | a volume floor within `radius + 15` below an **airborne** ball made it read as grounded ⇒ a mid-air jump |
| foe ground probe | same, and a grounded foe gets horizontal traction ⇒ it flew |

**Checklist when you introduce a generated collider:**

1. Grep for `raycast`, `scene_query`, `overlap_`, `sweep_` across the engine.
2. Decide explicitly, per call site, whether that collider should be a hit.
3. If not, add it to the ignore set used by that callback.
4. Write a test that drives the **real callback** through a stubbed query interface, and **verify the
   test fails with the exclusion disabled**. A regression test that passes against the broken code
   proves nothing.

### A plane needs a box

**[verified]** PhysX has no infinite-plane collider. A Plane-shaped volume must become a thin box
slab. Pick the half-thickness to reproduce whatever tolerance the previous implementation had — a
genuinely thin slab is a **gameplay change**, not a refactor.

## 7. Per-frame callbacks

Trigger polling runs in a per-frame callback, so the discipline in `AGENTS.md` §2.E applies in full:
guard the callback body, report **once** with a traceback, and disable rather than retry — a single
uncaught exception in a per-frame callback printed ~11 MB of identical tracebacks per hour in this
repository. Never `except Exception: pass`. Never leave a `/persistent/...` setting mutated.
