# Classes
---

## AssetType
---




# Variables
---

### `AssetHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `AssetVersion`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `sourcePath`

- **Type:** `string`



### `type`

- **Type:** `AssetType`



### `targetPath`

- **Type:** `string`



### `generateMipmaps`

- **Type:** `bool`

- **Default Value:** `true`



### `compressTextures`

- **Type:** `bool`

- **Default Value:** `true`



### `optimizeMeshes`

- **Type:** `bool`

- **Default Value:** `true`



### `type`

- **Type:** `AssetType`



### `version`

- **Type:** `AssetVersion`



### `guid`

- **Type:** `string`



### `timestamp`

- **Type:** `time_point`



### `totalAssets`

- **Type:** `uint32_t`



### `processedAssets`

- **Type:** `uint32_t`



### `failedImports`

- **Type:** `uint32_t`



### `cachedAssets`

- **Type:** `uint32_t`



### `averageProcessTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`



### `cacheSize`

- **Type:** `size_t`




# Structs
---

### `AssetImportDesc`

- **Description:** 

- **Members:**

  - `sourcePath`: `string` - 

  - `type`: `AssetType` - 

  - `targetPath`: `string` - 



### `AssetMetadata`

- **Description:** 

- **Members:**

  - `type`: `AssetType` - 

  - `version`: `AssetVersion` - 

  - `guid`: `string` - 

  - `timestamp`: `time_point` - 



### `AssetStats`

- **Description:** 

- **Members:**

  - `totalAssets`: `uint32_t` - 

  - `processedAssets`: `uint32_t` - 

  - `failedImports`: `uint32_t` - 

  - `cachedAssets`: `uint32_t` - 

  - `averageProcessTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 

  - `cacheSize`: `size_t` - 


