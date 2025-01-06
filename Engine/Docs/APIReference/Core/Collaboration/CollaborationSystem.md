
## Classes

### CollaborationSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const CollaborationConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateSession



**Parameters:** const std::string& identifier

---

### JoinSession



**Parameters:** SessionHandle handle, const UserInfo& user

---

### LeaveSession



**Parameters:** SessionHandle handle

---

### CloseSession



**Parameters:** SessionHandle handle

---

### SubmitChange



**Parameters:** SessionHandle handle, const ChangeSet& changes

---

### RevertChange



**Parameters:** SessionHandle handle, const ChangeId& changeId

---

### AcceptChange



**Parameters:** SessionHandle handle, const ChangeId& changeId

---

### RejectChange



**Parameters:** SessionHandle handle, const ChangeId& changeId

---

### Synchronize



**Parameters:** SessionHandle handle

---

### RequestLock



**Parameters:** SessionHandle handle, const ResourceId& resourceId

---

### ReleaseLock



**Parameters:** SessionHandle handle, const ResourceId& resourceId

---

### RegisterEventHandler



**Parameters:** std::unique_ptr<ICollaborationEventHandler> handler

---

### UnregisterEventHandler



**Parameters:** const std::string& handlerId

---
