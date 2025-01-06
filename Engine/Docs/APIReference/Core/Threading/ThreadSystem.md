
## Classes

### ThreadSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const ThreadConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### CreateThread



**Parameters:** const ThreadDesc& desc

---

### DestroyThread



**Parameters:** ThreadHandle handle

---

### CreateFiber



**Parameters:** const FiberDesc& desc

---

### DestroyFiber



**Parameters:** FiberHandle handle

---

### SetThreadAffinity



**Parameters:** ThreadHandle thread, uint32_t coreMask

---

### SetThreadPriority



**Parameters:** ThreadHandle thread, ThreadPriority priority

---

### SwitchToFiber



**Parameters:** FiberHandle fiber

---

### YieldFiber



**Parameters:** 

---
