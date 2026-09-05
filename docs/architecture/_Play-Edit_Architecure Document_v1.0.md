# Hydragon Play-Edit System – v1.0

## 📋 Executive Summary

The **Hydragon Play-Edit System** is a unified workflow that allows game designers, level artists, and developers to seamlessly **playtest** and **edit** a game level simultaneously, within the same session, without destructive operations. The system leverages the core architecture of Omniverse Kit and USD—**Live Sessions**, **Layers**, **Payloads**, and **Composition Arcs**—to provide a non-linear, iterative, and collaborative environment where gameplay and content creation are not separate modes but two faces of the same interactive process.

A key differentiator is the **Multi-Layer Edit Target workflow**, which allows designers to organize edits into independent layers for A/B testing, selective commits, and non-destructive experimentation—similar to working with adjustment layers in Photoshop.

---

## 🎯 Core Design Principles

### 1. Play-Edit as a Unified Mode, Not a Toggle

The Play-Edit system does not switch between "Edit Mode" and "Play Mode". Instead, it treats **interaction with the game world** and **content authoring** as activities that can happen concurrently. Users can:

- **Move the player character** through the level (playing) and **immediately reposition an obstacle** when they notice it's misplaced.
- **Spawn enemies dynamically** and, if they like the result, **save the changes** to the level's base file.
- **Collaborate with other users** who are editing the same level, seeing their changes in real-time while playing.

### 2. Non-Destructive Editing via USD Composition

The USD architecture is built on the principle of **composition arcs**, where multiple layers combine to form a final composed scene . The strength ordering of these arcs, known by the acronym **LIVRPS** (Layers, Inherits, Variants, References, Payloads, Specializes), determines which opinions take precedence .

**Key Implication:** Stronger layers (like a Session Layer) can override weaker layers (like the base ROOT layer) without modifying the original data . This enables:

- **Iterative Design:** Test ideas freely and discard them without consequences.
- **Versioning:** Each session can be saved as a new layer, preserving the history of changes.
- **Safe Experimentation:** No risk of breaking the original level file.

### 3. Collaborative by Design

The Play-Edit system supports **multiple concurrent users** editing and playing in the same level simultaneously through **Live Sessions** .

**Live Session Fundamentals** :
- A Live Session is bound to a **base layer** (the USD file being edited)
- All participants join the same session and see each other's modifications in real-time
- Changes are written to a **Live Layer** (`.live` file) that is inserted into the layer stack
- The Live Session Owner has permission to merge the session back to the base layer

**Implementation:** When a user joins a Live Session, the Live Layer is inserted into the stage's composition stack . For sublayers, it's inserted into the local sublayer stack; for prims, it's inserted into the prim's referenceList or payloadList.

---

## 🏗️ System Architecture

### Layer Stack Structure

The Play-Edit system relies on a **structured USD layer stack** to manage different types of content and edits . Layers are ordered by **strength**—stronger layers override weaker ones.

| Layer Name | Description | Strength in LIVRPS | Role in Play-Edit |
| :--- | :--- | :--- | :--- |
| **Live Session Layer (`.live`)** | Auto-created during Live Sessions. Contains real-time collaborative edits. | **Strongest** (overrides all below) | All temporary edits during Play-Edit are written here. When the session ends, changes can be merged or discarded . |
| **Session Layer (Edit Target)** | User-created layer for a specific editing session. | Strong | **The primary work layer.** The designer switches Edit Target to this layer to organize edits into logical groups (e.g., "Cinema Facade Edits," "Debris Zone Edits"). |
| **Hydragon Layout Layer** | Contains positioning and orientation of assets. | Medium | Edits to asset placement (e.g., moving a building) are written here. |
| **Hydragon Material Layer** | Contains material assignments. | Medium | Edits to materials (e.g., changing wall color) are written here. |
| **Hydragon Lighting Layer** | Contains environment lights and light rigs. | Medium | Edits to lighting (e.g., time of day) are written here. |
| **Hydragon Animation Layer** | Contains animation opinions. | Medium | Edits to animations (e.g., camera paths) are written here. |
| **Hydragon Assets Layer** | Contains imported assets (meshes, textures, sounds). | Weak | Static asset data. Other layers reference and position these assets. |
| **ROOT Layer** | The base layer of the USD stage. | **Weakest** | **The "Image Original."** Contains permanent, approved content. Edits during Play-Edit do not touch this layer until explicitly committed . |

### Live Session Physical Structure

When a Live Session is created, the following directory structure is established on disk :

