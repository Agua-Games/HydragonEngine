# Classes
---

## StreamingMemoryManager
---




# Variables
---

### `reservedStreamingMemory`

- **Type:** `size_t`



### `maxStreamingBlocks`

- **Type:** `size_t`



### `enablePrioritization`

- **Type:** `bool`



### `evictionThreshold`

- **Type:** `float`



### `reservedMemory`

- **Type:** `size_t`



### `currentUsage`

- **Type:** `size_t`



### `priority`

- **Type:** `StreamingPriority`



### `m_StreamingSystem`

- **Type:** `IStreamingSystem*`



### `m_Strategy`

- **Type:** `IMemoryStrategy*`




# Structs
---

### `StreamingConfig`

- **Description:** 

- **Members:**

  - `reservedStreamingMemory`: `size_t` - 

  - `maxStreamingBlocks`: `size_t` - 

  - `enablePrioritization`: `bool` - 

  - `evictionThreshold`: `float` - 



### `ModuleInfo`

- **Description:** 

- **Members:**

  - `reservedMemory`: `size_t` - 

  - `currentUsage`: `size_t` - 

  - `priority`: `StreamingPriority` - 




# Functions
---

## registerStreamingSystem



- **Parameters:** IStreamingSystem* system

- **Return:** `void`

---

## setMemoryStrategy



- **Parameters:** IMemoryStrategy* strategy

- **Return:** `void`

---

## registerModule



- **Parameters:** const std::string& moduleName, size_t reservedMemory

- **Return:** `void`

---

## setModulePriority



- **Parameters:** const std::string& moduleName, StreamingPriority priority

- **Return:** `void`

---

## allocateStreamingMemory



- **Parameters:** size_t size, const StreamingAllocationInfo& info

- **Return:** `void*`

---

## deallocateStreamingMemory



- **Parameters:** void* ptr

- **Return:** `void`

---

## updateStreamingState



- **Parameters:** 

- **Return:** `void`

---
