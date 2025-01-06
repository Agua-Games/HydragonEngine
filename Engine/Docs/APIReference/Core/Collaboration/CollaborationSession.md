# Classes
---

## CollaborationSession
---




# Variables
---

### `enableAutoSync`

- **Type:** `bool`

- **Default Value:** `true`



### `enableChangeTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLocking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCompression`

- **Type:** `bool`

- **Default Value:** `true`



### `maxChangesPerSync`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `syncBufferSize`

- **Type:** `uint32_t`

- **Default Value:** `1024 * 1024`



### `syncInterval`

- **Type:** `float`

- **Default Value:** `0.1f`



### `config`

- **Type:** `const SessionConfig&`

- **Default Value:** `{})`



### `m_Config`

- **Type:** `SessionConfig`



### `m_Identifier`

- **Type:** `string`



### `m_Stats`

- **Type:** `SessionStats`



### `m_SyncTimer`

- **Type:** `float`



### `m_Initialized`

- **Type:** `bool`



### `m_SessionMutex`

- **Type:** `mutex`




# Structs
---

### `SessionConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 




# Functions
---

## CollaborationSession



- **Parameters:** const SessionConfig& config = {}

- **Return:** `explicit`

---

## Initialize



- **Parameters:** const std::string& identifier

- **Return:** `bool`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** float deltaTime

- **Return:** `void`

---

## AddUser



- **Parameters:** const UserInfo& user

- **Return:** `bool`

---

## RemoveUser



- **Parameters:** const UserId& userId

- **Return:** `bool`

---

## UpdateUserStatus



- **Parameters:** const UserId& userId, bool active

- **Return:** `bool`

---

## SubmitChange



- **Parameters:** const Change& change

- **Return:** `bool`

---

## RevertChange



- **Parameters:** ChangeId changeId

- **Return:** `bool`

---

## AcceptChange



- **Parameters:** ChangeId changeId

- **Return:** `bool`

---

## RejectChange



- **Parameters:** ChangeId changeId

- **Return:** `bool`

---

## AcquireLock



- **Parameters:** const UserId& userId, const ResourceId& resourceId

- **Return:** `bool`

---

## ReleaseLock



- **Parameters:** const UserId& userId, const ResourceId& resourceId

- **Return:** `bool`

---

## Synchronize



- **Parameters:** 

- **Return:** `bool`

---

## SetSyncInterval



- **Parameters:** float interval

- **Return:** `void`

---

## RegisterEventHandler



- **Parameters:** std::shared_ptr<ICollaborationEventHandler> handler

- **Return:** `void`

---

## UnregisterEventHandler



- **Parameters:** const std::string& handlerId

- **Return:** `void`

---

## ResolveConflicts



- **Parameters:** const std::vector<Change>& changes

- **Return:** `bool`

---

## NotifyEventHandlers



- **Parameters:** const CollaborationEvent& event

- **Return:** `void`

---

## CompressChanges



- **Parameters:** const std::vector<Change>& changes, std::vector<uint8_t>& compressed

- **Return:** `bool`

---

## DecompressChanges



- **Parameters:** const std::vector<uint8_t>& compressed, std::vector<Change>& changes

- **Return:** `bool`

---
