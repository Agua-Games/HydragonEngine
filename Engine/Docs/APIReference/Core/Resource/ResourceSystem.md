# Classes
---

## ResourceSystem
---




# Variables
---

### `maxResources`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `maxPendingLoads`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `enableAsyncLoading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `enableResourcePacking`

- **Type:** `bool`

- **Default Value:** `true`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `512 * 1024 * 1024`



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

  - `1024`: `512 * 1024 *` - 

  - `resourceRootPath`: `string` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static ResourceSystem&`

---

## Initialize



- **Parameters:** const ResourceConfig& config = {}

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

## LoadResource



- **Parameters:** const ResourceDesc& desc

- **Return:** `ResourceHandle`

---

## UnloadResource



- **Parameters:** ResourceHandle handle

- **Return:** `void`

---

## GetResourceData



- **Parameters:** ResourceHandle handle

- **Return:** `void*`

---

## SetResourceCallback



- **Parameters:** ResourceHandle handle, const ResourceCallback& callback

- **Return:** `void`

---

## PreloadResource



- **Parameters:** const std::string& path

- **Return:** `void`

---

## PurgeUnusedResources



- **Parameters:** 

- **Return:** `void`

---
