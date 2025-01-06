# Classes
---

## SceneSystem
---




# Variables
---

### `maxScenes`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `maxEntitiesPerScene`

- **Type:** `uint32_t`

- **Default Value:** `100000`



### `enableSceneStreaming`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAsyncLoading`

- **Type:** `bool`

- **Default Value:** `true`



### `sceneMemoryBudget`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024 * 1024`



### `config`

- **Type:** `const SceneConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const LoadSceneParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `SceneConfig`



### `m_Stats`

- **Type:** `SceneStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `SceneConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static SceneSystem&`

---

## Initialize



- **Parameters:** const SceneConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## CreateScene



- **Parameters:** const SceneDesc& desc

- **Return:** `SceneHandle`

---

## DestroyScene



- **Parameters:** SceneHandle handle

- **Return:** `void`

---

## LoadScene



- **Parameters:** const std::string& path, const LoadSceneParams& params = {}

- **Return:** `void`

---

## UnloadScene



- **Parameters:** SceneHandle handle

- **Return:** `void`

---

## GetEntityManager



- **Parameters:** SceneHandle handle

- **Return:** `EntityManager*`

---
