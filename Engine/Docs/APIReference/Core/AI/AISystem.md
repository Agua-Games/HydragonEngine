# Classes
---

## AISystem
---




# Variables
---

### `maxAgents`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxBehaviorTrees`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `enablePathfinding`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePerception`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePrediction`

- **Type:** `bool`

- **Default Value:** `true`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `64 * 1024 * 1024`



### `config`

- **Type:** `const AIConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AIConfig`



### `m_Stats`

- **Type:** `AIStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AIConfig`

- **Description:** 

- **Members:**

  - `1024`: `64 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AISystem&`

---

## Initialize



- **Parameters:** `const AIConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** `float deltaTime`

- **Return:** `void`

---

## CreateAgent



- **Parameters:** `const AgentDesc& desc`

- **Return:** `AgentHandle`

---

## DestroyAgent



- **Parameters:** `AgentHandle handle`

- **Return:** `void`

---

## SetBehavior



- **Parameters:** `AgentHandle agent`, `const std::string& behaviorPath`

- **Return:** `void`

---

## SetTarget



- **Parameters:** `AgentHandle agent`, `const Vector3& position`

- **Return:** `void`

---

## RequestPath



- **Parameters:** `AgentHandle agent`, `const Vector3& destination`

- **Return:** `bool`

---

## UpdatePerception



- **Parameters:** `AgentHandle agent`, `const PerceptionEvent& event`

- **Return:** `void`

---
