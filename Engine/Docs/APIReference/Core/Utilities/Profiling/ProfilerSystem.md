# Classes
---

## ProfilerSystem
---




# Variables
---

### `enableProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableThreadProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableGPUProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMemoryProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `maxSamples`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `samplingInterval`

- **Type:** `float`

- **Default Value:** `0.016f`



### `config`

- **Type:** `const ProfilerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ProfilerConfig`



### `m_Stats`

- **Type:** `ProfilerStats`



### `m_FrameStart`

- **Type:** `time_point`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ProfilerConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static ProfilerSystem&`

---

## Initialize



- **Parameters:** `const ProfilerConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## BeginFrame



- **Parameters:** ``

- **Return:** `void`

---

## EndFrame



- **Parameters:** ``

- **Return:** `void`

---

## BeginScope



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## EndScope



- **Parameters:** ``

- **Return:** `void`

---

## AddMarker



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## AddCounter



- **Parameters:** `const std::string& name`, `float value`

- **Return:** `void`

---

## BeginGPUScope



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## EndGPUScope



- **Parameters:** ``

- **Return:** `void`

---
