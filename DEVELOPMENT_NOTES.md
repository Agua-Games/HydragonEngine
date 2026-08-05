# Hydragon Engine Development Notes & Performance Optimization

This document tracks Hydragon-specific technical findings, performance issues, and workarounds to keep development decoupled from the base Omniverse Kit App template.

---

## Known Issues

### 1. CPU-Bound Performance Drops in Editor (USD Notice & Preview Observers)
* **Status:** Workaround Active (Default Disabled)
* **Symptom:** Editor frame rate drops from ~78 FPS down to ~50 FPS, even in static or idle scenes.
* **Profiler Analysis (F8):** 
  * CPU loop (`App Update` / Carbonite `Dispatch Event: update`) spikes to **~17ms** (CPU-bound).
  * GPU render frame time remains extremely low at **~0.6ms** (GPU is idle).
  * The bottleneck is inside the event dispatcher (`Notify observers`).

#### Root Causes
1. **Notice Loop Cascade:** Background UI systems (like Scene Optimizer and Asset Validator) register USD stage listeners. Whenever the timeline updates or any value changes, a cascade of notice-callback evaluations traverses the stage, creating heavy CPU overhead.
2. **Background Rendering Viewports:** The retargeting preview extension (`omni.anim.retarget.preview`) spawns background virtual Hydra viewport contexts that run render loops on every frame, consuming **~1.5ms** of CPU time even when its tab/window is closed.
3. **Shutdown Reference Leaks:** Disabling these extensions at runtime via the Extension Manager does *not* recover performance. NVIDIA’s scripts for these extensions contain Python reference leaks in their `on_shutdown()` cycle (cyclic references, unreleased frame stacks). The Python garbage collector cannot reclaim their objects, leaving their background threads and update listeners active in memory.

#### Working Solution & Guidelines
To maintain optimal editor performance, these extensions are **commented out by default** in [`hydragon.editor.kit`](file:///d:/AguaGames/HydragonEngine/alpha/source/apps/hydragon.editor.kit):
* `omni.anim.retarget.bundle`
* `omni.anim.navigation.bundle`
* `omni.scene.optimizer.bundle`

**Developer Workflow:**
* **Uncomment** these lines only when actively working on AI pathfinding/navmeshes, skeletal retargeting, or running stage optimizations.
* **Comment them back out** when you return to layout, core scripting, or gameplay testing, then run `.\repo.bat build` and restart the editor.

#### Long-Term Resolution Plan
* Investigate the exact reference leaks in `omni.anim.retarget.preview` and `omni.scene.optimizer.core` using memory profiling tools.
* Implement a runtime patch (similar to our menu monkeypatch) that intercepts these extensions' `on_startup` and forces them to unsubscribe from the `on_update` stream unless their UI panel is active.
* Report the reference leaks to NVIDIA Omniverse developer forums.
