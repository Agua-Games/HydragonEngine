
## Classes

### SceneEditorSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const SceneEditorConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateEntity



**Parameters:** const EntityDesc& desc

---

### DeleteEntity



**Parameters:** EntityHandle handle

---

### DuplicateEntity



**Parameters:** EntityHandle handle

---

### SetSelection



**Parameters:** const std::vector<EntityHandle>& entities

---

### ClearSelection



**Parameters:** 

---

### BeginTransform



**Parameters:** TransformMode mode

---

### UpdateTransform



**Parameters:** const Transform& delta

---

### EndTransform



**Parameters:** 

---

### CreatePrefab



**Parameters:** const std::string& name, EntityHandle root

---

### InstantiatePrefab



**Parameters:** const std::string& name, const Transform& transform

---
