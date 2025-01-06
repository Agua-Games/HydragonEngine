
## Classes

### RuntimeDialogueSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeDialogueConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### StartDialogue



**Parameters:** const std::string& dialogueId, const DialogueParams& params = {}

---

### StopDialogue



**Parameters:** DialogueHandle handle

---

### PauseDialogue



**Parameters:** DialogueHandle handle

---

### ResumeDialogue



**Parameters:** DialogueHandle handle

---

### AdvanceDialogue



**Parameters:** DialogueHandle handle

---

### SelectChoice



**Parameters:** DialogueHandle handle, uint32_t choiceIndex

---

### SetVariable



**Parameters:** const std::string& name, const DialogueVariable& value

---

### SetSpeaker



**Parameters:** DialogueHandle handle, EntityHandle speaker

---

### SetListener



**Parameters:** DialogueHandle handle, EntityHandle listener

---

### SetTextSpeed



**Parameters:** DialogueHandle handle, float charactersPerSecond

---

### LoadDialogueGraph



**Parameters:** const std::string& path

---

### UnloadDialogueGraph



**Parameters:** const std::string& path

---

### RegisterDialogueCallback



**Parameters:** DialogueEventType type, const DialogueCallback& callback

---

### UnregisterDialogueCallback



**Parameters:** DialogueEventType type

---

### AddCustomCondition



**Parameters:** const std::string& name, const DialogueCondition& condition

---

### RemoveCustomCondition



**Parameters:** const std::string& name

---

### SaveConversationState



**Parameters:** DialogueHandle handle, const std::string& name

---

### LoadConversationState



**Parameters:** DialogueHandle handle, const std::string& name

---
