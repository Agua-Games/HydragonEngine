
## Classes

### WidgetManagerSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const WidgetManagerConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### RegisterWidget



**Parameters:** const std::string& type, std::unique_ptr<IWidget> widget

---

### UnregisterWidget



**Parameters:** const std::string& type

---

### CreateWidget



**Parameters:** const std::string& type, const WidgetDesc& desc

---

### DestroyWidget



**Parameters:** WidgetHandle handle

---

### SetWidgetProperty



**Parameters:** WidgetHandle handle, const std::string& name, 
                          const PropertyValue& value

---

### EnableDragDrop



**Parameters:** WidgetHandle handle, const DragDropConfig& config

---

### DisableDragDrop



**Parameters:** WidgetHandle handle

---
