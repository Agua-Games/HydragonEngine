
## Classes

### NodeGraphSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const NodeGraphConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateNode



**Parameters:** const NodeDesc& desc

---

### DeleteNode



**Parameters:** NodeHandle handle

---

### DuplicateNode



**Parameters:** NodeHandle handle

---

### ConnectNodes



**Parameters:** NodeHandle output, uint32_t outputPin,
                                NodeHandle input, uint32_t inputPin

---

### DisconnectNodes



**Parameters:** ConnectionHandle handle

---

### SetNodePosition



**Parameters:** NodeHandle handle, const Vector2& position

---

### SetNodeComment



**Parameters:** NodeHandle handle, const std::string& comment

---

### GroupNodes



**Parameters:** const std::vector<NodeHandle>& nodes, const std::string& name

---

### UngroupNodes



**Parameters:** GroupHandle handle

---

### RegisterNodeType



**Parameters:** const NodeTypeInfo& info

---

### UnregisterNodeType



**Parameters:** const std::string& type

---
