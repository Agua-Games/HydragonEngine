
# Variables
---

### `PluginHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `PluginVersion`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `major`

- **Type:** `uint16_t`



### `minor`

- **Type:** `uint16_t`



### `patch`

- **Type:** `uint16_t`



### `minor`

- **Type:** `major &&`

- **Default Value:** `= other.minor && patch >= other.patch)`



### `name`

- **Type:** `string`



### `description`

- **Type:** `string`



### `author`

- **Type:** `string`



### `version`

- **Type:** `Version`



### `engineVersion`

- **Type:** `Version`



### `sandboxed`

- **Type:** `bool`



### `loadedPlugins`

- **Type:** `uint32_t`



### `enabledPlugins`

- **Type:** `uint32_t`



### `failedLoads`

- **Type:** `uint32_t`



### `hotReloads`

- **Type:** `uint32_t`



### `totalMemoryUsed`

- **Type:** `size_t`



### `averageLoadTimeMs`

- **Type:** `float`



### `InitPluginFunc`

- **Type:** `using`

- **Default Value:** `bool (*)(const PluginInfo*)`



### `ShutdownPluginFunc`

- **Type:** `using`

- **Default Value:** `void (*)()`



### `UpdatePluginFunc`

- **Type:** `using`

- **Default Value:** `void (*)(float deltaTime)`




# Structs
---

### `Version`

- **Description:** 

- **Members:**

  - `major`: `uint16_t` - 

  - `minor`: `uint16_t` - 

  - `patch`: `uint16_t` - 



### `PluginInfo`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `description`: `string` - 

  - `author`: `string` - 

  - `version`: `Version` - 

  - `engineVersion`: `Version` - 

  - `sandboxed`: `bool` - 



### `PluginStats`

- **Description:** 

- **Members:**

  - `loadedPlugins`: `uint32_t` - 

  - `enabledPlugins`: `uint32_t` - 

  - `failedLoads`: `uint32_t` - 

  - `hotReloads`: `uint32_t` - 

  - `totalMemoryUsed`: `size_t` - 

  - `averageLoadTimeMs`: `float` - 


