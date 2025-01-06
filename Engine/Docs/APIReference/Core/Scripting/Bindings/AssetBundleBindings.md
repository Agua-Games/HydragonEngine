# Classes
---

## AssetBundleBindings
---




# Variables
---

### `enableDeltaBundles`

- **Type:** `bool`

- **Default Value:** `true`



### `enableBundlePatching`

- **Type:** `bool`

- **Default Value:** `true`



### `enableBundleValidation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableManifestCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `maxConcurrentBundles`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `maxPatchSize`

- **Type:** `uint32_t`

- **Default Value:** `256 * 1024 * 1024`



### `manifestCacheSize`

- **Type:** `uint32_t`

- **Default Value:** `16 * 1024 * 1024`



### `bundleCachePath`

- **Type:** `string`

- **Default Value:** `"Cache/Bundles"`



### `config`

- **Type:** `const AssetBundleBindingsConfig&`

- **Default Value:** `{})`



### `desc`

- **Type:** `const BundleDesc&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AssetBundleBindingsConfig`



### `m_Stats`

- **Type:** `AssetBundleBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AssetBundleBindingsConfig`

- **Description:** 

- **Members:**

  - `1024`: `256 * 1024 *` - 

  - `1024`: `16 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AssetBundleBindings&`

---

## Initialize



- **Parameters:** `const AssetBundleBindingsConfig& config = {}`

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

## CreateBundle



- **Parameters:** `const std::string& name`, `const BundleDesc& desc = {}`

- **Return:** `BundleHandle`

---

## LoadBundle



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## UnloadBundle



- **Parameters:** `BundleHandle handle`

- **Return:** `void`

---

## SaveBundle



- **Parameters:** `BundleHandle handle`, `const std::string& path`

- **Return:** `void`

---

## AddToBundle



- **Parameters:** `BundleHandle bundle`, `AssetHandle asset`

- **Return:** `void`

---

## RemoveFromBundle



- **Parameters:** `BundleHandle bundle`, `AssetHandle asset`

- **Return:** `void`

---

## CreatePatch



- **Parameters:** `BundleHandle source`, `BundleHandle target`, `const std::string& path`

- **Return:** `void`

---

## ApplyPatch



- **Parameters:** `BundleHandle handle`, `const std::string& patchPath`

- **Return:** `void`

---

## GenerateManifest



- **Parameters:** `BundleHandle handle`, `const std::string& path`

- **Return:** `void`

---

## LoadManifest



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## OptimizeBundle



- **Parameters:** `BundleHandle handle`

- **Return:** `void`

---

## ValidateBundle



- **Parameters:** `BundleHandle handle`

- **Return:** `void`

---

## RepairBundle



- **Parameters:** `BundleHandle handle`

- **Return:** `void`

---

## EnableBundleTracking



- **Parameters:** `bool enable`

- **Return:** `void`

---

## DumpBundleContents



- **Parameters:** `BundleHandle handle`, `const std::string& path`

- **Return:** `void`

---

## AnalyzeBundleUsage



- **Parameters:** `BundleHandle handle`

- **Return:** `void`

---
