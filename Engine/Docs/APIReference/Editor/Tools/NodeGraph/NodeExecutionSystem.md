
## Classes

### NodeExecutionSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const NodeExecutionConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### ExecuteNode



**Parameters:** NodeHandle handle

---

### ExecuteGraph



**Parameters:** const std::vector<NodeHandle>& outputNodes

---

### StopExecution



**Parameters:** 

---

### SetNodeInput



**Parameters:** NodeHandle handle, uint32_t pin, const NodeValue& value

---

### RegisterExecutor



**Parameters:** const std::string& nodeType, 
                         std::unique_ptr<INodeExecutor> executor

---
