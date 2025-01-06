# Classes
---

## AssetManagerSystem
---




# Variables
---

### `enableContentBrowser`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAssetIndexing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMetadataCache`

- **Type:** `bool`

- **Default Value:** `true`



### `enableThumbnails`

- **Type:** `bool`

- **Default Value:** `true`



### `maxConcurrentImports`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `thumbnailSize`

- **Type:** `uint32_t`

- **Default Value:** `256`



### `metadataCacheSize`

- **Type:** `size_t`

- **Default Value:** `64 * 1024 * 1024`



### `assetRoot`

- **Type:** `path`

- **Default Value:** `"Content"`



### `config`

- **Type:** `const AssetManagerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AssetManagerConfig`



### `m_Stats`

- **Type:** `AssetManagerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AssetManagerConfig`

- **Description:** 

- **Members:**

  - `1024`: `64 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AssetManagerSystem&`

---

## Initialize



- **Parameters:** `const AssetManagerConfig& config = {}`

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

## CreateFolder



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## DeleteFolder



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## RenameFolder



- **Parameters:** `const std::string& oldPath`, `const std::string& newPath`

- **Return:** `void`

---

## MoveAsset



- **Parameters:** `const std::string& sourcePath`, `const std::string& targetPath`

- **Return:** `void`

---

## CopyAsset



- **Parameters:** `const std::string& sourcePath`, `const std::string& targetPath`

- **Return:** `void`

---

## DeleteAsset



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## ImportAssets



- **Parameters:** `const std::vector<std::string>& paths`

- **Return:** `void`

---

## ExportAssets



- **Parameters:** `const std::vector<std::string>& paths`, `const std::string& targetDir`

- **Return:** `void`

---

## BuildAssetIndex



- **Parameters:** ``

- **Return:** `void`

---

## SearchAssets



- **Parameters:** `const AssetSearchQuery& query`, `AssetSearchResults& results`

- **Return:** `void`

---

## RegisterAssetProcessor



- **Parameters:** `const std::string& extension`, `std::unique_ptr<IAssetProcessor> processor`

- **Return:** `void`

---
