# Classes
---

## AssetSystem
---




# Variables
---

### `maxAssets`

- **Type:** `uint32_t`

- **Default Value:** `100000`



### `maxDependencies`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `enableAssetPacking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVersioning`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `256 * 1024 * 1024`



### `assetRootPath`

- **Type:** `string`



### `cacheRootPath`

- **Type:** `string`



### `config`

- **Type:** `const AssetConfig&`

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

### `AssetConfig`

- **Description:** 

- **Members:**

  - `1024`: `256 * 1024 *` - 

  - `assetRootPath`: `string` - 

  - `cacheRootPath`: `string` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static AssetSystem&`

---

## Initialize



- **Parameters:** const AssetConfig& config = {}

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

## ImportAsset



- **Parameters:** const AssetImportDesc& desc

- **Return:** `AssetHandle`

---

## ProcessAsset



- **Parameters:** AssetHandle handle

- **Return:** `void`

---

## ExportAsset



- **Parameters:** AssetHandle handle, const std::string& path

- **Return:** `void`

---

## AddDependency



- **Parameters:** AssetHandle dependent, AssetHandle dependency

- **Return:** `void`

---

## RemoveDependency



- **Parameters:** AssetHandle dependent, AssetHandle dependency

- **Return:** `void`

---

## SetMetadata



- **Parameters:** AssetHandle handle, const AssetMetadata& metadata

- **Return:** `void`

---
