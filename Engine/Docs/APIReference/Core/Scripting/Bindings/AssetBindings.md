# Classes
---

## AssetBindings
---




# Variables
---

### `enableAsyncImport`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVersioning`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDependencyTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMetadata`

- **Type:** `bool`

- **Default Value:** `true`



### `maxPendingImports`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxAssetRefs`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `maxDependencies`

- **Type:** `uint32_t`

- **Default Value:** `5000`



### `assetRegistryPath`

- **Type:** `string`

- **Default Value:** `"Data/AssetRegistry"`



### `config`

- **Type:** `const AssetBindingsConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const ImportParams&`

- **Default Value:** `{})`



### `params`

- **Type:** `const ExportParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AssetBindingsConfig`



### `m_Stats`

- **Type:** `AssetBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AssetBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AssetBindings&`

---

## Initialize



- **Parameters:** `const AssetBindingsConfig& config = {}`

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

## ImportAsset



- **Parameters:** `const std::string& path`, `const ImportParams& params = {}`

- **Return:** `AssetHandle`

---

## ExportAsset



- **Parameters:** `AssetHandle handle`, `const std::string& path`, `const ExportParams& params = {}`

- **Return:** `void`

---

## DeleteAsset



- **Parameters:** `AssetHandle handle`

- **Return:** `void`

---

## RenameAsset



- **Parameters:** `AssetHandle handle`, `const std::string& newName`

- **Return:** `void`

---

## CreateAssetRef



- **Parameters:** `AssetHandle handle`

- **Return:** `AssetRef`

---

## ReleaseAssetRef



- **Parameters:** `AssetRef ref`

- **Return:** `void`

---

## SetMetadata



- **Parameters:** `AssetHandle handle`, `const std::string& key`, `const Variant& value`

- **Return:** `void`

---

## RemoveMetadata



- **Parameters:** `AssetHandle handle`, `const std::string& key`

- **Return:** `void`

---

## AddDependency



- **Parameters:** `AssetHandle dependent`, `AssetHandle dependency`

- **Return:** `void`

---

## RemoveDependency



- **Parameters:** `AssetHandle dependent`, `AssetHandle dependency`

- **Return:** `void`

---

## CreateVersion



- **Parameters:** `AssetHandle handle`, `const std::string& version`

- **Return:** `void`

---

## SwitchVersion



- **Parameters:** `AssetHandle handle`, `const std::string& version`

- **Return:** `void`

---

## RegisterImporter



- **Parameters:** `const std::string& extension`, `std::unique_ptr<IAssetImporter> importer`

- **Return:** `void`

---

## RegisterExporter



- **Parameters:** `const std::string& extension`, `std::unique_ptr<IAssetExporter> exporter`

- **Return:** `void`

---

## SetImportCallback



- **Parameters:** `const ImportCallback& callback`

- **Return:** `void`

---

## SetExportCallback



- **Parameters:** `const ExportCallback& callback`

- **Return:** `void`

---

## EnableAssetTracking



- **Parameters:** `bool enable`

- **Return:** `void`

---

## DumpAssetState



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## ValidateAssetReferences



- **Parameters:** ``

- **Return:** `void`

---
