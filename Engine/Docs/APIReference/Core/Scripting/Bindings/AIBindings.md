
## Classes

### AIBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const AIBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateAgent



**Parameters:** const AgentDesc& desc

---

### DestroyAgent



**Parameters:** AgentHandle handle

---

### SetAgentPosition



**Parameters:** AgentHandle handle, const Vector3& position

---

### SetAgentRotation



**Parameters:** AgentHandle handle, const Quaternion& rotation

---

### CreateBehaviorTree



**Parameters:** const std::string& name, const BehaviorTreeDesc& desc

---

### DestroyBehaviorTree



**Parameters:** BehaviorTreeHandle handle

---

### AssignBehaviorTree



**Parameters:** AgentHandle agent, BehaviorTreeHandle tree

---

### SetBehaviorTreeVariable



**Parameters:** BehaviorTreeHandle handle, const std::string& name, const Variant& value

---

### RequestPath



**Parameters:** const PathRequest& request

---

### CancelPathRequest



**Parameters:** PathRequestHandle handle

---

### RegisterStimulus



**Parameters:** const Stimulus& stimulus

---

### UpdateStimulusLocation



**Parameters:** StimulusHandle handle, const Vector3& location

---

### RemoveStimulus



**Parameters:** StimulusHandle handle

---

### SetDecisionMaker



**Parameters:** AgentHandle handle, const DecisionMakerDesc& desc

---

### UpdateWorldState



**Parameters:** AgentHandle handle, const WorldState& state

---

### AddGoal



**Parameters:** AgentHandle handle, const Goal& goal

---

### RemoveGoal



**Parameters:** AgentHandle handle, GoalHandle goalHandle

---

### EnableDebugVisualization



**Parameters:** bool enable

---

### SetDebugMode



**Parameters:** AIDebugMode mode

---

### DumpAgentState



**Parameters:** AgentHandle handle, const std::string& path

---
