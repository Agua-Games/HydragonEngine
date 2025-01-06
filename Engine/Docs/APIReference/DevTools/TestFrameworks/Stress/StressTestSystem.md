# Classes
---

## StressTestSystem
---




# Variables
---

### `enableLoadGeneration`

- **Type:** `bool`

- **Default Value:** `true`



### `enableResourceMonitoring`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCrashRecovery`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMetricsCollection`

- **Type:** `bool`

- **Default Value:** `true`



### `maxConcurrentUsers`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `testDuration`

- **Type:** `uint32_t`

- **Default Value:** `3600`



### `rampUpTime`

- **Type:** `float`

- **Default Value:** `300.0f`



### `config`

- **Type:** `const StressTestConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `StressTestConfig`



### `m_Stats`

- **Type:** `StressTestStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `StressTestConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static StressTestSystem&`

---

## Initialize



- **Parameters:** const StressTestConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## RegisterScenario



- **Parameters:** const std::string& name, const LoadScenario& scenario

- **Return:** `void`

---

## RegisterMetric



- **Parameters:** const std::string& name, const MetricCallback& callback

- **Return:** `void`

---

## RunLoadTest



- **Parameters:** const std::string& scenario

- **Return:** `void`

---

## StopLoadTest



- **Parameters:** 

- **Return:** `void`

---

## SetLoadProfile



- **Parameters:** const LoadProfile& profile

- **Return:** `void`

---

## SetResourceLimits



- **Parameters:** const ResourceLimits& limits

- **Return:** `void`

---

## GenerateReport



- **Parameters:** const std::string& path

- **Return:** `void`

---

## ExportMetrics



- **Parameters:** const std::string& path

- **Return:** `void`

---
