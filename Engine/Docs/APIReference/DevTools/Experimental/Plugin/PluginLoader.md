# Classes
---

## PluginLoader
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



### `enableCrashProtection`

- **Type:** `bool`

- **Default Value:** `true`



### `pluginDirectory`

- **Type:** `path`

- **Default Value:** `"Plugins"`



### `maxPlugins`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `config`

- **Type:** `const PluginLoaderConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `PluginLoaderConfig`



### `m_Stats`

- **Type:** `PluginLoaderStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `PluginLoaderConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static PluginLoader&`

---

## Initialize



- **Parameters:** `const PluginLoaderConfig& config = {}`

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

- **Return:** `PluginHandle`

---

## UnloadPlugin



- **Parameters:** `PluginHandle handle`

- **Return:** `void`

---

## ReloadPlugin



- **Parameters:** `PluginHandle handle`

- **Return:** `void`

---

## EnablePlugin



- **Parameters:** `PluginHandle handle`, `bool enable`

- **Return:** `void`

---
