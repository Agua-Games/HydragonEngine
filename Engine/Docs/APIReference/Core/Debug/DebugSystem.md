
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



**Parameters:** 

---

### DrawLine



**Parameters:** const Vector3& start, const Vector3& end, const Color& color

---

### DrawSphere



**Parameters:** const Vector3& center, float radius, const Color& color

---

### DrawBox



**Parameters:** const Vector3& min, const Vector3& max, const Color& color

---

### Log



**Parameters:** LogLevel level, const std::string& message

---

### AddWatch



**Parameters:** const std::string& name, const WatchCallback& callback

---

### BeginScope



**Parameters:** const std::string& name

---

### EndScope



**Parameters:** ProfilerScope scope

---
