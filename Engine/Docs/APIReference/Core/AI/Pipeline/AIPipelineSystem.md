# Classes
---

## AIPipelineSystem
---




# Variables
---

### `sceneHandle`

- **Type:** `SceneHandle`



### `behaviorTree`

- **Type:** `BehaviorTreeHandle`



### `generationContext`

- **Type:** `GenerationContextHandle`



### `enableIntegration`

- **Type:** `bool`

- **Default Value:** `true`



### `enableProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableValidation`

- **Type:** `bool`

- **Default Value:** `true`



### `maxIntegrationPoints`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `128 * 1024 * 1024`



### `config`

- **Type:** `const PipelineConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `PipelineConfig`



### `m_Stats`

- **Type:** `PipelineStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AIIntegrationPoint`

- **Description:** 

- **Members:**

  - `sceneHandle`: `SceneHandle` - 

  - `behaviorTree`: `BehaviorTreeHandle` - 

  - `generationContext`: `GenerationContextHandle` - 



### `PipelineConfig`

- **Description:** 

- **Members:**

  - `1024`: `128 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AIPipelineSystem&`

---

## Initialize



- **Parameters:** `const PipelineConfig& config = {}`

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

## CreateIntegrationPoint



- **Parameters:** `const AIIntegrationPoint& point`

- **Return:** `IntegrationHandle`

---

## DestroyIntegrationPoint



- **Parameters:** `IntegrationHandle handle`

- **Return:** `void`

---

## UpdateIntegrationPoint



- **Parameters:** `IntegrationHandle handle`, `const AIIntegrationPoint& point`

- **Return:** `void`

---
