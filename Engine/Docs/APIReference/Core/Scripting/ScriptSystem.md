# Classes
---

## ScriptSystem
---




# Variables
---

### `maxScripts`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxCoroutines`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `enableHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebugger`

- **Type:** `bool`

- **Default Value:** `true`



### `enableProfiler`

- **Type:** `bool`

- **Default Value:** `true`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `128 * 1024 * 1024`



### `scriptRootPath`

- **Type:** `string`



### `config`

- **Type:** `const ScriptConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ScriptConfig`



### `m_Stats`

- **Type:** `ScriptStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ScriptConfig`

- **Description:** 

- **Members:**

  - `1024`: `128 * 1024 *` - 

  - `scriptRootPath`: `string` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static ScriptSystem&`

---

## Initialize



- **Parameters:** const ScriptConfig& config = {}

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

## CreateScript



- **Parameters:** const ScriptDesc& desc

- **Return:** `ScriptHandle`

---

## DestroyScript



- **Parameters:** ScriptHandle handle

- **Return:** `void`

---

## ExecuteScript



- **Parameters:** ScriptHandle handle

- **Return:** `void`

---

## StopScript



- **Parameters:** ScriptHandle handle

- **Return:** `void`

---

## SetGlobal



- **Parameters:** const std::string& name, const ScriptValue& value

- **Return:** `void`

---

## StartCoroutine



- **Parameters:** ScriptHandle script, const std::string& function

- **Return:** `CoroutineHandle`

---

## StopCoroutine



- **Parameters:** CoroutineHandle handle

- **Return:** `void`

---
