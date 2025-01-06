# Classes
---

## AssetType
---



## LoadPriority
---



## UpdateStrategy
---




# Variables
---

### `AssetHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `priority`

- **Type:** `LoadPriority`

- **Default Value:** `LoadPriority::Normal`



### `asyncLoad`

- **Type:** `bool`

- **Default Value:** `true`



### `keepInMemory`

- **Type:** `bool`

- **Default Value:** `false`



### `generateMips`

- **Type:** `bool`

- **Default Value:** `true`



### `type`

- **Type:** `AssetType`



### `path`

- **Type:** `string`



### `size`

- **Type:** `size_t`



### `lastAccess`

- **Type:** `time_point`



### `referenceCount`

- **Type:** `uint32_t`



### `isLoaded`

- **Type:** `bool`



### `isHotReloadable`

- **Type:** `bool`



### `totalLoaded`

- **Type:** `size_t`



### `totalMemoryUsed`

- **Type:** `size_t`



### `peakMemoryUsed`

- **Type:** `size_t`



### `loadingErrors`

- **Type:** `uint32_t`



### `averageLoadTime`

- **Type:** `float`




# Structs
---

### `LoadParams`

- **Description:** 

- **Members:**



### `AssetInfo`

- **Description:** 

- **Members:**

  - `type`: `AssetType` - 

  - `path`: `string` - 

  - `size`: `size_t` - 

  - `lastAccess`: `time_point` - 

  - `referenceCount`: `uint32_t` - 

  - `isLoaded`: `bool` - 

  - `isHotReloadable`: `bool` - 



### `AssetStats`

- **Description:** 

- **Members:**

  - `totalLoaded`: `size_t` - 

  - `totalMemoryUsed`: `size_t` - 

  - `peakMemoryUsed`: `size_t` - 

  - `loadingErrors`: `uint32_t` - 

  - `averageLoadTime`: `float` - 


