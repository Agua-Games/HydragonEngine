Com base nas suas definições, a arquitetura da **Hydragon Engine** foi atualizada para a **versão 2.0**. O documento foi revisado para incorporar a visão de que **todos os sistemas de gameplay** (desde gerenciadores de eventos até configurações de input) são **"Primeiros Cidadãos" do USD**. Isso significa que eles são representados como prims customizados, tornando todo o projeto portável entre engines como Unreal e Unity sem perda de dados ou lógica.

Abaixo está a versão revisada e detalhada do documento.

---

# Hydragon Engine Architecture Document v2.0

## 📋 Executive Summary

**Hydragon** is a game engine and editor built on NVIDIA Omniverse Kit, leveraging USD (Universal Scene Description) as its core data layer. The architecture is designed around **nodal, encapsulated smart assets** that communicate through well-defined interfaces, enabling procedural workflows, visual scripting, and seamless migration to other engines (particularly Unreal Engine).

**Version 2.0 Update:** All game systems (Gameplay Managers, UI, Input, etc.) are now represented as **Custom USD Prims**. This ensures that the entire game project is a collection of declarative, engine-agnostic data that can be translated and reconstructed in other engines.

---

## 🏛️ Core Architecture Principles

### 1. USD as the Single Source of Truth

All game content is defined as USD assets on disk:
- **Levels** → USD Stages (`.usd` files)
- **Characters, Items, Props** → USD Asset files (referenced into levels)
- **Gameplay Logic** → USD Metadata, Custom Attributes, and OmniGraph nodes
- **UI Data** → USD Layering and Composition Arcs
- **Game Managers** → Custom USD Prims (e.g., `QuestManager`, `EventBus`)

**Why USD:**
- Declarative, human-readable data format
- Non-destructive composition (layering, referencing, variants)
- Built-in procedural features
- Native support for massive datasets and streaming
- Engine-agnostic by design

### 2. Encapsulated Smart Assets

Every game asset (Character, Item, Enemy, etc.) is a **self-contained USD asset** with:

| Component | Description |
|-----------|-------------|
| **Root Prim (`defaultPrim`)** | An `Xform` that acts as the container for the entire asset hierarchy |
| **Geometry Prims** | Meshes, skeletal meshes, collision volumes |
| **Interface Attributes** | Custom USD attributes on the root prim that serve as the asset's public API |
| **Internal Logic** | OmniGraph nodes, Python scripts, or references to external logic files |
| **Metadata** | Tags, categories, version info, editor UI hints |

**Key Rule:** Assets **never** access each other's internal structures directly. Communication occurs exclusively through the **interface attributes** exposed on their root prims.

### 3. Interface-Driven Communication

Each asset exposes a public interface via custom attributes on its root prim:

```usda
def "Character" (
    prepend apiSchemas = ["CharacterAPI"]
)
{
    // Interface (Public) Attributes
    int health = 100
    float speed = 3.5
    asset modelFile = @assets/models/knight.usd@
    asset soundFootstep = @assets/sounds/footstep.wav@
    
    // Internal Prims (Hidden from external access)
    def "Skeleton" { ... }
    def "AnimationController" { ... }
    def "Inventory" { ... }
}
```

**Benefits:**
- **Decoupling:** Internal implementation can change without breaking dependencies
- **Versioning:** New attributes can be added; removed attributes are ignored (fail silent)
- **Tool Agnostic:** Any USD-compatible tool (Maya, Houdini, Unreal) can read and write these interfaces
- **Testability:** Easy to swap implementations or create mock versions. This also supports **collaborative non-linear level layout** (designers can use placeholder prims/templates) and **proxy/unloading strategies** (heavy assets can be swapped for simplified proxies or bounding boxes).
- **Manager Systems:** Game managers (Quest, Event, Save) can be placed as custom prims in the level stage, making them travel with the asset to other engines.

### 4. All Systems as First-Class USD Citizens

Every high-level game system is represented as a custom USD prim with a custom schema. This makes them fully portable and editable within the USD ecosystem.

| Hydragon System | USD Representation | Rationale |
| :--- | :--- | :--- |
| **Gameplay Managers (Quests, Events)** | Custom `Prim` (e.g., `QuestManager`), potentially an `OmniGraph` node | Logic and data are stored as declarations, easily exportable to Unreal's blueprint systems. |
| **UI (Menus, Loading Screens)** | Custom `Prim` with a new schema (`UIManager`) | UI layout and data can be stored as attributes, preserving the structure for reconstruction in other engines. |
| **Input Mappings** | Custom `Prim` with a new schema (`InputManager`) | Input configurations are stored as attributes, directly mapping to Unreal's Input Action Mappings. |

**Implementation Example for Managers:**

