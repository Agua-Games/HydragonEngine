# Classes
---

## TerrainProfiler
---




# Variables
---

### `averageStreamingTime`

- **Type:** `double`

- **Default Value:** `0.0`



### `peakStreamingTime`

- **Type:** `double`

- **Default Value:** `0.0`



### `streamingOperations`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `totalBytesStreamed`

- **Type:** `size_t`

- **Default Value:** `0`



### `averageLODUpdateTime`

- **Type:** `double`

- **Default Value:** `0.0`



### `lodTransitions`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `averageLODLevel`

- **Type:** `float`

- **Default Value:** `0.0f`



### `MAX_FRAME_HISTORY`

- **Type:** `static constexpr size_t`

- **Default Value:** `300`



### `startTime`

- **Type:** `Point`



### `endTime`

- **Type:** `Point`



### `m_CurrentFrame`

- **Type:** `FrameData`



### `m_StreamingMetrics`

- **Type:** `StreamingMetrics`



### `m_LODMetrics`

- **Type:** `LODMetrics`




# Structs
---

### `StreamingMetrics`

- **Description:** 

- **Members:**



### `LODMetrics`

- **Description:** 

- **Members:**



### `FrameData`

- **Description:** 

- **Members:**

  - `startTime`: `Point` - 

  - `endTime`: `Point` - 




# Functions
---

## updateMetrics



- **Parameters:** ``

- **Return:** `void`

---
