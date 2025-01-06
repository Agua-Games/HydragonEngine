# Classes
---

## CollaborationSystem
---




# Variables
---

### `vcs`

- **Type:** `VersionControlHandle`



### `strategy`

- **Type:** `ConflictResolutionStrategy`



### `syncPolicy`

- **Type:** `SyncPolicy`



### `enableRealTimeSync`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVersionControl`

- **Type:** `bool`

- **Default Value:** `true`



### `enableConflictResolution`

- **Type:** `bool`

- **Default Value:** `true`



### `enableChangeTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `syncInterval`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxParticipants`

- **Type:** `uint32_t`

- **Default Value:** `32`



### `serverAddress`

- **Type:** `string`

- **Default Value:** `"localhost"`



### `serverPort`

- **Type:** `uint16_t`

- **Default Value:** `8080`



### `config`

- **Type:** `const CollaborationConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `CollaborationConfig`



### `m_Stats`

- **Type:** `CollaborationStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `CollaborationContext`

- **Description:** 

- **Members:**

  - `vcs`: `VersionControlHandle` - 

  - `strategy`: `ConflictResolutionStrategy` - 

  - `syncPolicy`: `SyncPolicy` - 



### `CollaborationConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static CollaborationSystem&`

---

## Initialize



- **Parameters:** `const CollaborationConfig& config = {}`

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

## CreateSession



- **Parameters:** `const std::string& identifier`

- **Return:** `SessionHandle`

---

## JoinSession



- **Parameters:** `SessionHandle handle`, `const UserInfo& user`

- **Return:** `void`

---

## LeaveSession



- **Parameters:** `SessionHandle handle`

- **Return:** `void`

---

## CloseSession



- **Parameters:** `SessionHandle handle`

- **Return:** `void`

---

## SubmitChange



- **Parameters:** `SessionHandle handle`, `const ChangeSet& changes`

- **Return:** `void`

---

## RevertChange



- **Parameters:** `SessionHandle handle`, `const ChangeId& changeId`

- **Return:** `void`

---

## AcceptChange



- **Parameters:** `SessionHandle handle`, `const ChangeId& changeId`

- **Return:** `void`

---

## RejectChange



- **Parameters:** `SessionHandle handle`, `const ChangeId& changeId`

- **Return:** `void`

---

## Synchronize



- **Parameters:** `SessionHandle handle`

- **Return:** `void`

---

## RequestLock



- **Parameters:** `SessionHandle handle`, `const ResourceId& resourceId`

- **Return:** `void`

---

## ReleaseLock



- **Parameters:** `SessionHandle handle`, `const ResourceId& resourceId`

- **Return:** `void`

---

## RegisterEventHandler



- **Parameters:** `std::unique_ptr<ICollaborationEventHandler> handler`

- **Return:** `void`

---

## UnregisterEventHandler



- **Parameters:** `const std::string& handlerId`

- **Return:** `void`

---
