# Classes
---

## MemoryBenchmark
---



## WorkloadType
---




# Variables
---

### `name`

- **Type:** `string`



### `averageTimeMs`

- **Type:** `double`



### `minTimeMs`

- **Type:** `double`



### `maxTimeMs`

- **Type:** `double`



### `operationCount`

- **Type:** `size_t`



### `totalBytes`

- **Type:** `size_t`



### `throughputMBps`

- **Type:** `double`



### `baselineMemory`

- **Type:** `size_t`



### `actualMemory`

- **Type:** `size_t`



### `overheadPercent`

- **Type:** `double`



### `fragmentationRatio`

- **Type:** `double`



### `largestFreeBlock`

- **Type:** `size_t`



### `totalFreeBlocks`

- **Type:** `size_t`



### `averageBlockSize`

- **Type:** `double`



### `contentionRatio`

- **Type:** `double`



### `maxConcurrentThreads`

- **Type:** `uint32_t`



### `averageWaitTimeMs`

- **Type:** `double`



### `fragMetrics`

- **Type:** `FragmentationMetrics`



### `threadMetrics`

- **Type:** `ThreadingMetrics`



### `suspiciousPatterns`

- **Type:** `uint32_t`



### `burstAccessRatio`

- **Type:** `double`



### `address`

- **Type:** `void*`



### `accessCount`

- **Type:** `size_t`



### `accessFrequency`

- **Type:** `double`



### `accessPatternMetrics`

- **Type:** `AccessPatternMetrics`



### `iterationCount`

- **Type:** `size_t`

- **Default Value:** `1000`



### `minAllocationSize`

- **Type:** `size_t`

- **Default Value:** `8`



### `maxAllocationSize`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024`



### `includeNetworkTests`

- **Type:** `bool`

- **Default Value:** `false`



### `includeSecurityTests`

- **Type:** `bool`

- **Default Value:** `true`



### `threadCount`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `measureContention`

- **Type:** `bool`

- **Default Value:** `true`



### `operationsPerThread`

- **Type:** `size_t`

- **Default Value:** `1000`



### `threadConfig`

- **Type:** `ThreadConfig`



### `measureFragmentation`

- **Type:** `bool`

- **Default Value:** `true`



### `workloadType`

- **Type:** `WorkloadType`

- **Default Value:** `WorkloadType::Mixed`



### `trackAccessPatterns`

- **Type:** `bool`

- **Default Value:** `true`



### `maxHotSpots`

- **Type:** `uint32_t`

- **Default Value:** `10`



### `burstThreshold`

- **Type:** `double`

- **Default Value:** `1000.0`



### `patternConfig`

- **Type:** `PatternConfig`



### `config`

- **Type:** `const BenchmarkConfig&`

- **Default Value:** `BenchmarkConfig())`



### `address`

- **Type:** `void*`



### `accessCount`

- **Type:** `size_t`



### `timestamp`

- **Type:** `time_point`



### `isBurst`

- **Type:** `bool`



### `s_PatternMutex`

- **Type:** `mutex`




# Structs
---

### `BenchmarkResult`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `averageTimeMs`: `double` - 

  - `minTimeMs`: `double` - 

  - `maxTimeMs`: `double` - 

  - `operationCount`: `size_t` - 

  - `totalBytes`: `size_t` - 

  - `throughputMBps`: `double` - 

  - `baselineMemory`: `size_t` - 

  - `actualMemory`: `size_t` - 

  - `overheadPercent`: `double` - 

  - `fragmentationRatio`: `double` - 

  - `largestFreeBlock`: `size_t` - 

  - `totalFreeBlocks`: `size_t` - 

  - `averageBlockSize`: `double` - 



### `ThreadingMetrics`

- **Description:** 

- **Members:**

  - `contentionRatio`: `double` - 

  - `maxConcurrentThreads`: `uint32_t` - 

  - `averageWaitTimeMs`: `double` - 



### `AccessPatternMetrics`

- **Description:** 

- **Members:**

  - `suspiciousPatterns`: `uint32_t` - 

  - `burstAccessRatio`: `double` - 

  - `address`: `void*` - 

  - `accessCount`: `size_t` - 

  - `accessFrequency`: `double` - 



### `BenchmarkConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 



### `PatternConfig`

- **Description:** 

- **Members:**



### `AccessPattern`

- **Description:** 

- **Members:**

  - `address`: `void*` - 

  - `accessCount`: `size_t` - 

  - `timestamp`: `time_point` - 

  - `isBurst`: `bool` - 




# Functions
---

## BenchmarkBasicAllocation



- **Parameters:** const BenchmarkConfig& config

- **Return:** `static BenchmarkResult`

---

## BenchmarkSecureAllocation



- **Parameters:** const BenchmarkConfig& config

- **Return:** `static BenchmarkResult`

---

## BenchmarkPoolAllocation



- **Parameters:** const BenchmarkConfig& config

- **Return:** `static BenchmarkResult`

---

## BenchmarkNetworkSecurity



- **Parameters:** const BenchmarkConfig& config

- **Return:** `static BenchmarkResult`

---

## MeasureTime



- **Parameters:** Func&& func

- **Return:** `static double`

---
