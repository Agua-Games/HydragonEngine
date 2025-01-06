
## Classes

### DebugSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const DebugConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** float deltaTime

---

### DrawLine



**Parameters:** const Vector3& start, const Vector3& end, const Color& color

---

### DrawSphere



**Parameters:** const Vector3& center, float radius, const Color& color

---

### DrawBox



**Parameters:** const Vector3& center, const Vector3& extents, const Color& color

---

### DrawText



**Parameters:** const Vector3& position, const std::string& text, const Color& color

---

### AddInspectorValue



**Parameters:** const std::string& name, const std::string& value

---

### AddConsoleCommand



**Parameters:** const std::string& name, const ConsoleCommandFunc& func

---
