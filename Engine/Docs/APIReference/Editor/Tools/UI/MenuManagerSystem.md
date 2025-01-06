
## Classes

### MenuManagerSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const MenuManagerConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateMenu



**Parameters:** const std::string& path, const MenuDesc& desc

---

### DestroyMenu



**Parameters:** MenuHandle handle

---

### AddMenuItem



**Parameters:** MenuHandle menu, const MenuItemDesc& desc

---

### RemoveMenuItem



**Parameters:** MenuHandle menu, const std::string& id

---

### RegisterMenuAction



**Parameters:** const std::string& id, const MenuActionCallback& callback

---

### UnregisterMenuAction



**Parameters:** const std::string& id

---

### ShowContextMenu



**Parameters:** const std::string& type, const Vector2& position

---

### HideContextMenu



**Parameters:** 

---
