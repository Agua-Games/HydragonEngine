
## Classes

### AssetPackageBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const AssetPackageBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreatePackage



**Parameters:** const std::string& name, const PackageDesc& desc = {}

---

### OpenPackage



**Parameters:** const std::string& path

---

### ClosePackage



**Parameters:** PackageHandle handle

---

### SavePackage



**Parameters:** PackageHandle handle, const std::string& path

---

### AddAsset



**Parameters:** PackageHandle package, AssetHandle asset

---

### RemoveAsset



**Parameters:** PackageHandle package, AssetHandle asset

---

### StreamPackage



**Parameters:** const std::string& path, const StreamParams& params = {}

---

### UnstreamPackage



**Parameters:** PackageHandle handle

---

### SetCompressionLevel



**Parameters:** PackageHandle handle, CompressionLevel level

---

### CompressPackage



**Parameters:** PackageHandle handle

---

### DecompressPackage



**Parameters:** PackageHandle handle

---

### SetEncryptionKey



**Parameters:** PackageHandle handle, const std::string& key

---

### EncryptPackage



**Parameters:** PackageHandle handle

---

### DecryptPackage



**Parameters:** PackageHandle handle

---

### DefragmentPackage



**Parameters:** PackageHandle handle

---

### ValidatePackage



**Parameters:** PackageHandle handle

---

### OptimizePackage



**Parameters:** PackageHandle handle

---

### EnablePackageTracking



**Parameters:** bool enable

---

### DumpPackageContents



**Parameters:** PackageHandle handle, const std::string& path

---

### AnalyzePackageUsage



**Parameters:** PackageHandle handle

---
