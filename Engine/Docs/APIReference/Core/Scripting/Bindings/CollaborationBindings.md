
## Classes

### CollaborationBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const CollaborationBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateSession



**Parameters:** const std::string& name, const SessionDesc& desc = {}

---

### JoinSession



**Parameters:** const std::string& sessionId, const JoinParams& params = {}

---

### LeaveSession



**Parameters:** SessionHandle handle

---

### CloseSession



**Parameters:** SessionHandle handle

---

### SetUserInfo



**Parameters:** const UserInfo& info

---

### UpdateUserStatus



**Parameters:** UserStatus status

---

### LockAsset



**Parameters:** AssetHandle asset

---

### UnlockAsset



**Parameters:** AssetHandle asset

---

### SubmitChange



**Parameters:** const ChangeDesc& desc

---

### RevertChange



**Parameters:** ChangeHandle handle

---

### AcceptChange



**Parameters:** ChangeHandle handle

---

### RejectChange



**Parameters:** ChangeHandle handle

---

### ResolveConflict



**Parameters:** ConflictHandle handle, const ResolutionStrategy& strategy

---

### CommitChanges



**Parameters:** const std::string& message

---

### PullChanges



**Parameters:** 

---

### PushChanges



**Parameters:** 

---

### ForceSyncNow



**Parameters:** 

---

### SetSyncInterval



**Parameters:** float seconds

---

### EnableAutoSync



**Parameters:** bool enable

---

### SendMessage



**Parameters:** const CollabMessage& message

---

### BroadcastNotification



**Parameters:** const NotificationDesc& desc

---

### SetMessageCallback



**Parameters:** const MessageCallback& callback

---

### EnableCollabLogging



**Parameters:** bool enable

---

### DumpSessionState



**Parameters:** SessionHandle handle, const std::string& path

---

### ValidateCollabState



**Parameters:** 

---
