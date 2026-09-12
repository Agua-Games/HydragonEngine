# Hydragon Engine - Agent Guidelines & Coding Standards

This document defines the core engineering standards and conventions for all AI agents working on the Hydragon Engine repository.

---

## 1. Language Policy
* **All Code, Docstrings, and Comments MUST BE IN ENGLISH.**
* Variable names, class names, function names, schema tokens, and logging strings must be in English.
* User communications in chat may be in Portuguese (or the user's preferred language), but all codebase artifacts, commit messages, and source code must be strictly in English.

---

## 2. Architecture & Performance Conventions

### A. OpenUSD & Component-Driven Architecture (API Schemas)
* Treat OpenUSD API Schemas (HydragonActorAPI, HydragonPlayerControllerAPI, HydragonFollowCameraAPI, HydragonChaserAIAPI) as components in an Entity-Component-System (ECS).
* Each interactive prim is an Entity; applied API schemas are its Components.
* **Schema Fallbacks vs Engine Codebase Debugging:**
  - "Fail-silent" applies strictly to OpenUSD custom schema architecture (graceful degradation when a DCC tool or stage does not have custom schemas/attributes registered).
  - For Python engine code, systems, and active development: **do not hide errors silently**. Use explicit error logging (`carb.log_error`), informative exceptions, or temporary debug assertions whenever needed to pinpoint issues quickly rather than guessing.

### B. Avoid Stage Traversal Anti-Patterns
* **NEVER** run stage.Traverse() or full hierarchy scans inside per-frame loops (_on_physics_step or _on_app_update).
* Discover and register actor entities **once** on stage load or simulation start (omni.timeline.TimelineEventType.PLAY).
* Maintain active entity registries in memory dictionaries (Dict[str, Component]) for O(1) lookup and O(N) iteration.
* Listen to stage mutation notices (Usd.Notice.ObjectsChanged) if dynamic runtime addition or duplication must be supported.

### C. Entity Destruction
* When an entity is destroyed or killed in gameplay, deactivate it cleanly via OpenUSD:
  prim.SetActive(False)
* Setting prim.SetActive(False) instructs Omniverse Kit, PhysX, and Hydra/Fabric to immediately:
  1. Remove its rigid bodies and colliders from the physics simulation scene.
  2. Cease all graphics rendering and draw calls.
  3. Free its CPU/GPU simulation overhead.
* Remove the deactivated entity from active system update loops.

### D. PhysX Simulation Safety
* Always apply physical forces at the rigid body's true simulated center of mass (world_pos from physx_iface.get_rigidbody_transformation), never at the world origin (0, 0, 0).
* Author and ensure physics:angularDamping and physics:linearDamping on rolling entities to avoid unconstrained rotational energy accumulation.

### E. Per-Frame Callbacks Must Never Spam the Log

* **Every callback on a per-frame or per-step stream must be exception-safe at its top level:** `_on_app_update` (get_update_event_stream), `_on_physics_step` (PhysX step events), and any `Tf.Notice` handler that can fire per frame.
* **Why this is not optional.** An uncaught exception in a per-frame callback prints a full traceback on *every* frame, for the whole life of the session. Measured in this repository: a single one-line `AttributeError` raised from `_on_app_update` (a missing `self._fallthrough_counter`) produced **893 MB of Kit log in one session** — 258 identical tracebacks inside a 200 KB sample window, roughly 11 MB per hour, sustained.
* **Fail loud once, then stop.** Guard the callback body and, on the first failure:
  1. Log with `carb.log_error`, including the full traceback and a clear component tag.
  2. Set a flag that disables the callback, or at minimum stops the retry.
  3. Keep it disabled until something explicitly re-enables it (`startup()`, a stage event, a user action).

  This keeps the fail-loud rule from section 1.A while making a one-off defect cost one log line instead of a gigabyte.
* **Do NOT wrap a whole callback in `try/except Exception: pass`.** Silently swallowing the error is worse than the spam: the feature stops working with no evidence at all. The guard must *report*, not conceal.
* **Never leave a persistent setting mutated.** Anything written under `/persistent/...` survives a restart and silently changes behaviour for every later session. This includes `/persistent/physics/visualizationDisplayColliders` and `/persistent/app/hydragon/viewport/showVolumes` (the Hydragon volume wireframe toggle, owned by `volume_triggers.VolumeDisplayToggle`). Restore the previous value when the operation that changed it finishes, or scope the change to something non-persistent.
* **Weigh the cost before enabling a scene-wide debug visualisation.** `visualizationDisplayColliders` is a None / Selected / All radiobox with no per-prim granularity: "All" enables debug drawing for *every* collider in the stage, including large triangle-mesh colliders such as ground planes and vehicle meshes. Use "Selected", or expect the editor to seize up.

---

## Omniverse Platform Notes — read before building viewport or physics UI

`docs/omniverse_architecture/` holds platform-level knowledge that was expensive to establish and that is NOT documented by NVIDIA:

| document | read it before... |
|---|---|
| [`README.md`](docs/omniverse_architecture/README.md) | choosing how to draw anything in a viewport |
| [`viewport_drawing_and_gizmos.md`](docs/omniverse_architecture/viewport_drawing_and_gizmos.md) | building a gizmo, manipulator, overlay or visual helper |
| [`collision_triggers_and_scene_queries.md`](docs/omniverse_architecture/collision_triggers_and_scene_queries.md) | authoring a collider, a trigger, or any raycast/overlap |
| [`warp_compute_and_tile_fft.md`](docs/omniverse_architecture/warp_compute_and_tile_fft.md) | writing a Warp kernel, using `wp.tile_fft`, or testing GPU compute without an editor |
| [`debugging_and_tooling.md`](docs/omniverse_architecture/debugging_and_tooling.md) | attaching a debugger, finding a log, writing a probe |

**Check it first; do not re-derive it from a live editor.** Every claim there is tagged `[verified]` or `[inferred]` — respect the distinction, and add your own findings there rather than in a scratch file.

Two rules from it that are violated most often:

* **Only rasterized prims are clickable.** The native viewport pick reads the hydra pick buffer, not a raycast. So a prim that does not render cannot be clicked, and a prim that renders *will* be clicked over the area it draws. Never "hide" a helper with a shading hint (`primvars:isVolume`) — use `visibility = "invisible"` on a prim that is separate from the visual one.
* **A scene query cannot be given a collision channel.** `raycast_all` / `overlap_*` take no filter argument and the bindings expose no `PxQueryFilterCallback`. Any collider you generate is therefore visible to **every** raycast in the engine. When you add one, grep for `raycast` / `scene_query` / `overlap_` and decide per call site whether it should count as a hit.

## Codebase Memory MCP

**MANDATORY: use Codebase Memory MCP graph tools FIRST — before reading files or making code changes.**

This rule applies to every request involving this codebase.

Always call `list_projects` first when you do not already know the project name, then use the `display_name` or exact `name` returned by that tool.

```json
// Step 0 — discover project names
mcp_codebase-memo_list_projects()

// Step 1 — use the project identifier returned above
mcp_codebase-memo_get_architecture({ "project": "<display_name>" })
```

### Workflow

1. Call `list_projects` to discover the correct project name.
2. Call `get_architecture(project)` to understand the codebase structure.
3. Use `search_graph` to find relevant symbols, `trace_call_path` for call chains.
4. Use `get_code_snippet` to read specific function implementations.
5. Only use `read_file` when you need exact raw content to edit a specific line.

### Available Tools (14 MCP tools)

**Indexing:**
- `index_repository(repo_path)` — Index a repository into the knowledge graph
- `list_projects` — List all indexed projects with node/edge counts
- `delete_project(project)` — Remove a project and all its graph data
- `index_status(project)` — Check indexing status

**Querying:**
- `search_graph(name_pattern, name_scope, label, file_pattern, exclude_file_pattern)` — Structured search by label, name/qualified_name, include/exclude file globs
- `trace_call_path(function_name, direction, depth)` — BFS call chain traversal
- `detect_changes(project)` — Map git diff to affected symbols + risk
- `query_graph(query)` — Execute Cypher-like graph queries (read-only)
- `get_graph_schema(project)` — Node/edge counts, relationship patterns
- `get_code_snippet(qualified_name)` — Read source code for a function
- `get_architecture(project)` — Codebase overview: languages, packages, routes, hotspots
- `search_code(pattern, project)` — Grep-like text search within indexed files
- `manage_adr(action)` — CRUD for Architecture Decision Records
- `ingest_traces(traces)` — Ingest runtime traces to validate HTTP edges
