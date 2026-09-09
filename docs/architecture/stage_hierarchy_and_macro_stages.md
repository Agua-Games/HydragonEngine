# Hydragon Engine - Stage Hierarchy & Macro-Stage Architecture

**Document Version:** 1.0  
**Status:** Approved Standard  
**Component:** OpenUSD Architecture / Level Composition / Runtime Systems  

---

## 1. Architectural Overview & Design Philosophy

In traditional game engines (e.g. Unreal, Unity), persistent game singletons (GameMode, Audio Listener, UI HUD Canvas) often run globally across the entire engine process, or require complex project settings to disable when opening isolated assets or test maps.

Hydragon Engine leverages **OpenUSD (Universal Scene Description)** as its native scene graph, data model, and serialization format. To maintain clean modularity across development teams (environment artists, animators, lighting artists, gameplay engineers) and allow foreign USD stages to be inspected safely:

1. **Strict Opt-In, Prim-Driven Architecture**:
   Engine runtime systems (Soundtrack, UI HUD, Gameplay Effects/VFX, Game Rules, Player Controllers) **MUST REMAIN COMPLETELY DORMANT** unless an explicit USD prim authored with their corresponding API schema is present on the active stage.
2. **Zero Stage Churn & Zero Traversal on Bare Stages**:
   When opening a prop showcase, lighting rig, or bare geometry test stage, no audio is played, no UI overlays are rendered, and no procedural VFX pools or materials are injected into the stage.
3. **Macro-Stage (Master Stage) Composition**:
   Persistent game-wide singletons are authored on a top-level **Macro Stage** (`game_stage.usda` or `master_stage.usda`). Sublevels, menus, and gameplay arenas are referenced/payloaded as children within this macro stage.

---

## 2. Stage Hierarchy & Nesting Model

The project stage hierarchy mirrors the game's logical execution flow, from root singletons down to sublevels and interactive entities:

```text
game_stage.usda (Macro / Master Stage - Root)
│
├── /World/Singletons (Persistent Systems across game lifetime)
│   ├── SoundtrackManager      [HydragonSoundtrackAPI]  (BGM, dynamic cross-fading)
│   ├── GameManager            [HydragonGameAPI]        (Rules, global score, state)
│   ├── UICanvas               [HydragonUICanvasAPI]    (HUD, countdowns, popups)
│   ├── EffectsManager         [HydragonEffectsAPI]     (VFX pools, particle mode)
│   └── SaveGameStats          [HydragonSaveStatsAPI]   (Persistent inventory, quests)
│
├── /World/Sublevels (Referenced or Payloaded Sub-Stages)
│   │
│   ├── MainMenuSublevel (references "main_menu_stage.usda")
│   │   ├── /World/Environment (Menu 3D background, camera, lighting)
│   │   └── /World/MenuUI      (Start game, options, level select)
│   │
│   └── Level01Sublevel (references "level_01_stage.usda")
│       ├── /World/Environment (Terrain, architecture, sky, lighting)
│       ├── /World/PhysicsScene (Gravity, physics settings)
│       ├── /World/FollowCamera [HydragonFollowCameraAPI]
│       ├── /World/Player       [HydragonPlayerControllerAPI, HydragonActorAPI]
│       ├── /World/Foes         [HydragonChaserAIAPI, HydragonActorAPI]
│       └── /World/Triggers     [HydragonTriggerAPI]
```

---

## 3. Benefits of the Macro-Stage Pattern

| Capability | Macro Stage Pattern | Global Singleton Anti-Pattern |
| :--- | :--- | :--- |
| **Standalone Level Testing** | Level designer opens `level_01_stage.usda` alone. Can play in peace without unwanted menus or background music blasting. | Music and HUD blast automatically on every opened stage. |
| **Asset & Lighting Inspection** | Lighting artist opens `props/chest.usda` to adjust materials. Stage remains pristine without runtime noise. | Engine creates `/World/Effects` and UI canvases inside the asset file. |
| **Cross-Level Persistence** | Game singletons live in the root macro stage and persist when switching active sublevel payloads. | Complex singleton preservation logic across map reloads. |
| **Multi-User Collaboration** | Environment team edits `level_01_environment.usda`, audio team edits `SoundtrackManager` in `game_stage.usda` with zero file lock conflicts. | Merge conflicts on monolith project settings. |

