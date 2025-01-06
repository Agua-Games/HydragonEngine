# Classes
---

## DebugSystem
---




# Variables
---

### `enableDebugDraw`

- **Type:** `bool`

- **Default Value:** `true`



### `enableInspector`

- **Type:** `bool`

- **Default Value:** `true`



### `enableConsole`

- **Type:** `bool`

- **Default Value:** `true`



### `persistentDrawDuration`

- **Type:** `bool`

- **Default Value:** `0.0f`



### `maxDrawCommands`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `config`

- **Type:** `const DebugConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `DebugConfig`



### `m_Stats`

- **Type:** `DebugStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `DebugConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static DebugSystem&`

---

## Initialize



- **Parameters:** `const DebugConfig& config = {}`

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

## DrawLine



- **Parameters:** `const Vector3& start`, `const Vector3& end`, `const Color& color`

- **Return:** `void`

---

## DrawSphere



- **Parameters:** `const Vector3& center`, `float radius`, `const Color& color`

- **Return:** `void`

---

## DrawBox



- **Parameters:** `const Vector3& center`, `const Vector3& extents`, `const Color& color`

- **Return:** `void`

---

## DrawText



- **Parameters:** `const Vector3& position`, `const std::string& text`, `const Color& color`

- **Return:** `void`

---

## AddInspectorValue



- **Parameters:** `const std::string& name`, `const std::string& value`

- **Return:** `void`

---

## AddConsoleCommand



- **Parameters:** `const std::string& name`, `const ConsoleCommandFunc& func`

- **Return:** `void`

---