```usda
def "QuestManager" (
    prepend apiSchemas = ["QuestManagerAPI"]
)
{
    custom string quests = ["Q_001", "Q_002"] // List of quest IDs
    custom bool isActive = true
}
```

**Compound Nodes:** Complex subsystems can be encapsulated as Compound Nodes, collapsing complex graphs into a single, reusable node that represents the entire system.

**Bundles:** The engine can process multiple prims as a bundle (e.g., `QuestManager` containing individual `Quest` prims) for efficient filtering and operations.

### 5. Nodal Architecture at Every Level

#### USD Level (Data Graph)
- Prims form a hierarchical graph
- Composition arcs (references, payloads, variants) define relationships
- **Asset → Node Mapping:** Each `defaultPrim` acts as a logical node in the game world

#### OmniGraph Level (Behavior Graph)
- Action Graphs define procedural logic
- Nodes operate on USD prims (reading/writing attributes)
- Graphs are serialized as USD prims (`def OmniGraphNode`) within the asset
- **Node → Prim Mapping:** OmniGraph nodes are "bridges" that expose prim data to the procedural system

#### Gameplay Level (ECS-like Components)
- Python Scripting Components attached to prims
- Custom schemas extend USD with game-specific types
- Node-based visual scripting for designers

### 6. Procedural Capabilities

Built-in procedural features at multiple levels:

| Level | Technology | Use Case |
|-------|------------|----------|
| **Asset Generation** | USD's `PointInstancer`, Python API | Scatter vegetation, populate crowds |
| **World Building** | OmniGraph, Replicator | Generate terrain, buildings, roads |
| **Behavior** | Action Graph Nodes | AI logic, quest triggers, UI flow |
| **Content Creation** | Integration with Houdini/Substance | Complex procedural assets exported as USD |

### 7. Separation of Concerns: Hydragon Core vs. Extensions

```
┌─────────────────────────────────────────────────┐
│           HYDRAGON BUNDLE EXTENSION            │
│  (Activates all core and optional extensions)   │
└─────────────────────────────────────────────────┘
                      │
        ┌─────────────┴─────────────┐
        │                           │
┌───────▼────────┐         ┌───────▼────────┐
│  HYDRAGON CORE  │         │   OPTIONAL     │
│   EXTENSION     │         │  EXTENSIONS    │
│                 │         │                │
│ • Save/Load     │         │ • Boss AI      │
│ • Quest System  │         │ • Dialogue     │
│ • Checkpoints   │         │ • Crafting     │
│ • Score/Progress│         │ • Inventory UI │
│ • Generic Events│         │ • Weather      │
│ • Menu/Intro    │         │                │
│ • Input Mapping │         │                │
└─────────────────┘         └────────────────┘
```

**Hydragon Core:** Permanent, foundational systems that ship with every game project.  
**Optional Extensions:** Domain-specific features that can be enabled/disabled per project.  
**Hydragon Bundle:** A meta-extension that activates all required extensions in one click.

---

## 🔄 Data Flow & Component Interaction

### Level Loading Sequence

```
1. User opens Level.usd
2. Stage loads → Prims are composed (references resolved)
3. For each asset instance:
   a. Its root prim is identified (via custom schema or convention)
   b. Interface attributes are read
   c. Internal logic (OmniGraph, Python) is initialized
   d. The asset registers itself with the game world
4. Managers (Quest, Event, Input) are loaded as custom prims
5. Game loop begins → Assets update via their logic
6. Communication between assets happens through interface attributes:
   - Asset A writes to its root prim attribute
   - Asset B reads that attribute (via OmniGraph node)
   - Changes trigger events/callbacks
```

### Save/Load System

```
Save:
  1. For each asset, read its current state (health, position, inventory)
  2. For each manager, read its current state (quest progression, event flags)
  3. Write state to JSON/TOML file with asset reference path
  4. Flush to disk

Load:
  1. Read JSON/TOML file
  2. For each entry, find corresponding USD prim by path
  3. Write saved values to interface attributes
  4. Notify assets and managers that they've been restored
```

---

## 🔌 Engine Agnosticism & USD Bridge

### Goal: Export Hydragon projects to Unreal Engine with zero data loss.

**Strategy: Metadata-First Export**

Hydragon assets store all gameplay metadata in USD as custom attributes, schemas, and OmniGraph nodes. This metadata travels with the asset.

**Unreal Import Pipeline:**

