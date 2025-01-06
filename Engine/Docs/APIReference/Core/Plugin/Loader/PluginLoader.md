# Classes
---

## IPluginLoader
---




# Variables
---

### `validateSignature`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCache`

- **Type:** `bool`

- **Default Value:** `true`



### `resolveSymbols`

- **Type:** `bool`

- **Default Value:** `true`



### `searchPath`

- **Type:** `string`



### `config`

- **Type:** `const LoaderConfig&`

- **Default Value:** `{})`



### `m_Config`

- **Type:** `LoaderConfig`



### `enableSandbox`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebug`

- **Type:** `bool`

- **Default Value:** `true`



### `maxStackSize`

- **Type:** `uint32_t`

- **Default Value:** `1024 * 1024`



### `scriptEngine`

- **Type:** `string`



### `config`

- **Type:** `const ScriptConfig&`

- **Default Value:** `{})`



### `m_Config`

- **Type:** `ScriptConfig`




# Structs
---

### `LoaderConfig`

- **Description:** 

- **Members:**

  - `searchPath`: `string` - 



### `ScriptConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 

  - `scriptEngine`: `string` - 




# Functions
---

## DynamicPluginLoader



- **Parameters:** const LoaderConfig& config = {}

- **Return:** `explicit`

---

## ScriptPluginLoader



- **Parameters:** const ScriptConfig& config = {}

- **Return:** `explicit`

---
