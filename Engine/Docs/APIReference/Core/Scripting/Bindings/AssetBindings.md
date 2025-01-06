
## Classes

### AssetBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const AssetBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### ImportAsset



**Parameters:** const std::string& path, const ImportParams& params = {}

---

### ExportAsset



**Parameters:** AssetHandle handle, const std::string& path, const ExportParams& params = {}

---

### DeleteAsset



**Parameters:** AssetHandle handle

---

### RenameAsset



**Parameters:** AssetHandle handle, const std::string& newName

---

### CreateAssetRef



**Parameters:** AssetHandle handle

---

### ReleaseAssetRef



**Parameters:** AssetRef ref

---

### SetMetadata



**Parameters:** AssetHandle handle, const std::string& key, const Variant& value

---

### RemoveMetadata



**Parameters:** AssetHandle handle, const std::string& key

---

### AddDependency



**Parameters:** AssetHandle dependent, AssetHandle dependency

---

### RemoveDependency



**Parameters:** AssetHandle dependent, AssetHandle dependency

---

### CreateVersion



**Parameters:** AssetHandle handle, const std::string& version

---

### SwitchVersion



**Parameters:** AssetHandle handle, const std::string& version

---

### RegisterImporter



**Parameters:** const std::string& extension, std::unique_ptr<IAssetImporter> importer

---

### RegisterExporter



**Parameters:** const std::string& extension, std::unique_ptr<IAssetExporter> exporter

---

### SetImportCallback



**Parameters:** const ImportCallback& callback

---

### SetExportCallback



**Parameters:** const ExportCallback& callback

---

### EnableAssetTracking



**Parameters:** bool enable

---

### DumpAssetState



**Parameters:** const std::string& path

---

### ValidateAssetReferences



**Parameters:** 

---
