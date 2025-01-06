# Classes
---

## SceneEditorSystem
---




# Variables
---

### `enableGizmos`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSnapping`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLayers`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePrefabs`

- **Type:** `bool`

- **Default Value:** `true`



### `maxUndoOperations`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `gridSize`

- **Type:** `float`

- **Default Value:** `1.0f`



### `snapIncrement`

- **Type:** `Vector3`

- **Default Value:** `{0.5f, 0.5f, 0.5f}`



### `config`

- **Type:** `const SceneEditorConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `SceneEditorConfig`



### `m_Stats`

- **Type:** `SceneEditorStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `SceneEditorConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static SceneEditorSystem&`

---

## Initialize



- **Parameters:** const SceneEditorConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## CreateEntity



- **Parameters:** const EntityDesc& desc

- **Return:** `void`

---

## DeleteEntity



- **Parameters:** EntityHandle handle

- **Return:** `void`

---

## DuplicateEntity



- **Parameters:** EntityHandle handle

- **Return:** `void`

---

## SetSelection



- **Parameters:** const std::vector<EntityHandle>& entities

- **Return:** `void`

---

## ClearSelection



- **Parameters:** 

- **Return:** `void`

---

## BeginTransform



- **Parameters:** TransformMode mode

- **Return:** `void`

---

## UpdateTransform



- **Parameters:** const Transform& delta

- **Return:** `void`

---

## EndTransform



- **Parameters:** 

- **Return:** `void`

---

## CreatePrefab



- **Parameters:** const std::string& name, EntityHandle root

- **Return:** `void`

---

## InstantiatePrefab



- **Parameters:** const std::string& name, const Transform& transform

- **Return:** `void`

---
