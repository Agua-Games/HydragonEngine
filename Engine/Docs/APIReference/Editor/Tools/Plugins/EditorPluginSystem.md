# Classes
---

## EditorPluginSystem
---




# Variables
---

### `enableHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVersioning`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSandboxing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAutoUpdate`

- **Type:** `bool`

- **Default Value:** `true`



### `maxPlugins`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `sandboxMemoryLimit`

- **Type:** `uint32_t`

- **Default Value:** `256 * 1024 * 1024`



### `pluginDirectory`

- **Type:** `string`

- **Default Value:** `"Editor/Plugins"`



### `config`

- **Type:** `const EditorPluginConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `EditorPluginConfig`



### `m_Stats`

- **Type:** `EditorPluginStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `EditorPluginConfig`

- **Description:** 

- **Members:**

  - `1024`: `256 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static EditorPluginSystem&`

---

## Initialize



- **Parameters:** `const EditorPluginConfig& config = {}`

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

## LoadPlugin



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## UnloadPlugin



- **Parameters:** `const std::string& pluginId`

- **Return:** `void`

---

## ReloadPlugin



- **Parameters:** `const std::string& pluginId`

- **Return:** `void`

---

## EnablePlugin



- **Parameters:** `const std::string& pluginId`

- **Return:** `void`

---

## DisablePlugin



- **Parameters:** `const std::string& pluginId`

- **Return:** `void`

---

## RegisterExtensionPoint



- **Parameters:** `const std::string& point`, `const ExtensionPointDesc& desc`

- **Return:** `void`

---

## UnregisterExtensionPoint



- **Parameters:** `const std::string& point`

- **Return:** `void`

---
