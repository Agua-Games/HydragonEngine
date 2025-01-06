
## Classes

### ProfilerSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const ProfilerConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### BeginFrame



**Parameters:** 

---

### EndFrame



**Parameters:** 

---

### BeginScope



**Parameters:** const char* name, ProfilerCategory category

---

### EndScope



**Parameters:** ScopeHandle handle

---

### AddMarker



**Parameters:** const char* name, ProfilerCategory category

---

### AddCounter



**Parameters:** const char* name, float value

---

### BeginGPUScope



**Parameters:** const char* name

---

### EndGPUScope



**Parameters:** 

---

### SetThreadName



**Parameters:** const std::string& name

---

### EnableCapture



**Parameters:** bool enable

---

### ExportCapture



**Parameters:** const std::string& path

---

### ClearCapture



**Parameters:** 

---
