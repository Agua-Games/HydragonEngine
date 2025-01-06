# Classes
---

## RuntimeAISystem
---




# Variables
---

### `enableBehaviorTrees`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePathfinding`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePerception`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePrediction`

- **Type:** `bool`

- **Default Value:** `true`



### `maxAgents`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxBehaviorTrees`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `perceptionUpdatesPerFrame`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `perceptionRange`

- **Type:** `float`

- **Default Value:** `100.0f`



### `behaviorTreePath`

- **Type:** `string`

- **Default Value:** `"Config/AI/Behaviors"`



### `config`

- **Type:** `const RuntimeAIConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeAIConfig`



### `m_Stats`

- **Type:** `RuntimeAIStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeAIConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static RuntimeAISystem&`

---

## Initialize



- **Parameters:** `const RuntimeAIConfig& config = {}`

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

## CreateAgent



- **Parameters:** `const AIAgentDesc& desc`

- **Return:** `AIAgentHandle`

---

## DestroyAgent



- **Parameters:** `AIAgentHandle handle`

- **Return:** `void`

---

## AssignBehavior



- **Parameters:** `AIAgentHandle handle`, `const std::string& behaviorId`

- **Return:** `void`

---

## SetAgentTarget



- **Parameters:** `AIAgentHandle handle`, `const Vector3& position`

- **Return:** `void`

---

## SetAgentState



- **Parameters:** `AIAgentHandle handle`, `const std::string& state`

- **Return:** `void`

---

## RequestPath



- **Parameters:** `AIAgentHandle handle`, `const Vector3& destination`

- **Return:** `bool`

---

## CancelPathRequest



- **Parameters:** `AIAgentHandle handle`

- **Return:** `void`

---

## UpdatePathfinding



- **Parameters:** `AIAgentHandle handle`

- **Return:** `void`

---

## AddPerceptionStimulus



- **Parameters:** `const Vector3& position`, `const StimulusDesc& desc`

- **Return:** `void`

---

## UpdatePerception



- **Parameters:** `AIAgentHandle handle`, `const PerceptionEvent& event`

- **Return:** `void`

---

## SetPerceptionRange



- **Parameters:** `AIAgentHandle handle`, `float range`

- **Return:** `void`

---

## RegisterBehaviorNode



- **Parameters:** `const std::string& type`, `std::unique_ptr<IAIBehaviorNode> node`

- **Return:** `void`

---

## UnregisterBehaviorNode



- **Parameters:** `const std::string& type`

- **Return:** `void`

---

## RegisterPerceptionSensor



- **Parameters:** `const std::string& type`, `std::unique_ptr<IPerceptionSensor> sensor`

- **Return:** `void`

---

## UnregisterPerceptionSensor



- **Parameters:** `const std::string& type`

- **Return:** `void`

---
