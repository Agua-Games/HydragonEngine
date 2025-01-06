# Classes
---

## NodeValidationSystem
---




# Variables
---

### `enableLiveValidation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTypeChecking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCycleDetection`

- **Type:** `bool`

- **Default Value:** `true`



### `enableConnectionLimits`

- **Type:** `bool`

- **Default Value:** `true`



### `maxInputConnections`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `maxOutputConnections`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `validationInterval`

- **Type:** `uint32_t`

- **Default Value:** `500`



### `config`

- **Type:** `const NodeValidationConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `NodeValidationConfig`



### `m_Stats`

- **Type:** `NodeValidationStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `NodeValidationConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static NodeValidationSystem&`

---

## Initialize



- **Parameters:** `const NodeValidationConfig& config = {}`

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

## ValidateNode



- **Parameters:** `NodeHandle handle`

- **Return:** `void`

---

## ValidateConnection



- **Parameters:** `ConnectionHandle handle`

- **Return:** `void`

---

## ValidateGraph



- **Parameters:** ``

- **Return:** `void`

---

## AddValidationRule



- **Parameters:** `const std::string& nodeType`, `const ValidationRule& rule`

- **Return:** `void`

---

## RemoveValidationRule



- **Parameters:** `const std::string& nodeType`, `const std::string& ruleName`

- **Return:** `void`

---