```
$(Base Directory)/
├── $(Layer Name).usd          # Base layer file
├── .live/                      # Live Session directory
│   └── $(Session Name).live/
│       ├── __session__.toml    # Session metadata (owner, description, etc.)
│       ├── __session__.channel # Communication channel for collaboration
│       ├── root.live           # The actual Live Layer
│       └── shared_data/        # Shared data between clients
```

### Multi-Layer Edit Target Workflow

The core advancement in v1.0 is the ability to work with **multiple independent Edit Targets** simultaneously. This allows designers to:

1. **Organize Edits into Logical Groups:** Edits to the cinema facade go in Layer A; edits to the debris zone go in Layer B.
2. **Perform A/B Testing:** Toggle layers on/off to compare different versions.
3. **Commit Selectively:** Merge Layer A to ROOT while keeping Layer B as a work-in-progress.
4. **Collaborate Without Conflict:** Different users can work on different layers and merge later.

**Technical Implementation:** The `omni.kit.usd.layers` extension provides APIs for managing layers and switching Edit Targets . The `omni.kit.usd_undo` extension ensures that operations on layers and Edit Targets are undoable .

---

## 🧩 Core Features & Workflows

### 1. Play-Edit Toggle

- **Hotkey/Button:** A dedicated button (e.g., a "Play" icon) or hotkey (e.g., `P`) that toggles the "active" state of the Play-Edit system.
- **Input Focus:** When active, keyboard and mouse input are routed to the Player Controller. When "paused," input is routed to the editor.
- **Viewport Overlay:** A HUD overlay displays gameplay information (e.g., health, quests) while the system is active.

### 2. Multi-Layer Edit Target Management

The Layer Management UI provides controls for:

- **Creating a New Sublayer:** `CreateSublayer` command to create a new layer file and optionally insert it as a sublayer of the root layer .
- **Setting Edit Target:** `SetEditTarget` command to make a specific layer the active target for all edits .
- **Muting/Unmuting Layers:** `SetLayerMuteness` command to temporarily deactivate a layer (like hiding a Photoshop layer) .
- **Locking/Unlocking Layers:** `LockLayer` command to prevent edits to a protected layer .
- **Merging Layers:** `MergeLayers` command to combine multiple layers into one .
- **Flattening Layers:** `FlattenLayers` command to collapse the entire layer stack into a single layer .
- **Moving Prim Specs:** `MovePrimSpecsToLayer` to move specific prims from one layer to another .

### 3. Interactive Play

- **Character Controller:** The existing `omni.physxcct` extension provides a full character controller with collision detection, sweep, ledge detection, and gamepad support.
- **Gameplay State:** The game state (quests, checkpoints, player progress) is managed by the **Hydragon Core** extension and can be saved to JSON/TOML for persistent storage.
- **Dynamic Sky:** The time of day can be changed using the `omni.kit.environment.core` extension, which provides `PlaySettings` for controlling sunstudy play, looping, and rate.

### 4. In-Situ Editing (Play-Edit)

- **Obstacle Repositioning:** While "paused," users can select and move a prim (e.g., an obstacle) using standard editor tools. The changes are written to the **current Edit Target**.
- **Asset Spawning:** Users can spawn new assets (e.g., enemies, props) using the Hydragon Spawner system. Spawned objects are written to the current Edit Target.
- **Properties Editing:** Users can edit custom attributes (e.g., `health`, `speed`) on any prim. These changes are written to the current Edit Target.

**Key Rule:** The designer can switch Edit Target at any time. Edits to the cinema facade go to Layer A; edits to debris go to Layer B.

### 5. Save and Commit Workflow

- **Save All:** A "Save All" button that performs a Flatten operation, merging all Session Layer changes into the base file (ROOT) and saving the stage . This is the equivalent of "Flatten Image" in Photoshop.
- **Save/Bake Transforms:** A "Bake Transforms" button that applies all transform changes from the current Edit Target to the base file.
- **Save/Bake Selected:** A "Bake Selected" button that applies changes from the current Edit Target to the base file only for the currently selected prims.
- **Merge to New Layer:** Changes are saved as a new, separate USD layer file. This is non-destructive and preserves a record of the session .
- **Do Not Merge:** The Live Session Layer is retained, and changes are not merged .

### 6. Teleport

- **Hotkey/Button:** A hotkey (e.g., `T`) or button that teleports the player character to the current viewport camera position.
- **Implementation:** The teleport function uses the `omni.kit.tool.teleport` extension's raycasting logic to find a valid position on the ground, then uses the CCT API's `set_position` to place the character there. The character is placed slightly above the ground to avoid physics interpenetration.

### 7. Dynamic Level Streaming

