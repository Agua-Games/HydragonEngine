# Classes
---

## ConfigSystem
---




# Variables
---

### `IConfigProcessor`

- **Type:** `class`



### `ISchemaValidator`

- **Type:** `class`



### `IVersionController`

- **Type:** `class`



### `IEncryptionManager`

- **Type:** `class`



### `IAutoSaveManager`

- **Type:** `class`



### `enableVersioning`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEncryption`

- **Type:** `bool`

- **Default Value:** `false`



### `enableSchemaValidation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAutoSave`

- **Type:** `bool`

- **Default Value:** `true`



### `format`

- **Type:** `string`

- **Default Value:** `"json"`



### `desc`

- **Type:** `const ConfigDesc&`

- **Default Value:** `{})`



### `path`

- **Type:** `string&`

- **Default Value:** `"")`



### `description`

- **Type:** `string&`

- **Default Value:** `"")`



### `m_Stats`

- **Type:** `return`



### `m_Stats`

- **Type:** `ConfigStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ConfigDesc`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static ConfigSystem&`

---

## Initialize



- **Parameters:** ``

- **Return:** `bool`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## CreateConfig



- **Parameters:** `const std::string& name`, `const ConfigDesc& desc = {}`

- **Return:** `ConfigHandle`

---

## DestroyConfig



- **Parameters:** `ConfigHandle handle`

- **Return:** `void`

---

## LoadConfig



- **Parameters:** `const std::string& path`

- **Return:** `bool`

---

## SaveConfig



- **Parameters:** `ConfigHandle handle`, `const std::string& path = ""`

- **Return:** `bool`

---

## SetValue



- **Parameters:** `ConfigHandle handle`, `const std::string& key`, `const Variant& value`

- **Return:** `void`

---

## RemoveValue



- **Parameters:** `ConfigHandle handle`, `const std::string& key`

- **Return:** `void`

---

## CreateSection



- **Parameters:** `ConfigHandle handle`, `const std::string& section`

- **Return:** `void`

---

## RemoveSection



- **Parameters:** `ConfigHandle handle`, `const std::string& section`

- **Return:** `void`

---

## SetSchema



- **Parameters:** `ConfigHandle handle`, `const ConfigSchema& schema`

- **Return:** `void`

---

## CreateSnapshot



- **Parameters:** `ConfigHandle handle`, `const std::string& description = ""`

- **Return:** `void`

---

## RestoreSnapshot



- **Parameters:** `ConfigHandle handle`, `uint32_t version`

- **Return:** `void`

---

## SetEncryptionKey



- **Parameters:** `ConfigHandle handle`, `const std::string& key`

- **Return:** `void`

---

## EncryptConfig



- **Parameters:** `ConfigHandle handle`

- **Return:** `void`

---

## DecryptConfig



- **Parameters:** `ConfigHandle handle`

- **Return:** `void`

---

## ValidateConfig



- **Parameters:** `ConfigHandle handle`

- **Return:** `void`

---

## OptimizeConfig



- **Parameters:** `ConfigHandle handle`

- **Return:** `void`

---

## CompactConfig



- **Parameters:** `ConfigHandle handle`

- **Return:** `void`

---

## EnableConfigTracking



- **Parameters:** `bool enable`

- **Return:** `void`

---

## DumpConfigState



- **Parameters:** `ConfigHandle handle`, `const std::string& path`

- **Return:** `void`

---

## AnalyzeConfigUsage



- **Parameters:** `ConfigHandle handle`

- **Return:** `void`

---
