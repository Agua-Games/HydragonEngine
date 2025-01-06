
## Classes

### MemorySystem




## Functions

### get



**Parameters:** 

---

### initialize



**Parameters:** const MemoryConfig& config = {}

---

### shutdown



**Parameters:** 

---

### allocate



**Parameters:** size_t size, const AllocationInfo& info = {}

---

### deallocate



**Parameters:** void* ptr

---

### addStrategy



**Parameters:** const std::string& name, IMemoryStrategy* strategy

---

### setDefaultStrategy



**Parameters:** const std::string& name

---
