# Classes
---

## AssetSystem
---




# Variables
---

### `updateStrategy`

- **Type:** `UpdateStrategy`



### `virtualMemoryBudget`

- **Type:** `size_t`

- **Default Value:** `8 * 1024 * 1024 * 1024ull`



### `maxLoadedAssets`

- **Type:** `size_t`

- **Default Value:** `10000`



### `enableAsyncLoading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `assetRootPath`

- **Type:** `string`



### `config`

- **Type:** `const AssetConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const LoadParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AssetConfig`



### `m_Stats`

- **Type:** `AssetStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AssetDependencyGraph`

- **Description:** 

- **Members:**

  - `updateStrategy`: `UpdateStrategy` - 



### `AssetConfig`

- **Description:** 

- **Members:**

  - `1024ull`: `8 * 1024 * 1024 *` - 

  - `assetRootPath`: `string` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AssetSystem&`

---

## Initialize



- **Parameters:** `const AssetConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## LoadAsset



- **Parameters:** `const std::string& path`, `const LoadParams& params = {}`

- **Return:** `AssetHandle`

---

## UnloadAsset



- **Parameters:** `AssetHandle handle`

- **Return:** `void`

---