- **Payloads:** Parts of the level are stored as **Payloads** (USD files referenced via `payload`). When the Play-Edit system is active, only the currently relevant payloads are **loaded**; others are **unloaded** to conserve memory and improve performance .
- **Selective Loading:** Users can explicitly load or unload a payload, focusing their editing on a specific section of the level ("sublevel").

---

## 🛠️ Implementation Guidelines

### Extensions

The Play-Edit system is built as a set of Omniverse Kit extensions, reusing and extending existing core extensions.

- **HydragonPlayMode:** The main extension that manages the Play-Edit state (active/paused), input routing, and UI.
- **HydragonCore:** Provides gameplay systems (Save/Load, Quests, Checkpoints, Teleport) that can be called from the Play-Edit system.
- **HydragonSpawner:** Manages the spawning of assets and the lifecycle of spawned objects (cleanup on session end).
- **HydragonLayerManager:** Provides UI controls for managing layers, including creating sublayers, setting Edit Targets, muting, locking, merging, and flattening.

### Python API Usage

#### Creating a New Sublayer and Setting Edit Target

```python
import omni.kit.commands
import omni.usd

# Create a new sublayer
omni.kit.commands.execute(
    'CreateSublayer',
    dest_layer_identifier='/path/to/layer.usda'
)

# Set it as the current Edit Target
stage = omni.usd.get_context().get_stage()
new_layer = Usd.Layer.FindOrOpen('/path/to/layer.usda')
stage.SetEditTarget(Usd.EditTarget(new_layer))
```

#### Muting a Layer

```python
import omni.kit.commands

omni.kit.commands.execute(
    'SetLayerMuteness',
    layer_identifier='/path/to/layer.usda',
    muted=True
)
```

#### Merging Layers

```python
import omni.kit.commands

omni.kit.commands.execute(
    'MergeLayers',
    src_layer_identifier='/path/to/source.usda',
    dest_layer_identifier='/path/to/destination.usda'
)
```

#### Flattening Layers

```python
import omni.kit.commands

omni.kit.commands.execute(
    'FlattenLayers',
    dest_layer_identifier='/path/to/flattened.usda'
)
```

#### Undo/Redo Support

The `omni.kit.usd_undo` extension provides utility classes for managing undo operations on USD layers and Edit Targets :

```python
import omni.kit.usd_undo

usd_undo = omni.kit.usd_undo.UsdLayerUndo(stage.GetEditTarget().GetLayer())
usd_undo.reserve("/root/prim")  # Track changes to this prim
# Perform edits...
usd_undo.undo()  # Revert all changes
```

#### Live Session Management

The `omni.kit.usd.layers.LiveSyncing` interface provides APIs for joining and managing Live Sessions .

### UI

The Play-Edit system's UI is built using the `omni.ui` framework.

**Key UI elements:**
- A "Play" button with a state indicator.
- A "Save All" button (Flatten).
- A "Teleport" button.
- A "Bake Transforms" button.
- Layer Management Panel showing:
  - List of all layers in the stack
  - Current Edit Target indicator
  - Mute/Unmute toggle per layer
  - Lock/Unlock toggle per layer
  - "Create New Layer" button
  - "Merge to..." context menu
  - "Set as Edit Target" context menu

---

## 📁 Project Directory Structure

Within a Hydragon project, the Play-Edit system uses the following structure:

```
HydragonProjects/
├── GameProject1/
│   ├── Levels/
│   │   ├── Level1.usd (Base level file - ROOT layer)
│   │   ├── Level1_Layout.usd (Layout layer)
│   │   ├── Level1_Materials.usd (Materials layer)
│   │   ├── Level1_Lighting.usd (Lighting layer)
│   │   ├── CinemaFacade_Edits.usda (User-created layer for cinema edits)
│   │   ├── DebrisZone_Edits.usda (User-created layer for debris edits)
│   │   ├── .live/ (Live Session directory)
│   │   │   └── Level1.live/
│   │   │       ├── __session__.toml
│   │   │       ├── __session__.channel
│   │   │       └── root.live
│   ├── Assets/
│   │   ├── ...
│   └── Config/
│       └── play_edit_settings.json
```

---

## 🎮 Use Case: The Cinema Facade & Debris Zone

**Scenario:** A level designer is playtesting a motorcycle level. They notice two issues:

1. The cinema facade composition is off—needs adjusting.
2. Some debris is missing from a zone up ahead.

**Workflow:**

