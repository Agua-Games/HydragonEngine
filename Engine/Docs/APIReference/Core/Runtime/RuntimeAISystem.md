
## Classes

### RuntimeAISystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeAIConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateAgent



**Parameters:** const AIAgentDesc& desc

---

### DestroyAgent



**Parameters:** AIAgentHandle handle

---

### AssignBehavior



**Parameters:** AIAgentHandle handle, const std::string& behaviorId

---

### SetAgentTarget



**Parameters:** AIAgentHandle handle, const Vector3& position

---

### SetAgentState



**Parameters:** AIAgentHandle handle, const std::string& state

---

### RequestPath



**Parameters:** AIAgentHandle handle, const Vector3& destination

---

### CancelPathRequest



**Parameters:** AIAgentHandle handle

---

### UpdatePathfinding



**Parameters:** AIAgentHandle handle

---

### AddPerceptionStimulus



**Parameters:** const Vector3& position, const StimulusDesc& desc

---

### UpdatePerception



**Parameters:** AIAgentHandle handle, const PerceptionEvent& event

---

### SetPerceptionRange



**Parameters:** AIAgentHandle handle, float range

---

### RegisterBehaviorNode



**Parameters:** const std::string& type, std::unique_ptr<IAIBehaviorNode> node

---

### UnregisterBehaviorNode



**Parameters:** const std::string& type

---

### RegisterPerceptionSensor



**Parameters:** const std::string& type, std::unique_ptr<IPerceptionSensor> sensor

---

### UnregisterPerceptionSensor



**Parameters:** const std::string& type

---
