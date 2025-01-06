# Classes
---

## ScriptDebuggerSystem
---




# Variables
---

### `enableVariableInspection`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCallStack`

- **Type:** `bool`

- **Default Value:** `true`



### `enableWatches`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `maxWatchCount`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxCallStackDepth`

- **Type:** `uint32_t`

- **Default Value:** `128`



### `updateInterval`

- **Type:** `float`

- **Default Value:** `0.1f`



### `config`

- **Type:** `const ScriptDebuggerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ScriptDebuggerConfig`



### `m_Stats`

- **Type:** `ScriptDebuggerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ScriptDebuggerConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static ScriptDebuggerSystem&`

---

## Initialize



- **Parameters:** `const ScriptDebuggerConfig& config = {}`

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

## AttachToScript



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## DetachFromScript



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## Continue



- **Parameters:** ``

- **Return:** `void`

---

## StepOver



- **Parameters:** ``

- **Return:** `void`

---

## StepInto



- **Parameters:** ``

- **Return:** `void`

---

## StepOut



- **Parameters:** ``

- **Return:** `void`

---

## AddWatch



- **Parameters:** `const std::string& expression`

- **Return:** `void`

---

## RemoveWatch



- **Parameters:** `const std::string& expression`

- **Return:** `void`

---

## EvaluateExpression



- **Parameters:** `const std::string& expression`

- **Return:** `void`

---
