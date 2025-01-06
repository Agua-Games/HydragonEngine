# Classes
---

## ScriptEditorSystem
---




# Variables
---

### `enableAutoCompletion`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSyntaxHighlighting`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLiveCompilation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebugger`

- **Type:** `bool`

- **Default Value:** `true`



### `maxOpenScripts`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `undoLevels`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxFileSize`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024`



### `config`

- **Type:** `const ScriptEditorConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ScriptEditorConfig`



### `m_Stats`

- **Type:** `ScriptEditorStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ScriptEditorConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static ScriptEditorSystem&`

---

## Initialize



- **Parameters:** const ScriptEditorConfig& config = {}

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

## OpenScript



- **Parameters:** const std::string& path

- **Return:** `void`

---

## SaveScript



- **Parameters:** const std::string& path

- **Return:** `void`

---

## CloseScript



- **Parameters:** const std::string& path

- **Return:** `void`

---

## SetContent



- **Parameters:** const std::string& path, const std::string& content

- **Return:** `void`

---

## CompileScript



- **Parameters:** const std::string& path

- **Return:** `void`

---

## DebugScript



- **Parameters:** const std::string& path

- **Return:** `void`

---

## AddBreakpoint



- **Parameters:** const std::string& path, uint32_t line

- **Return:** `void`

---

## RemoveBreakpoint



- **Parameters:** const std::string& path, uint32_t line

- **Return:** `void`

---
