# Classes
---

## NodeExecutionSystem
---




# Variables
---

### `enableLiveExecution`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebugVisualization`

- **Type:** `bool`

- **Default Value:** `true`



### `enableProfiling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `maxExecutionDepth`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `cacheSize`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `executionTimeout`

- **Type:** `float`

- **Default Value:** `5000.0f`



### `config`

- **Type:** `const NodeExecutionConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `NodeExecutionConfig`



### `m_Stats`

- **Type:** `NodeExecutionStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `NodeExecutionConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static NodeExecutionSystem&`

---

## Initialize



- **Parameters:** const NodeExecutionConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## ExecuteNode



- **Parameters:** NodeHandle handle

- **Return:** `void`

---

## ExecuteGraph



- **Parameters:** const std::vector<NodeHandle>& outputNodes

- **Return:** `void`

---

## StopExecution



- **Parameters:** 

- **Return:** `void`

---

## SetNodeInput



- **Parameters:** NodeHandle handle, uint32_t pin, const NodeValue& value

- **Return:** `void`

---

## RegisterExecutor



- **Parameters:** const std::string& nodeType, 
                         std::unique_ptr<INodeExecutor> executor

- **Return:** `void`

---
