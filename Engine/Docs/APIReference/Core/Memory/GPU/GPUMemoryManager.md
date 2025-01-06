# Classes
---

## GPUMemoryManager
---




# Variables
---

### `memory`

- **Type:** `VkDeviceMemory`



### `size`

- **Type:** `VkDeviceSize`



### `offset`

- **Type:** `VkDeviceSize`



### `memoryType`

- **Type:** `uint32_t`



### `memory`

- **Type:** `VkDeviceMemory`



### `size`

- **Type:** `size_t`



### `used`

- **Type:** `size_t`




# Structs
---

### `GPUAllocation`

- **Description:** 

- **Members:**

  - `memory`: `VkDeviceMemory` - 

  - `size`: `VkDeviceSize` - 

  - `offset`: `VkDeviceSize` - 

  - `memoryType`: `uint32_t` - 



### `GPUMemoryHeap`

- **Description:** 

- **Members:**

  - `memory`: `VkDeviceMemory` - 

  - `size`: `size_t` - 

  - `used`: `size_t` - 




# Functions
---

## AllocateGPUMemory



- **Parameters:** `VkMemoryRequirements reqs`, `VkMemoryPropertyFlags props`

- **Return:** `GPUAllocation`

---

## DeallocateGPUMemory



- **Parameters:** `GPUAllocation& allocation`

- **Return:** `void`

---
