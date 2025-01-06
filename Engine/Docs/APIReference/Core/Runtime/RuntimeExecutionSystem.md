# Classes
---

## RuntimeExecutionSystem
---




# Variables
---

### `enableProcessIsolation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableResourceTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCrashRecovery`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMetrics`

- **Type:** `bool`

- **Default Value:** `true`



### `maxProcesses`

- **Type:** `uint32_t`

- **Default Value:** `32`



### `watchdogInterval`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `processMemoryLimit`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024 * 1024`



### `config`

- **Type:** `const RuntimeExecutionConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeExecutionConfig`



### `m_Stats`

- **Type:** `RuntimeExecutionStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeExecutionConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static RuntimeExecutionSystem&`

---

## Initialize



- **Parameters:** `const RuntimeExecutionConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## StartProcess



- **Parameters:** `const ProcessDesc& desc`

- **Return:** `ProcessHandle`

---

## StopProcess



- **Parameters:** `ProcessHandle handle`

- **Return:** `void`

---

## SuspendProcess



- **Parameters:** `ProcessHandle handle`

- **Return:** `void`

---

## ResumeProcess



- **Parameters:** `ProcessHandle handle`

- **Return:** `void`

---

## SetProcessPriority



- **Parameters:** `ProcessHandle handle`, `ProcessPriority priority`

- **Return:** `void`

---

## SetProcessAffinity



- **Parameters:** `ProcessHandle handle`, `uint64_t affinityMask`

- **Return:** `void`

---
