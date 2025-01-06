
## Classes

### AssetValidationSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const AssetValidationConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### ValidateAsset



**Parameters:** const std::string& path

---

### ValidateFolder



**Parameters:** const std::string& path

---

### ValidateReferences



**Parameters:** const std::string& path

---

### AddValidationRule



**Parameters:** const std::string& assetType, 
                          const ValidationRule& rule

---

### RemoveValidationRule



**Parameters:** const std::string& assetType, 
                            const std::string& ruleName

---

### RegisterValidator



**Parameters:** const std::string& assetType, 
                         std::unique_ptr<IAssetValidator> validator

---
