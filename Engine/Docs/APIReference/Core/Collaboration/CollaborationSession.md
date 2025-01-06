
## Classes

### CollaborationSession




## Functions

### CollaborationSession



**Parameters:** const SessionConfig& config = {}

---

### Initialize



**Parameters:** const std::string& identifier

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** float deltaTime

---

### AddUser



**Parameters:** const UserInfo& user

---

### RemoveUser



**Parameters:** const UserId& userId

---

### UpdateUserStatus



**Parameters:** const UserId& userId, bool active

---

### SubmitChange



**Parameters:** const Change& change

---

### RevertChange



**Parameters:** ChangeId changeId

---

### AcceptChange



**Parameters:** ChangeId changeId

---

### RejectChange



**Parameters:** ChangeId changeId

---

### AcquireLock



**Parameters:** const UserId& userId, const ResourceId& resourceId

---

### ReleaseLock



**Parameters:** const UserId& userId, const ResourceId& resourceId

---

### Synchronize



**Parameters:** 

---

### SetSyncInterval



**Parameters:** float interval

---

### RegisterEventHandler



**Parameters:** std::shared_ptr<ICollaborationEventHandler> handler

---

### UnregisterEventHandler



**Parameters:** const std::string& handlerId

---

### ResolveConflicts



**Parameters:** const std::vector<Change>& changes

---

### NotifyEventHandlers



**Parameters:** const CollaborationEvent& event

---

### CompressChanges



**Parameters:** const std::vector<Change>& changes, std::vector<uint8_t>& compressed

---

### DecompressChanges



**Parameters:** const std::vector<uint8_t>& compressed, std::vector<Change>& changes

---
