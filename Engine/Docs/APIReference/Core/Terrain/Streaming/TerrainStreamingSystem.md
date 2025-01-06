# Classes
---

## TerrainStreamingSystem
---




# Variables
---

### `maxActiveRegions`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `streamingBudgetMB`

- **Type:** `uint32_t`

- **Default Value:** `512`



### `streamingDistance`

- **Type:** `float`

- **Default Value:** `1000.0f`



### `enablePrioritization`

- **Type:** `bool`

- **Default Value:** `true`



### `maxPendingRequests`

- **Type:** `uint32_t`

- **Default Value:** `64`



### `m_Metrics`

- **Type:** `return`



### `regionId`

- **Type:** `uint32_t`



### `priority`

- **Type:** `float`



### `isLoad`

- **Type:** `bool`



### `region`

- **Type:** `StreamingRegion`



### `loadProgress`

- **Type:** `float`



### `isLoading`

- **Type:** `bool`



### `lastAccess`

- **Type:** `Point`



### `m_Config`

- **Type:** `StreamingConfig`



### `m_VirtualMemory`

- **Type:** `VirtualMemoryManager`



### `m_Metrics`

- **Type:** `StreamingMetrics`




# Structs
---

### `StreamingConfig`

- **Description:** 

- **Members:**



### `StreamingRequest`

- **Description:** 

- **Members:**

  - `regionId`: `uint32_t` - 

  - `priority`: `float` - 

  - `isLoad`: `bool` - 



### `ActiveRegion`

- **Description:** 

- **Members:**

  - `region`: `StreamingRegion` - 

  - `loadProgress`: `float` - 

  - `isLoading`: `bool` - 

  - `lastAccess`: `Point` - 




# Functions
---

## unregisterStreamingRegion



- **Parameters:** uint32_t regionId

- **Return:** `void`

---

## updateRegionPriorities



- **Parameters:** const Vector3& viewerPosition

- **Return:** `void`

---

## processStreamingQueue



- **Parameters:** 

- **Return:** `void`

---

## updateMetrics



- **Parameters:** const StreamingRequest& request

- **Return:** `void`

---

## cleanupUnusedRegions



- **Parameters:** 

- **Return:** `void`

---
