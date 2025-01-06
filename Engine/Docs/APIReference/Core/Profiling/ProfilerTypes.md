# Classes
---

## ProfilerCategory
---




# Variables
---

### `ScopeHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `TimePoint`

- **Type:** `using`

- **Default Value:** `std::chrono::high_resolution_clock::time_point`



### `Duration`

- **Type:** `using`

- **Default Value:** `std::chrono::nanoseconds`



### `name`

- **Type:** `const char*`



### `category`

- **Type:** `ProfilerCategory`



### `startTime`

- **Type:** `TimePoint`



### `duration`

- **Type:** `Duration`



### `threadId`

- **Type:** `uint32_t`



### `depth`

- **Type:** `uint32_t`



### `memoryUsage`

- **Type:** `size_t`



### `frameNumber`

- **Type:** `uint64_t`



### `startTime`

- **Type:** `TimePoint`



### `cpuTime`

- **Type:** `Duration`



### `gpuTime`

- **Type:** `Duration`



### `frameTimeMs`

- **Type:** `float`



### `peakMemoryUsage`

- **Type:** `size_t`



### `activeScopes`

- **Type:** `uint32_t`



### `totalEvents`

- **Type:** `uint32_t`



### `droppedEvents`

- **Type:** `uint32_t`



### `averageFrameTimeMs`

- **Type:** `float`



### `minFrameTimeMs`

- **Type:** `float`



### `maxFrameTimeMs`

- **Type:** `float`



### `averageCPUTimeMs`

- **Type:** `float`



### `averageGPUTimeMs`

- **Type:** `float`



### `peakMemoryUsage`

- **Type:** `size_t`



### `startTime`

- **Type:** `double`



### `endTime`

- **Type:** `double`



### `markerCount`

- **Type:** `uint32_t`



### `cpuUsage`

- **Type:** `float`



### `gpuUsage`

- **Type:** `float`



### `memoryUsage`

- **Type:** `size_t`



### `networkBandwidth`

- **Type:** `float`



### `diskIORate`

- **Type:** `float`



### `severityScore`

- **Type:** `float`



### `primaryBottleneck`

- **Type:** `string`



### `occurrenceCount`

- **Type:** `uint32_t`




# Structs
---

### `ProfilerEvent`

- **Description:** 

- **Members:**

  - `name`: `const char*` - 

  - `category`: `ProfilerCategory` - 

  - `startTime`: `TimePoint` - 

  - `duration`: `Duration` - 

  - `threadId`: `uint32_t` - 

  - `depth`: `uint32_t` - 

  - `memoryUsage`: `size_t` - 



### `ProfilerFrame`

- **Description:** 

- **Members:**

  - `frameNumber`: `uint64_t` - 

  - `startTime`: `TimePoint` - 

  - `cpuTime`: `Duration` - 

  - `gpuTime`: `Duration` - 

  - `frameTimeMs`: `float` - 

  - `peakMemoryUsage`: `size_t` - 



### `ProfilerStats`

- **Description:** 

- **Members:**

  - `activeScopes`: `uint32_t` - 

  - `totalEvents`: `uint32_t` - 

  - `droppedEvents`: `uint32_t` - 

  - `averageFrameTimeMs`: `float` - 

  - `minFrameTimeMs`: `float` - 

  - `maxFrameTimeMs`: `float` - 

  - `averageCPUTimeMs`: `float` - 

  - `averageGPUTimeMs`: `float` - 

  - `peakMemoryUsage`: `size_t` - 



### `PerformanceTimeline`

- **Description:** 

- **Members:**

  - `startTime`: `double` - 

  - `endTime`: `double` - 

  - `markerCount`: `uint32_t` - 



### `ResourceUtilization`

- **Description:** 

- **Members:**

  - `cpuUsage`: `float` - 

  - `gpuUsage`: `float` - 

  - `memoryUsage`: `size_t` - 

  - `networkBandwidth`: `float` - 

  - `diskIORate`: `float` - 



### `BottleneckAnalysis`

- **Description:** 

- **Members:**

  - `severityScore`: `float` - 

  - `primaryBottleneck`: `string` - 

  - `occurrenceCount`: `uint32_t` - 