| USD Data | Unreal Target |
|----------|---------------|
| Mesh prims | Static/Skeletal Meshes |
| Interface attributes (e.g., `health`, `speed`) | Variables in Blueprint or C++ classes |
| Metadata (e.g., `category=Enemy`, `isBoss=true`) | Tags, Categories, Editor hints |
| Custom schemas (e.g., `CharacterAPI`) | Base classes or interfaces |
| OmniGraph nodes (simplified) | Blueprint event graph nodes |
| Assets references (`SdfAssetPath`) | Asset path mappings |
| Manager Prims (e.g., `QuestManager`) | Game Instance or Subsystem classes |
| UI Prims | UMG Widget Blueprints |
| Input Prims | Input Action Mappings |

**Unreal Mapping System** (within the framework):

```cpp
// Within Agua Games Framework (Unreal)
void UAGF_ImportManager::ImportUSDStage(
    const FString& StagePath,
    FUsdMetadataExportOptions& Options
)
{
    // 1. Open USD Stage
    // 2. For each prim:
    //    a. Identify its schema (CharacterAPI, QuestManagerAPI, etc.)
    //    b. Map schema to Unreal class
    //    c. Read interface attributes
    //    d. Create Unreal asset with matching properties
    //    e. Preserve hierarchy and parent/child relationships
    // 3. Set up asset references (modelFile, etc.)
    // 4. Generate Blueprint class that mirrors the USD asset structure
}
```

**Fail-Silent Principle:** Unreal framework ignores unfamiliar attributes gracefully, logging warnings but continuing import.

---

## 🧩 Hydragon Editor UX Flow

### User Perspective: Creating a Character

```
1. User opens Hydragon Editor (Omniverse Kit)
2. Menu → Create → Character
3. Behind the scenes:
   a. Kit references /assets/templates/Character.usd
   b. The template has a Character schema root
   c. It references a skeletal mesh (default model)
   d. It has pre-attached Python scripting component (basic controller)
   e. Interface attributes: health, speed, modelFile, etc.
4. New Character prim appears in the stage
5. User can:
   a. Edit interface attributes (e.g., set health=150)
   b. Replace the modelFile with their own asset
   c. See changes reflected in viewport immediately
   d. Add OmniGraph nodes to customize behavior
   e. Save as a new USD asset in their project
```

### Spawning Assets

```
1. Spawner prim exists in level (e.g., "EnemySpawner")
2. It has an attribute: assetToSpawn = @assets/characters/Goblin.usd@
3. At runtime:
   a. Spawner reads assetToSpawn
   b. It creates a reference to that USD file in the stage
   c. The new Character prim appears
   d. Its internal logic starts running
   e. It initializes with default interface attributes
```

---

## 📁 Project Directory Structure

```
HydragonProjects/
├── GameProject1/
│   ├── Levels/
│   │   ├── Level1.usd
│   │   ├── Level2.usd
│   │   └── Level1_LightingOverride.usd (layer)
│   ├── Assets/
│   │   ├── Characters/
│   │   │   ├── Knight.usd
│   │   │   ├── Goblin.usd
│   │   │   └── Templates/
│   │   │       └── CharacterTemplate.usd
│   │   ├── Items/
│   │   ├── Environment/
│   │   ├── Audio/
│   │   ├── UI/
│   │   │   ├── MainMenu.usd
│   │   │   └── LoadingScreen.usd
│   │   └── Input/
│   │       └── InputMappings.usd
│   ├── Extensions/
│   │   ├── HydragonCore/
│   │   └── MyCustomExtension/
│   └── Config/
│       ├── hydragon_settings.json
│       ├── savefile.json (game state)
│       └── tags.json
└── SharedLibrary/
    ├── CommonUSD/
    │   ├── Schemas/
    │   │   ├── CharacterAPI.usda
    │   │   ├── ItemAPI.usda
    │   │   ├── QuestManagerAPI.usda
    │   │   ├── UIManagerAPI.usda
    │   │   └── InputManagerAPI.usda
    │   └── OmniGraphNodes/
    └── UnrealBridge/
        ├── USDImportPlugin/
        └── TranslationMaps/
```

---

## 🧩 Extension Development Guidelines

### When to Create an Extension

| Use Case | Recommended Approach |
|----------|----------------------|
| Adding new UI panels or menus | Create an Extension |
| Adding new USD schemas or APIs | Create an Extension |
| Adding new OmniGraph node types | Create an Extension |
| Adding procedural generation tools | Create an Extension |
| Quick prototyping of game logic | Use Python Scripting Component (can later become an Extension) |

### Hydragon Core Extension Structure

```
HydragonCore/
├── config/
│   ├── extension.toml (metadata)
│   └── dependencies.json
├── src/
│   ├── hydragon/
│   │   ├── core/
│   │   │   ├── save_system.py
│   │   │   ├── quest_system.py
│   │   │   ├── checkpoint_manager.py
│   │   │   └── ...
│   │   ├── ui/
│   │   ├── nodes/
│   │   └── __init__.py
├── resources/
└── extension.toml
```

