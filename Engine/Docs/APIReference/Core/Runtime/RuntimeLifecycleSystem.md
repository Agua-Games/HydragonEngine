
## Classes

### RuntimeLifecycleSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeLifecycleConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### RegisterLifecycleHandler



**Parameters:** const std::string& phase, const LifecycleCallback& callback

---

### UnregisterLifecycleHandler



**Parameters:** const std::string& phase

---

### RequestShutdown



**Parameters:** ShutdownReason reason = ShutdownReason::User

---

### CancelShutdown



**Parameters:** 

---

### SaveApplicationState



**Parameters:** 

---

### RestoreApplicationState



**Parameters:** 

---
