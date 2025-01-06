# Classes
---

## LongSessionManager
---




# Variables
---

### `peakMemoryUsage`

- **Type:** `size_t`



### `currentMemoryUsage`

- **Type:** `size_t`



### `uptime`

- **Type:** `hours`



### `defragmentationCount`

- **Type:** `uint32_t`



### `fragmentationRatio`

- **Type:** `float`



### `now`

- **Type:** `auto`

- **Default Value:** `Time::Now()`



### `m_LastPressureCheck`

- **Type:** `Point`



### `m_LastDeepCleanup`

- **Type:** `Point`




# Structs
---

### `SessionStats`

- **Description:** 

- **Members:**

  - `peakMemoryUsage`: `size_t` - 

  - `currentMemoryUsage`: `size_t` - 

  - `uptime`: `hours` - 

  - `defragmentationCount`: `uint32_t` - 

  - `fragmentationRatio`: `float` - 




# Functions
---

## requestAssetCaching



- **Parameters:** ``

- **Return:** `Notify editor to cache assets`

---

## compactMemoryPools



- **Parameters:** ``

- **Return:** `Compact memory pools`

---

## releaseUnusedResources



- **Parameters:** ``

- **Return:** `Release unused editor resources`

---

## defragmentEditorMemory



- **Parameters:** ``

- **Return:** `Defragment memory`

---
