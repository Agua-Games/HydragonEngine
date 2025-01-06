
## Classes

### RuntimeQuestSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeQuestConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### StartQuest



**Parameters:** const std::string& questId, const QuestParams& params = {}

---

### CompleteQuest



**Parameters:** QuestHandle handle

---

### FailQuest



**Parameters:** QuestHandle handle

---

### AbandonQuest



**Parameters:** QuestHandle handle

---

### UpdateObjective



**Parameters:** QuestHandle handle, const std::string& objectiveId, uint32_t progress

---

### CompleteObjective



**Parameters:** QuestHandle handle, const std::string& objectiveId

---

### FailObjective



**Parameters:** QuestHandle handle, const std::string& objectiveId

---

### AddQuestCondition



**Parameters:** QuestHandle handle, const QuestCondition& condition

---

### RemoveQuestCondition



**Parameters:** QuestHandle handle, const std::string& conditionId

---

### RegisterQuestCallback



**Parameters:** QuestEventType type, const QuestCallback& callback

---

### UnregisterQuestCallback



**Parameters:** QuestEventType type

---

### LoadQuestDatabase



**Parameters:** const std::string& path

---

### ReloadQuestDatabase



**Parameters:** 

---

### SaveQuestState



**Parameters:** const std::string& name

---

### LoadQuestState



**Parameters:** const std::string& name

---
