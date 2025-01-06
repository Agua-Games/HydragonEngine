# Classes
---

## LayoutManagerSystem
---




# Variables
---

### `enableAutoSave`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVersioning`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePresets`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSharing`

- **Type:** `bool`

- **Default Value:** `true`



### `maxLayouts`

- **Type:** `uint32_t`

- **Default Value:** `20`



### `autoSaveInterval`

- **Type:** `uint32_t`

- **Default Value:** `300`



### `layoutDirectory`

- **Type:** `string`

- **Default Value:** `"Editor/Layouts"`



### `config`

- **Type:** `const LayoutManagerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `LayoutManagerConfig`



### `m_Stats`

- **Type:** `LayoutManagerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `LayoutManagerConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static LayoutManagerSystem&`

---

## Initialize



- **Parameters:** const LayoutManagerConfig& config = {}

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

## SaveLayout



- **Parameters:** const std::string& name

- **Return:** `void`

---

## LoadLayout



- **Parameters:** const std::string& name

- **Return:** `void`

---

## DeleteLayout



- **Parameters:** const std::string& name

- **Return:** `void`

---

## ExportLayout



- **Parameters:** const std::string& name, const std::string& path

- **Return:** `void`

---

## ImportLayout



- **Parameters:** const std::string& path

- **Return:** `void`

---

## SetDefaultLayout



- **Parameters:** const std::string& name

- **Return:** `void`

---

## ResetToDefault



- **Parameters:** 

- **Return:** `void`

---
