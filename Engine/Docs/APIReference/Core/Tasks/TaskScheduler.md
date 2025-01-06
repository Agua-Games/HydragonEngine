# Classes
---

## TaskScheduler
---




# Variables
---

### `maxPendingTasks`

- **Type:** `uint32_t`

- **Default Value:** `1024`



### `maxConcurrentTasks`

- **Type:** `uint32_t`

- **Default Value:** `64`



### `enableTaskSteal`

- **Type:** `bool`

- **Default Value:** `true`



### `trackDependencies`

- **Type:** `bool`

- **Default Value:** `true`



### `config`

- **Type:** `const SchedulerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `SchedulerConfig`



### `m_Stats`

- **Type:** `TaskStats`




# Structs
---

### `SchedulerConfig`

- **Description:** 

- **Members:**




# Functions
---

## TaskScheduler



- **Parameters:** const SchedulerConfig& config = {}

- **Return:** `explicit`

---

## Initialize



- **Parameters:** 

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## ScheduleTask



- **Parameters:** const TaskDesc& desc

- **Return:** `TaskHandle`

---

## WaitForTask



- **Parameters:** TaskHandle handle

- **Return:** `void`

---

## CancelTask



- **Parameters:** TaskHandle handle

- **Return:** `void`

---

## ProcessTaskQueue



- **Parameters:** 

- **Return:** `void`

---

## UpdateDependencies



- **Parameters:** TaskHandle completedTask

- **Return:** `void`

---
