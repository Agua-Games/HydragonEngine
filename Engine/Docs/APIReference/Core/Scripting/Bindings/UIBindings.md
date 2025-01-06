
## Classes

### UIBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const UIBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateWidget



**Parameters:** const std::string& type, const WidgetDesc& desc

---

### DestroyWidget



**Parameters:** WidgetHandle handle

---

### SetParent



**Parameters:** WidgetHandle child, WidgetHandle parent

---

### SetPosition



**Parameters:** WidgetHandle handle, const Vector2& position

---

### SetSize



**Parameters:** WidgetHandle handle, const Vector2& size

---

### SetScale



**Parameters:** WidgetHandle handle, const Vector2& scale

---

### SetRotation



**Parameters:** WidgetHandle handle, float rotation

---

### SetVisible



**Parameters:** WidgetHandle handle, bool visible

---

### SetOpacity



**Parameters:** WidgetHandle handle, float opacity

---

### SetStyle



**Parameters:** WidgetHandle handle, const std::string& style

---

### SetStyleSheet



**Parameters:** const std::string& path

---

### RegisterEventHandler



**Parameters:** WidgetHandle handle, const std::string& event, const EventHandler& handler

---

### UnregisterEventHandler



**Parameters:** WidgetHandle handle, const std::string& event

---

### TriggerEvent



**Parameters:** WidgetHandle handle, const UIEvent& event

---

### CreateAnimation



**Parameters:** const AnimationDesc& desc

---

### DestroyAnimation



**Parameters:** AnimationHandle handle

---

### PlayAnimation



**Parameters:** WidgetHandle widget, AnimationHandle animation

---

### StopAnimation



**Parameters:** WidgetHandle widget, AnimationHandle animation

---

### InvalidateLayout



**Parameters:** WidgetHandle handle

---

### UpdateLayout



**Parameters:** 

---

### ClearLayoutCache



**Parameters:** 

---
