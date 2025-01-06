# Classes
---

## MemoryTracker
---




# Variables
---

### `enableTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableStackTraces`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLeakDetection`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAllocationHistory`

- **Type:** `bool`

- **Default Value:** `true`



### `maxTrackingEntries`

- **Type:** `size_t`

- **Default Value:** `10000`



### `config`

- **Type:** `const MemoryConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `size`

- **Type:** `size_t`



### `file`

- **Type:** `const char*`



### `line`

- **Type:** `int`



### `scope`

- **Type:** `string`



### `m_Config`

- **Type:** `MemoryConfig`



### `m_Stats`

- **Type:** `MemoryStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `MemoryConfig`

- **Description:** 

- **Members:**



### `AllocationEntry`

- **Description:** 

- **Members:**

  - `size`: `size_t` - 

  - `file`: `const char*` - 

  - `line`: `int` - 

  - `scope`: `string` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static MemoryTracker&`

---

## Initialize



- **Parameters:** const MemoryConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## TrackAllocation



- **Parameters:** void* ptr, size_t size, const char* file, int line

- **Return:** `void*`

---

## TrackDeallocation



- **Parameters:** void* ptr

- **Return:** `void`

---

## BeginScope



- **Parameters:** const std::string& name

- **Return:** `void`

---

## EndScope



- **Parameters:** 

- **Return:** `void`

---

## DumpStats



- **Parameters:** 

- **Return:** `void`

---

## CheckLeaks



- **Parameters:** 

- **Return:** `void`

---
