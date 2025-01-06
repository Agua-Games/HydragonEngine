
## Classes

### InspectorSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const InspectorConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### AddWatch



**Parameters:** const std::string& name, const WatchCallback& callback

---

### RemoveWatch



**Parameters:** const std::string& name

---

### AddBreakpoint



**Parameters:** const std::string& location

---

### RemoveBreakpoint



**Parameters:** const std::string& location

---

### InspectObject



**Parameters:** const void* object, const TypeInfo& typeInfo

---

### ModifyVariable



**Parameters:** const std::string& name, const Variable& value

---

### CaptureCallstack



**Parameters:** 

---

### CaptureMemorySnapshot



**Parameters:** 

---