---

## 4. USD Composition Example (game_stage.usda)

Below is an example of a declarative Macro Stage authoring persistent singletons and referencing sublevels:

```usda
#usda 1.0
(
    defaultPrim = "World"
    metersPerUnit = 0.01
    upAxis = "Y"
    doc = "Hydragon Engine - Macro Stage Root"
)

def Xform "World"
{
    # -------------------------------------------------------------------------
    # Persistent Singletons (Root Level)
    # -------------------------------------------------------------------------
    def Xform "SoundtrackManager" (
        prepend apiSchemas = ["HydragonSoundtrackAPI"]
    )
    {
        token soundtrack:currentTrack = "ambient"
        float soundtrack:volume = 0.8
        bool soundtrack:autoPlay = true
        bool soundtrack:isLooping = true
        float soundtrack:fadeDuration = 1.5
    }

    def Xform "GameManager" (
        prepend apiSchemas = ["HydragonGameAPI"]
    )
    {
        token game:state = "Playing"
        int game:score = 0
        int game:totalFoes = 10
    }

    def Xform "GameHUD" (
        prepend apiSchemas = ["HydragonUICanvasAPI"]
    )
    {
        bool hud:showControls = true
        bool hud:showCountdown = true
        bool hud:showScorePopups = true
        bool hud:autoActivateOnPlay = true
    }

    def Xform "EffectsManager" (
        prepend apiSchemas = ["HydragonEffectsAPI"]
    )
    {
        int effects:poolSize = 3
        int effects:numSparks = 150
        token effects:renderMode = "point_instancer" # or "points"
        float effects:sparkRadius = 20.0
        float effects:flashIntensity = 5000000.0
        bool effects:autoInitializeOnPlay = true
    }

    # -------------------------------------------------------------------------
    # Sublevel Payloads / References
    # -------------------------------------------------------------------------
    def Xform "ActiveLevel" (
        payload = @./levels/level_01_stage.usda@</World>
    )
    {
    }
}
```

---

## 5. API Schema Reference Matrix

| Subsystem | API Schema Name | Python Schema Wrapper | Menu Location |
| :--- | :--- | :--- | :--- |
| **Soundtrack** | `HydragonSoundtrackAPI` | `HydragonSoundtrack` | `Create > Hydragon > Soundtrack Manager` |
| **UI HUD** | `HydragonUICanvasAPI` | `HydragonUICanvas` | `Create > Hydragon > UI Canvas (Game HUD)` |
| **Effects VFX** | `HydragonEffectsAPI` | `HydragonEffectsManager` | `Create > Hydragon > Effects Manager` |
| **Game Rules** | `HydragonGameAPI` | `HydragonGameManager` | `Create > Hydragon > Game Manager` |
| **Player** | `HydragonPlayerControllerAPI` | `HydragonPlayerController` | `Create > Hydragon > Player Ball` |
| **Chaser AI** | `HydragonChaserAIAPI` | `HydragonChaserAI` | `Create > Hydragon > Foe Ball` |
| **Triggers** | `HydragonTriggerAPI` | `HydragonTrigger` | `Create > Hydragon > Goal Hole` |

---

## 6. Developer Guidelines

1. **Never Assume System Presence**: Systems should check `self._active_entity` or schema presence before executing logic. If absent, silently do nothing.
2. **One-Time Discovery on Simulation Start**: Discover singleton prims once in `TimelineEventType.PLAY`. Do not traverse the stage every frame.
3. **Deactivation Safety**: When stopping simulation (`STOP`), release runtime caches and de-allocate transient state so the stage remains unmodified.
