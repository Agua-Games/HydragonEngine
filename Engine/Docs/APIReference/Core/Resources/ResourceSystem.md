# Classes
---

## ResourceSystem
---




# Variables
---

### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `2 * 1024 * 1024 * 1024`



### `maxResources`

- **Type:** `uint32_t`

- **Default Value:** `100000`



### `enableAsyncLoading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCompression`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `resourceRootPath`

- **Type:** `string`



### `config`

- **Type:** `const ResourceConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ResourceConfig`



### `m_Stats`

- **Type:** `ResourceStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ResourceConfig`

- **Description:** 

- **Members:**

  - `1024`: `2 * 1024 * 1024 *` - 

  - `resourceRootPath`: `string` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static ResourceSystem&`

---

## Initialize



- **Parameters:** `const ResourceConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## LoadResource



- **Parameters:** `const ResourceDesc& desc`

- **Return:** `ResourceHandle`

---

## UnloadResource



- **Parameters:** `ResourceHandle handle`

- **Return:** `void`

---

## PreloadResource



- **Parameters:** `const ResourceDesc& desc`

- **Return:** `void`

---

## SetResourcePriority



- **Parameters:** `ResourceHandle handle`, `float priority`

- **Return:** `void`

---

## GetResourceData



- **Parameters:** `ResourceHandle handle`

- **Return:** `void*`

---

## AddResourceCallback



- **Parameters:** `ResourceHandle handle`, `const ResourceCallback& callback`

- **Return:** `void`

---

## RemoveResourceCallback



- **Parameters:** `ResourceHandle handle`

- **Return:** `void`

---
