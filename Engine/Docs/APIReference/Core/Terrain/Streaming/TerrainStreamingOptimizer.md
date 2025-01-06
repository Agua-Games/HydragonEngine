# Classes
---

## TerrainStreamingOptimizer
---




# Variables
---

### `enableDefragmentation`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePrediction`

- **Type:** `bool`

- **Default Value:** `true`



### `predictionThreshold`

- **Type:** `float`

- **Default Value:** `0.75f`



### `defragInterval`

- **Type:** `uint32_t`

- **Default Value:** `60`



### `size`

- **Type:** `size_t`



### `regionId`

- **Type:** `uint32_t`



### `isPredicted`

- **Type:** `bool`



### `m_memoryManager`

- **Type:** `GPUMemoryManager&`



### `m_defragmenter`

- **Type:** `GPUMemoryDefrag`



### `m_config`

- **Type:** `OptimizationConfig`



### `m_framesSinceDefrag`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `operator`

- **Type:** `TerrainStreamingOptimizer&`

- **Default Value:** `(const TerrainStreamingOptimizer&) = delete`




# Structs
---

### `OptimizationConfig`

- **Description:** 

- **Members:**



### `StreamingAllocation`

- **Description:** 

- **Members:**

  - `size`: `size_t` - 

  - `regionId`: `uint32_t` - 

  - `isPredicted`: `bool` - 


