
## Classes

### SceneDescriptionSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const DescriptionConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### CreateNode



**Parameters:** const NodeDesc& desc

---

### DestroyNode



**Parameters:** NodeHandle handle

---

### Connect



**Parameters:** NodeHandle from, NodeHandle to, const ConnectionDesc& desc

---

### Disconnect



**Parameters:** NodeHandle from, NodeHandle to

---

### SetParameter



**Parameters:** NodeHandle node, const char* name, const Parameter& value

---

### Serialize



**Parameters:** const std::string& path

---

### Deserialize



**Parameters:** const std::string& path

---

### Validate



**Parameters:** 

---

### Optimize



**Parameters:** 

---
