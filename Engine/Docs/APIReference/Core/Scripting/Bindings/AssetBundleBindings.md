
## Classes

### AssetBundleBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const AssetBundleBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateBundle



**Parameters:** const std::string& name, const BundleDesc& desc = {}

---

### LoadBundle



**Parameters:** const std::string& path

---

### UnloadBundle



**Parameters:** BundleHandle handle

---

### SaveBundle



**Parameters:** BundleHandle handle, const std::string& path

---

### AddToBundle



**Parameters:** BundleHandle bundle, AssetHandle asset

---

### RemoveFromBundle



**Parameters:** BundleHandle bundle, AssetHandle asset

---

### CreatePatch



**Parameters:** BundleHandle source, BundleHandle target, const std::string& path

---

### ApplyPatch



**Parameters:** BundleHandle handle, const std::string& patchPath

---

### GenerateManifest



**Parameters:** BundleHandle handle, const std::string& path

---

### LoadManifest



**Parameters:** const std::string& path

---

### OptimizeBundle



**Parameters:** BundleHandle handle

---

### ValidateBundle



**Parameters:** BundleHandle handle

---

### RepairBundle



**Parameters:** BundleHandle handle

---

### EnableBundleTracking



**Parameters:** bool enable

---

### DumpBundleContents



**Parameters:** BundleHandle handle, const std::string& path

---

### AnalyzeBundleUsage



**Parameters:** BundleHandle handle

---
