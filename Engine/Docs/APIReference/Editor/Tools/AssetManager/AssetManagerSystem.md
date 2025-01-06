
## Classes

### AssetManagerSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const AssetManagerConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateFolder



**Parameters:** const std::string& path

---

### DeleteFolder



**Parameters:** const std::string& path

---

### RenameFolder



**Parameters:** const std::string& oldPath, const std::string& newPath

---

### MoveAsset



**Parameters:** const std::string& sourcePath, const std::string& targetPath

---

### CopyAsset



**Parameters:** const std::string& sourcePath, const std::string& targetPath

---

### DeleteAsset



**Parameters:** const std::string& path

---

### ImportAssets



**Parameters:** const std::vector<std::string>& paths

---

### ExportAssets



**Parameters:** const std::vector<std::string>& paths, const std::string& targetDir

---

### BuildAssetIndex



**Parameters:** 

---

### SearchAssets



**Parameters:** const AssetSearchQuery& query, AssetSearchResults& results

---

### RegisterAssetProcessor



**Parameters:** const std::string& extension, 
                              std::unique_ptr<IAssetProcessor> processor

---
