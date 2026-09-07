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
