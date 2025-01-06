# Classes
---

## ConfigBindings
---




# Variables
---

### `enableAutoSave`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVersioning`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEncryption`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSchemaValidation`

- **Type:** `bool`

- **Default Value:** `true`



### `maxConfigSize`

- **Type:** `uint32_t`

- **Default Value:** `16 * 1024 * 1024`



### `maxHistoryEntries`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `autoSaveInterval`

- **Type:** `uint32_t`

- **Default Value:** `300`



### `configRootPath`

- **Type:** `string`

- **Default Value:** `"Config"`



### `config`

- **Type:** `const ConfigBindingsConfig&`

- **Default Value:** `{})`



### `desc`

- **Type:** `const ConfigDesc&`

- **Default Value:** `{})`



### `path`

- **Type:** `string&`

- **Default Value:** `"")`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ConfigBindingsConfig`



### `m_Stats`

- **Type:** `ConfigBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ConfigBindingsConfig`

- **Description:** 

- **Members:**

  - `1024`: `16 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static ConfigBindings&`

---

## Initialize



- **Parameters:** `const ConfigBindingsConfig& config = {}`

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

## CreateConfig



- **Parameters:** `const std::string& name`, `const ConfigDesc& desc = {}`

- **Return:** `ConfigHandle`

---

## LoadConfig



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## SaveConfig



- **Parameters:** `ConfigHandle handle`, `const std::string& path = ""`

- **Return:** `void`

---

## CloseConfig



- **Parameters:** `ConfigHandle handle`

- **Return:** `void`

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



- **Parameters:** `ConfigHandle handle`, `const std::string& description`

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

## ImportJSON



- **Parameters:** `ConfigHandle handle`, `const std::string& path`

- **Return:** `void`

---

## ExportJSON



- **Parameters:** `ConfigHandle handle`, `const std::string& path`

- **Return:** `void`

---

## ImportYAML



- **Parameters:** `ConfigHandle handle`, `const std::string& path`

- **Return:** `void`

---

## ExportYAML



- **Parameters:** `ConfigHandle handle`, `const std::string& path`

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

## EnableAutoSave



- **Parameters:** `bool enable`

- **Return:** `void`

---

## SetAutoSaveInterval



- **Parameters:** `uint32_t seconds`

- **Return:** `void`

---

## ForceAutoSave



- **Parameters:** ``

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
