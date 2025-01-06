# Classes
---

## MemoryProfiler
---




# Variables
---

### `address`

- **Type:** `void*`



### `size`

- **Type:** `size_t`



### `tag`

- **Type:** `string`



### `timestamp`

- **Type:** `time_point`



### `threadId`

- **Type:** `uint32_t`



### `callstack`

- **Type:** `string`



### `totalAllocations`

- **Type:** `size_t`



### `peakMemoryUsage`

- **Type:** `size_t`



### `fragmentationRatio`

- **Type:** `float`




# Structs
---

### `AllocationRecord`

- **Description:** 

- **Members:**

  - `address`: `void*` - 

  - `size`: `size_t` - 

  - `tag`: `string` - 

  - `timestamp`: `time_point` - 

  - `threadId`: `uint32_t` - 

  - `callstack`: `string` - 



### `ProfilerStats`

- **Description:** 

- **Members:**

  - `totalAllocations`: `size_t` - 

  - `peakMemoryUsage`: `size_t` - 

  - `fragmentationRatio`: `float` - 




# Functions
---

## Initialize



- **Parameters:** 

- **Return:** `static void`

---

## Shutdown



- **Parameters:** 

- **Return:** `static void`

---

## TrackAllocation



- **Parameters:** const AllocationRecord& record

- **Return:** `static void`

---

## UntrackAllocation



- **Parameters:** void* ptr

- **Return:** `static void`

---

## GetStats



- **Parameters:** 

- **Return:** `static ProfilerStats`

---

## ResetStats



- **Parameters:** 

- **Return:** `static void`

---

## UpdateFragmentationMetrics



- **Parameters:** 

- **Return:** `static void`

---

## PruneAllocationHistory



- **Parameters:** 

- **Return:** `static void`

---
