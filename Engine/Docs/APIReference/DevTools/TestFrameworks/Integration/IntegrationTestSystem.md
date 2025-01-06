# Classes
---

## IntegrationTestSystem
---




# Variables
---

### `enableEnvironmentIsolation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableStateTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableResourceMonitoring`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePerformanceMetrics`

- **Type:** `bool`

- **Default Value:** `true`



### `maxTestDuration`

- **Type:** `uint32_t`

- **Default Value:** `300000`



### `maxMemoryUsage`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024 * 1024`



### `config`

- **Type:** `const IntegrationTestConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `IntegrationTestConfig`



### `m_Stats`

- **Type:** `IntegrationStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `IntegrationTestConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static IntegrationTestSystem&`

---

## Initialize



- **Parameters:** `const IntegrationTestConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## RegisterScenario



- **Parameters:** `const std::string& name`, `const ScenarioCallback& callback`

- **Return:** `void`

---

## RegisterEnvironment



- **Parameters:** `const std::string& name`, `const EnvironmentSetup& setup`

- **Return:** `void`

---

## RunAllScenarios



- **Parameters:** ``

- **Return:** `void`

---

## RunScenario



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## SetupTestEnvironment



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## TeardownTestEnvironment



- **Parameters:** ``

- **Return:** `void`

---

## CaptureState



- **Parameters:** `const std::string& checkpoint`

- **Return:** `void`

---

## RestoreState



- **Parameters:** `const std::string& checkpoint`

- **Return:** `void`

---

## GenerateReport



- **Parameters:** `const std::string& path`

- **Return:** `void`

---
