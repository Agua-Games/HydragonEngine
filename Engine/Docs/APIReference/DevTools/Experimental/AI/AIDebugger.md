# Classes
---

## AIDebugger
---




# Variables
---

### `enableBehaviorTreeViz`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDecisionGraphs`

- **Type:** `bool`

- **Default Value:** `true`



### `enableStateInspection`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePerformanceAnalysis`

- **Type:** `bool`

- **Default Value:** `true`



### `maxTrackedEntities`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `updateInterval`

- **Type:** `float`

- **Default Value:** `0.1f`



### `config`

- **Type:** `const AIDebuggerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AIDebuggerConfig`



### `m_Stats`

- **Type:** `AIDebuggerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AIDebuggerConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static AIDebugger&`

---

## Initialize



- **Parameters:** const AIDebuggerConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** float deltaTime

- **Return:** `void`

---

## TrackEntity



- **Parameters:** EntityHandle entity

- **Return:** `void`

---

## UntrackEntity



- **Parameters:** EntityHandle entity

- **Return:** `void`

---

## VisualizeDecisionTree



- **Parameters:** EntityHandle entity

- **Return:** `void`

---

## VisualizeBehaviorTree



- **Parameters:** EntityHandle entity

- **Return:** `void`

---

## VisualizePathfinding



- **Parameters:** EntityHandle entity

- **Return:** `void`

---

## CaptureAISnapshot



- **Parameters:** const std::string& name

- **Return:** `void`

---

## CompareAISnapshots



- **Parameters:** const std::string& snapshot1, const std::string& snapshot2

- **Return:** `void`

---
