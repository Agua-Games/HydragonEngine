# Classes
---

## ConsoleSystem
---




# Variables
---

### `enableHistory`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAutoComplete`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSuggestions`

- **Type:** `bool`

- **Default Value:** `true`



### `enableScripting`

- **Type:** `bool`

- **Default Value:** `true`



### `maxHistoryEntries`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxSuggestions`

- **Type:** `uint32_t`

- **Default Value:** `10`



### `config`

- **Type:** `const ConsoleConfig&`

- **Default Value:** `{})`



### `description`

- **Type:** `string&`

- **Default Value:** `"")`



### `level`

- **Type:** `OutputLevel`

- **Default Value:** `OutputLevel::Info)`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ConsoleConfig`



### `m_Stats`

- **Type:** `ConsoleStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ConsoleConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static ConsoleSystem&`

---

## Initialize



- **Parameters:** `const ConsoleConfig& config = {}`

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

## RegisterCommand



- **Parameters:** `const std::string& name`, `const CommandCallback& callback`, `const std::string& description = ""`

- **Return:** `void`

---

## UnregisterCommand



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## ExecuteCommand



- **Parameters:** `const std::string& command`

- **Return:** `void`

---

## ExecuteScript



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## AddOutput



- **Parameters:** `const std::string& text`, `OutputLevel level = OutputLevel::Info`

- **Return:** `void`

---

## ClearOutput



- **Parameters:** ``

- **Return:** `void`

---
