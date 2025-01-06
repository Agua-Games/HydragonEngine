# Classes
---

## NodeGraphSystem
---




# Variables
---

### `enableLivePreview`

- **Type:** `bool`

- **Default Value:** `true`



### `enableNodeSnapping`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAutoLayout`

- **Type:** `bool`

- **Default Value:** `true`



### `enableComments`

- **Type:** `bool`

- **Default Value:** `true`



### `maxNodes`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxConnections`

- **Type:** `uint32_t`

- **Default Value:** `2000`



### `gridSize`

- **Type:** `float`

- **Default Value:** `16.0f`



### `nodeSpacing`

- **Type:** `Vector2`

- **Default Value:** `{200.0f, 150.0f}`



### `config`

- **Type:** `const NodeGraphConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `NodeGraphConfig`



### `m_Stats`

- **Type:** `NodeGraphStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `NodeGraphConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static NodeGraphSystem&`

---

## Initialize



- **Parameters:** const NodeGraphConfig& config = {}

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

## CreateNode



- **Parameters:** const NodeDesc& desc

- **Return:** `NodeHandle`

---

## DeleteNode



- **Parameters:** NodeHandle handle

- **Return:** `void`

---

## DuplicateNode



- **Parameters:** NodeHandle handle

- **Return:** `void`

---

## ConnectNodes



- **Parameters:** NodeHandle output, uint32_t outputPin,
                                NodeHandle input, uint32_t inputPin

- **Return:** `ConnectionHandle`

---

## DisconnectNodes



- **Parameters:** ConnectionHandle handle

- **Return:** `void`

---

## SetNodePosition



- **Parameters:** NodeHandle handle, const Vector2& position

- **Return:** `void`

---

## SetNodeComment



- **Parameters:** NodeHandle handle, const std::string& comment

- **Return:** `void`

---

## GroupNodes



- **Parameters:** const std::vector<NodeHandle>& nodes, const std::string& name

- **Return:** `void`

---

## UngroupNodes



- **Parameters:** GroupHandle handle

- **Return:** `void`

---

## RegisterNodeType



- **Parameters:** const NodeTypeInfo& info

- **Return:** `void`

---

## UnregisterNodeType



- **Parameters:** const std::string& type

- **Return:** `void`

---
