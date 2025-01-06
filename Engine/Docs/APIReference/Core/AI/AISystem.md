
## Classes

### AISystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const AIConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** float deltaTime

---

### CreateAgent



**Parameters:** const AgentDesc& desc

---

### DestroyAgent



**Parameters:** AgentHandle handle

---

### SetBehavior



**Parameters:** AgentHandle agent, const std::string& behaviorPath

---

### SetTarget



**Parameters:** AgentHandle agent, const Vector3& position

---

### RequestPath



**Parameters:** AgentHandle agent, const Vector3& destination

---

### UpdatePerception



**Parameters:** AgentHandle agent, const PerceptionEvent& event

---
