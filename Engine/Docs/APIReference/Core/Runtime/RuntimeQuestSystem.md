# Classes
---

## RuntimeQuestSystem
---




# Variables
---

### `enableBranching`

- **Type:** `bool`

- **Default Value:** `true`



### `enableParallelQuests`

- **Type:** `bool`

- **Default Value:** `true`



### `enableQuestChains`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTimeTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `maxActiveQuests`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `maxQuestSteps`

- **Type:** `uint32_t`

- **Default Value:** `20`



### `maxQuestConditions`

- **Type:** `uint32_t`

- **Default Value:** `10`



### `questUpdateInterval`

- **Type:** `float`

- **Default Value:** `0.5f`



### `questDatabasePath`

- **Type:** `string`

- **Default Value:** `"Config/Quests"`



### `config`

- **Type:** `const RuntimeQuestConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const QuestParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeQuestConfig`



### `m_Stats`

- **Type:** `RuntimeQuestStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeQuestConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static RuntimeQuestSystem&`

---

## Initialize



- **Parameters:** `const RuntimeQuestConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## StartQuest



- **Parameters:** `const std::string& questId`, `const QuestParams& params = {}`

- **Return:** `QuestHandle`

---

## CompleteQuest



- **Parameters:** `QuestHandle handle`

- **Return:** `void`

---

## FailQuest



- **Parameters:** `QuestHandle handle`

- **Return:** `void`

---

## AbandonQuest



- **Parameters:** `QuestHandle handle`

- **Return:** `void`

---

## UpdateObjective



- **Parameters:** `QuestHandle handle`, `const std::string& objectiveId`, `uint32_t progress`

- **Return:** `void`

---

## CompleteObjective



- **Parameters:** `QuestHandle handle`, `const std::string& objectiveId`

- **Return:** `void`

---

## FailObjective



- **Parameters:** `QuestHandle handle`, `const std::string& objectiveId`

- **Return:** `void`

---

## AddQuestCondition



- **Parameters:** `QuestHandle handle`, `const QuestCondition& condition`

- **Return:** `void`

---

## RemoveQuestCondition



- **Parameters:** `QuestHandle handle`, `const std::string& conditionId`

- **Return:** `void`

---

## RegisterQuestCallback



- **Parameters:** `QuestEventType type`, `const QuestCallback& callback`

- **Return:** `void`

---

## UnregisterQuestCallback



- **Parameters:** `QuestEventType type`

- **Return:** `void`

---

## LoadQuestDatabase



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## ReloadQuestDatabase



- **Parameters:** ``

- **Return:** `void`

---

## SaveQuestState



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## LoadQuestState



- **Parameters:** `const std::string& name`

- **Return:** `void`

---
