
## Classes

### ResourceBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const ResourceBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### LoadResource



**Parameters:** const std::string& path, const LoadParams& params = {}

---

### UnloadResource



**Parameters:** ResourceHandle handle

---

### PreloadResource



**Parameters:** const std::string& path

---

### SetResourcePriority



**Parameters:** ResourceHandle handle, ResourcePriority priority

---

### ReloadResource



**Parameters:** ResourceHandle handle

---

### CreateResourcePackage



**Parameters:** const std::string& name, const std::vector<std::string>& resources

---

### LoadResourcePackage



**Parameters:** const std::string& name

---

### UnloadResourcePackage



**Parameters:** const std::string& name

---

### EnableResourceCaching



**Parameters:** bool enable

---

### ClearResourceCache



**Parameters:** 

---

### SetCacheLimit



**Parameters:** size_t sizeInBytes

---

### RegisterLoadCallback



**Parameters:** const std::string& path, const LoadCallback& callback

---

### UnregisterLoadCallback



**Parameters:** const std::string& path

---

### EnableHotReload



**Parameters:** const std::string& path, bool enable

---

### DumpResourceState



**Parameters:** const std::string& path

---
