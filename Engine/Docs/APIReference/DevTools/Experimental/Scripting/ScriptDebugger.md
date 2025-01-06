# Classes
---

## ScriptDebugger
---




# Variables
---

### `enableLiveDebugging`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePerformanceAnalysis`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMemoryTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `maxBreakpoints`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxWatchVariables`

- **Type:** `uint32_t`

- **Default Value:** `100`



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



- **Parameters:** 

- **Return:** `static ScriptDebugger&`

---

## Initialize



- **Parameters:** const ScriptDebuggerConfig& config = {}

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

## AttachToScript



- **Parameters:** ScriptHandle handle

- **Return:** `void`

---

## DetachFromScript



- **Parameters:** ScriptHandle handle

- **Return:** `void`

---

## AddBreakpoint



- **Parameters:** const ScriptLocation& location

- **Return:** `void`

---

## RemoveBreakpoint



- **Parameters:** const ScriptLocation& location

- **Return:** `void`

---

## AddWatch



- **Parameters:** const std::string& expression

- **Return:** `void`

---

## RemoveWatch



- **Parameters:** const std::string& expression

- **Return:** `void`

---

## StepInto



- **Parameters:** 

- **Return:** `void`

---

## StepOver



- **Parameters:** 

- **Return:** `void`

---

## StepOut



- **Parameters:** 

- **Return:** `void`

---

## Continue



- **Parameters:** 

- **Return:** `void`

---
