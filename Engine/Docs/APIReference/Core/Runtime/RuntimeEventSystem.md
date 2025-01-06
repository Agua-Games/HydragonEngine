
## Classes

### RuntimeEventSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeEventConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### RegisterHandler



**Parameters:** const std::string& event, const EventCallback& handler, EventPriority priority = EventPriority::Normal

---

### UnregisterHandler



**Parameters:** const std::string& event, const EventCallback& handler

---

### PostEvent



**Parameters:** const std::string& event, const EventData& data = {}

---

### PostEventDeferred



**Parameters:** const std::string& event, const EventData& data = {}, uint32_t delayMs = 0

---

### FlushEvents



**Parameters:** const std::string& event = ""

---

### CancelDeferredEvents



**Parameters:** const std::string& event = ""

---
