# Classes
---

## ModulePriority
---



## AllocationStrategy
---




# Variables
---

### `tag`

- **Type:** `string`



### `strategy`

- **Type:** `AllocationStrategy`



### `priority`

- **Type:** `ModulePriority`



### `alignment`

- **Type:** `size_t`



### `isTemporary`

- **Type:** `bool`



### `totalAllocated`

- **Type:** `size_t`



### `currentUsage`

- **Type:** `size_t`



### `peakUsage`

- **Type:** `size_t`



### `allocationCount`

- **Type:** `size_t`



### `lastUpdate`

- **Type:** `time_point`




# Structs
---

### `AllocationInfo`

- **Description:** 

- **Members:**

  - `tag`: `string` - 

  - `strategy`: `AllocationStrategy` - 

  - `priority`: `ModulePriority` - 

  - `alignment`: `size_t` - 

  - `isTemporary`: `bool` - 



### `MemoryStats`

- **Description:** 

- **Members:**

  - `totalAllocated`: `size_t` - 

  - `currentUsage`: `size_t` - 

  - `peakUsage`: `size_t` - 

  - `allocationCount`: `size_t` - 

  - `lastUpdate`: `time_point` - 