---

## 🎮 Game Systems (Hydragon Core)

### Must-Have Systems

| System | Description | Implementation | Export Strategy |
|--------|-------------|----------------|-----------------|
| **Save/Load** | JSON/TOML serialization | Python system, API access | Exported as manager prim |
| **Quest/Progression** | Quests, checkpoints, unlocks | OmniGraph nodes + Python | Exported as `QuestManager` prim |
| **Generic Events** | Event bus for loose coupling | Python event system | Exported as `EventManager` prim |
| **Menu/Intro** | Main menu, loading screens | Hydragon Core UI extension | Exported as `UIManager` prim |
| **Input** | Keyboard, mouse, controller mapping | Extension (platform-specific) | Exported as `InputManager` prim |
| **Score/Stats** | Player score, kill counts | Interface attributes on Player prim | Exported as attributes |
| **Audio** | Footsteps, ambient, SFX | `SdfAssetPath` + AudioManager | Exported as attributes |
| **Camera** | Follow, cinematic, UI views | USD Camera prims + OmniGraph | Exported as prims |

### System Extension Pattern

```python
# Example: Quest System Node
# quest_nodes.ogn
class AddQuest:
    """
    Adds a quest to the player's quest log.
    """
    @input
    def player: "The player prim" -> SdfPath
    
    @input
    def questId: "ID of the quest to add" -> str
    
    @output
    def success: "True if quest was added successfully" -> bool
    
    @output
    def errorMessage: "Error description if failed" -> str
```

---

## 🚧 Performance Considerations

### Optimization Strategies

1. **LOD Generation:** Omniverse Kit has built-in auto-LOD generation
2. **Geometry Streaming:** Kit supports geometry streaming by design
3. **Payloading:** Use payloads for large assets to defer loading
4. **Instancing:** Mark referenced assets as `instanceable` for geometry reuse
5. **Proxies:** Heavy assets can be swapped for simplified proxies or bounding boxes
6. **Material Instancing:** Leverage OpenPBR uber-shader where possible

### Export to Consoles/Mobiles

| Strategy | Implementation |
|----------|----------------|
| **Native Export** | Not natively supported in Omniverse Kit |
| **Streaming** | Use Kit App Streaming on Azure or third-party solutions (Innoactive) |
| **USDA -> Unreal** | Export USD assets to Unreal for final packaging |

---

## 🔮 Future-Proofing & Evolution

### Versioning Strategy

```
USDA Asset Versioning:
- Major version: Breaking interface changes (new required attributes)
- Minor version: Added attributes (backward compatible)
- Patch version: Internal fixes, no interface changes

Hydragon Editor Versioning:
- Major: Requires asset schema updates
- Minor: New features, backward compatible
- Patch: Bug fixes
```

### Migration Path

```
Hydragon Editor → USD Export → Unreal Import
         │                │
         └────────────────┘
              USD Asset
        (self-contained, engine-agnostic)
```

**Goal:** Never lose work. Even if Omniverse Kit changes, the USD assets remain portable.

---

## ✅ Architecture Checklist

| Principle | Status |
|-----------|--------|
| USD as single source of truth | ✅ Implemented |
| Encapsulated smart assets | ✅ Planned |
| Interface-driven communication | ✅ Planned |
| Nodal architecture (USD + OmniGraph) | ✅ Supported natively |
| Engine agnostic (USD bridge) | ✅ Planned |
| Extensible via Hydragon Core + Optional Extensions | ✅ Planned |
| Procedural generation support | ✅ Supported |
| Fail-silent attribute handling | ✅ Designed |
| Python scripting for rapid prototyping | ✅ Supported natively |
| Visual scripting for designers | ✅ Supported (OmniGraph) |
| Game systems as custom USD prims | ✅ Designed |
| Exportable managers, UI, and input | ✅ Planned |
| Proxy support for heavy assets | ✅ Designed |

---

## 📝 Notes for AI Assistants

When generating code, planning tasks, or answering questions about Hydragon:

1. **Assume USD-Centric:** Every asset, every piece of data is stored in USD.
2. **Assume Encapsulation:** Assets should never directly access each other's internals.
3. **Assume Agnosticism:** The USD data layer should not depend on Omniverse-specific features that would break Unreal import.
4. **Prefer Extensions:** New features should be implemented as Omniverse Kit extensions, not hard-coded into the base editor.
5. **Support Fail-Silent:** When reading attributes, gracefully handle missing ones.
6. **Use Nodal Patterns:** Where possible, expose functionality as nodes.
7. **Everything is a Prim:** Game managers, UI systems, and input configurations are all custom USD prims.

---

*Document Version: 2.0*
*Last Updated: 2026-08-05*
*For: Hydragon Engine Project*