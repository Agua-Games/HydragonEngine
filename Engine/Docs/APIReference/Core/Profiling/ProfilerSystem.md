# Classes
---

## ProfilerSystem
---




# Variables
---

### `timeline`

- **Type:** `PerformanceTimeline`



### `resources`

- **Type:** `ResourceUtilization`



### `bottlenecks`

- **Type:** `BottleneckAnalysis`



### `enableCPUProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableGPUProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMemoryProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableThreadProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableNetworkProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableFileIOProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `maxSampleCount`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `maxFrameHistory`

- **Type:** `uint32_t`

- **Default Value:** `300`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `64 * 1024 * 1024`



### `config`

- **Type:** `const ProfilerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ProfilerConfig`



### `m_Stats`

- **Type:** `ProfilerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `SystemMetrics`

- **Description:** 

- **Members:**

  - `timeline`: `PerformanceTimeline` - 

  - `resources`: `ResourceUtilization` - 

  - `bottlenecks`: `BottleneckAnalysis` - 



### `ProfilerConfig`

- **Description:** 

- **Members:**

  - `1024`: `64 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static ProfilerSystem&`

---

## Initialize



- **Parameters:** const ProfilerConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## BeginFrame



- **Parameters:** 

- **Return:** `void`

---

## EndFrame



- **Parameters:** 

- **Return:** `void`

---

## BeginScope



- **Parameters:** const char* name, ProfilerCategory category

- **Return:** `ScopeHandle`

---

## EndScope



- **Parameters:** ScopeHandle handle

- **Return:** `void`

---

## AddMarker



- **Parameters:** const char* name, ProfilerCategory category

- **Return:** `void`

---

## AddCounter



- **Parameters:** const char* name, float value

- **Return:** `void`

---

## BeginGPUScope



- **Parameters:** const char* name

- **Return:** `void`

---

## EndGPUScope



- **Parameters:** 

- **Return:** `void`

---

## SetThreadName



- **Parameters:** const std::string& name

- **Return:** `void`

---

## EnableCapture



- **Parameters:** bool enable

- **Return:** `void`

---

## ExportCapture



- **Parameters:** const std::string& path

- **Return:** `void`

---

## ClearCapture



- **Parameters:** 

- **Return:** `void`

---
