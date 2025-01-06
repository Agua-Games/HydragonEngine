# Classes
---

## RuntimeDialogueSystem
---




# Variables
---

### `enableBranching`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLocalization`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVariables`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLipSync`

- **Type:** `bool`

- **Default Value:** `true`



### `maxActiveDialogues`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `maxChoiceOptions`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `defaultTextSpeed`

- **Type:** `float`

- **Default Value:** `30.0f`



### `defaultChoiceTimeout`

- **Type:** `float`

- **Default Value:** `10.0f`



### `dialogueCachePath`

- **Type:** `string`

- **Default Value:** `"Cache/Dialogue"`



### `config`

- **Type:** `const RuntimeDialogueConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const DialogueParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeDialogueConfig`



### `m_Stats`

- **Type:** `RuntimeDialogueStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeDialogueConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static RuntimeDialogueSystem&`

---

## Initialize



- **Parameters:** const RuntimeDialogueConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## StartDialogue



- **Parameters:** const std::string& dialogueId, const DialogueParams& params = {}

- **Return:** `DialogueHandle`

---

## StopDialogue



- **Parameters:** DialogueHandle handle

- **Return:** `void`

---

## PauseDialogue



- **Parameters:** DialogueHandle handle

- **Return:** `void`

---

## ResumeDialogue



- **Parameters:** DialogueHandle handle

- **Return:** `void`

---

## AdvanceDialogue



- **Parameters:** DialogueHandle handle

- **Return:** `void`

---

## SelectChoice



- **Parameters:** DialogueHandle handle, uint32_t choiceIndex

- **Return:** `void`

---

## SetVariable



- **Parameters:** const std::string& name, const DialogueVariable& value

- **Return:** `void`

---

## SetSpeaker



- **Parameters:** DialogueHandle handle, EntityHandle speaker

- **Return:** `void`

---

## SetListener



- **Parameters:** DialogueHandle handle, EntityHandle listener

- **Return:** `void`

---

## SetTextSpeed



- **Parameters:** DialogueHandle handle, float charactersPerSecond

- **Return:** `void`

---

## LoadDialogueGraph



- **Parameters:** const std::string& path

- **Return:** `void`

---

## UnloadDialogueGraph



- **Parameters:** const std::string& path

- **Return:** `void`

---

## RegisterDialogueCallback



- **Parameters:** DialogueEventType type, const DialogueCallback& callback

- **Return:** `void`

---

## UnregisterDialogueCallback



- **Parameters:** DialogueEventType type

- **Return:** `void`

---

## AddCustomCondition



- **Parameters:** const std::string& name, const DialogueCondition& condition

- **Return:** `void`

---

## RemoveCustomCondition



- **Parameters:** const std::string& name

- **Return:** `void`

---

## SaveConversationState



- **Parameters:** DialogueHandle handle, const std::string& name

- **Return:** `void`

---

## LoadConversationState



- **Parameters:** DialogueHandle handle, const std::string& name

- **Return:** `void`

---
