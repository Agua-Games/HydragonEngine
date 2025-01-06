# Classes
---

## ResourceType
---



## LoadPriority
---




# Variables
---

### `ResourceHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `path`

- **Type:** `string`



### `type`

- **Type:** `ResourceType`



### `priority`

- **Type:** `LoadPriority`

- **Default Value:** `LoadPriority::Normal`



### `async`

- **Type:** `bool`

- **Default Value:** `true`



### `cached`

- **Type:** `bool`

- **Default Value:** `true`



### `compressed`

- **Type:** `bool`

- **Default Value:** `true`



### `expectedSize`

- **Type:** `size_t`

- **Default Value:** `0`



### `type`

- **Type:** `ResourceType`



### `path`

- **Type:** `string`



### `size`

- **Type:** `size_t`



### `loaded`

- **Type:** `bool`



### `cached`

- **Type:** `bool`



### `compressed`

- **Type:** `bool`



### `loadProgress`

- **Type:** `float`



### `lastAccessTime`

- **Type:** `uint64_t`



### `ResourceCallback`

- **Type:** `using`

- **Default Value:** `std::function<void(ResourceHandle, bool)>`



### `totalResources`

- **Type:** `uint32_t`



### `loadedResources`

- **Type:** `uint32_t`



### `pendingLoads`

- **Type:** `uint32_t`



### `failedLoads`

- **Type:** `uint32_t`



### `totalMemoryUsed`

- **Type:** `size_t`



### `cachedMemoryUsed`

- **Type:** `size_t`



### `averageLoadTimeMs`

- **Type:** `float`




# Structs
---

### `ResourceDesc`

- **Description:** 

- **Members:**

  - `path`: `string` - 

  - `type`: `ResourceType` - 



### `ResourceInfo`

- **Description:** 

- **Members:**

  - `type`: `ResourceType` - 

  - `path`: `string` - 

  - `size`: `size_t` - 

  - `loaded`: `bool` - 

  - `cached`: `bool` - 

  - `compressed`: `bool` - 

  - `loadProgress`: `float` - 

  - `lastAccessTime`: `uint64_t` - 



### `ResourceStats`

- **Description:** 

- **Members:**

  - `totalResources`: `uint32_t` - 

  - `loadedResources`: `uint32_t` - 

  - `pendingLoads`: `uint32_t` - 

  - `failedLoads`: `uint32_t` - 

  - `totalMemoryUsed`: `size_t` - 

  - `cachedMemoryUsed`: `size_t` - 

  - `averageLoadTimeMs`: `float` - 