1. **Pause Play:** The designer parks the motorcycle and presses the hotkey to pause player input.
2. **Start Edit Session (Cinema):** They create a new layer called `CinemaFacade_Edits.usda` and set it as the Edit Target.
3. **Edit:** They adjust the cinema facade—moving elements, resizing, etc. All changes go to `CinemaFacade_Edits.usda`.
4. **Resume Play:** They unpause and ride to the debris zone.
5. **Start Edit Session (Debris):** They create a new layer called `DebrisZone_Edits.usda` and set it as the Edit Target.
6. **Edit:** They add debris props, adjust their positions. All changes go to `DebrisZone_Edits.usda`.
7. **Test & Iterate:** They can toggle layers on/off to see the effect of each set of edits independently.
8. **Commit:** After testing, they decide to:
   - **Merge Cinema Facade Edits to ROOT** (approved).
   - **Keep Debris Zone Edits as a separate layer** for further A/B testing.
   - Alternatively, **Flatten Everything** to create a single, optimized level file.

**Benefits:**
- **No loss of work:** Edits are stored independently.
- **A/B Testing:** Compare versions by toggling layers.
- **Collaborative:** Another designer could work on the cinema while the first works on debris—different layers, no conflict.
- **Incremental Commits:** Commit approved changes gradually.

---

## ✅ Architecture Checklist

| Feature | Status |
| :--- | :--- |
| **Unified Play-Edit Mode** | ✅ Designed |
| **Non-Destructive Editing** | ✅ Supported via USD composition arcs  |
| **Live Session Layer** | ✅ Supported natively by Omniverse  |
| **Collaborative Editing** | ✅ Supported natively by Omniverse  |
| **Multi-Layer Edit Targets** | ✅ Designed (via `SetEditTarget`) |
| **Layer Management (Create, Mute, Lock)** | ✅ Supported via `omni.kit.commands`  |
| **Layer Merging** | ✅ Supported via `MergeLayers`  |
| **Layer Flattening** | ✅ Supported via `FlattenLayers`  |
| **Undo/Redo for Layers** | ✅ Supported via `omni.kit.usd_undo`  |
| **Spawners & Temporary Objects** | ✅ Designed (written to current Edit Target) |
| **Teleport** | ✅ Designed (via `omni.kit.tool.teleport`) |
| **Dynamic Level Streaming** | ✅ Supported via Payloads  |
| **Time of Day Changes** | ✅ Supported via `omni.kit.environment.core` |
| **Strength Ordering (LIVRPS)** | ✅ Understood and leveraged  |

---

## 📝 Notes for AI Assistants

When generating code, planning tasks, or answering questions about the Hydragon Play-Edit system:

1. **Assume Play-Edit is the default mode.** The system is always in a "Play-Edit" state, not toggling between separate modes.
2. **Assume multiple Edit Targets.** Designers can create multiple layers and switch between them using `SetEditTarget`.
3. **Assume edits are non-destructive.** All edits are written to the current Edit Target, which is a layer above ROOT. The ROOT layer remains untouched until explicitly merged.
4. **Assume collaboration is a core feature.** Multiple users can be in the same Live Session simultaneously .
5. **Use the layer stack.** The layer stack is the primary mechanism for managing non-destructive edits .
6. **Use USD strength ordering (LIVRPS).** Understand that stronger layers override weaker ones .
7. **Make all changes to the current Edit Target.** For changes to be visible to other Live Session participants, they must be written to the Live Layer (which is the current Edit Target during a Live Session) .

---

## 🔗 References

- [USD Composition Basics – Strength Ordering](https://docs.nvidia.com/learn-openusd/latest/composition-basics/strength-ordering.html) 
- [omni.kit.usd.layers – Live Session Documentation](https://docs.omniverse.nvidia.com/kit/docs/omni.kit.usd.layers/2.2.10/Overview.html) 
- [omni.kit.usd.layers – Classes Documentation](https://docs.omniverse.nvidia.com/kit/docs/omni.kit.usd.layers/latest/omni.kit.usd.layers.Classes.html) 
- [USD in Kit – Layer/EditTarget Commands](https://docs.omniverse.nvidia.com/kit/docs/omni.kit.usd_docs/1.1.1/USD%20in%20Kit.html) 
- [omni.kit.usd_undo – Undo Support](https://docs.omniverse.nvidia.com/kit/docs/omni.kit.usd_undo/0.1.8/Overview.html) 
- [Maya USD – USD Stage Composition](https://help.autodesk.com/cloudhelp/2023/ENU/Maya-USD/files/USD-for-Maya/Get-Started-with-USD-in-Maya/Create-a-USD-stage/GUID-772B376F-4E86-4847-A1A0-BB84E21DB2FD.html) 
- [Creating Composition Arcs](https://docs.nvidia.com/learn-openusd/latest/creating-composition-arcs/index.html) 

---

*Document Version: 1.0*
*Last Updated: 2026-08-06*
*For: Hydragon Engine Project*