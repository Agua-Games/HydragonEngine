# Classes
---

## MemoryBindings
---




# Variables
---

### `enableAllocationTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMemoryProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLeakDetection`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSafetyChecks`

- **Type:** `bool`

- **Default Value:** `true`



### `maxTrackingEvents`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `profilerBufferSize`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024`



### `memoryStatsPath`

- **Type:** `string`

- **Default Value:** `"Stats/Memory"`



### `config`

- **Type:** `const MemoryBindingsConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `MemoryBindingsConfig`



### `m_Stats`

- **Type:** `MemoryBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `MemoryBindingsConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static MemoryBindings&`

---

## Initialize



- **Parameters:** `const MemoryBindingsConfig& config = {}`

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

## TrackAllocation



- **Parameters:** `const Memory::AllocationInfo& info`

- **Return:** `void`

---

## TrackDeallocation



- **Parameters:** `const Memory::AllocationInfo& info`

- **Return:** `void`

---

## BeginMemoryScope



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## EndMemoryScope



- **Parameters:** ``

- **Return:** `void`

---

## ExportMemoryStats



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## ClearMemoryStats



- **Parameters:** ``

- **Return:** `void`

---

## EnableLeakDetection



- **Parameters:** `bool enable`

- **Return:** `void`

---

## EnableMemoryGuards



- **Parameters:** `bool enable`

- **Return:** `void`

---

## AnalyzeFragmentation



- **Parameters:** ``

- **Return:** `void`

---

## GenerateAllocationReport



- **Parameters:** ``

- **Return:** `void`

---
