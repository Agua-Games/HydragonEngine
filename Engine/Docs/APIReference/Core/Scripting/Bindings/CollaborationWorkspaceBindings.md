
## Classes

### CollaborationWorkspaceBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const CollaborationWorkspaceBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateWorkspace



**Parameters:** const std::string& name, const WorkspaceDesc& desc = {}

---

### OpenWorkspace



**Parameters:** const std::string& path

---

### CloseWorkspace



**Parameters:** WorkspaceHandle handle

---

### SaveWorkspace



**Parameters:** WorkspaceHandle handle, const std::string& path

---

### ShareWorkspace



**Parameters:** WorkspaceHandle handle, const std::vector<UserInfo>& users

---

### UnshareWorkspace



**Parameters:** WorkspaceHandle handle, const UserInfo& user

---

### CreateSnapshot



**Parameters:** WorkspaceHandle workspace, const std::string& name

---

### RestoreSnapshot



**Parameters:** SnapshotHandle handle

---

### DeleteSnapshot



**Parameters:** SnapshotHandle handle

---

### CreateBranch



**Parameters:** WorkspaceHandle workspace, const std::string& name

---

### SwitchBranch



**Parameters:** WorkspaceHandle workspace, BranchHandle branch

---

### MergeBranch



**Parameters:** BranchHandle source, BranchHandle target

---

### DeleteBranch



**Parameters:** BranchHandle handle

---

### AddToWorkspace



**Parameters:** WorkspaceHandle workspace, AssetHandle asset

---

### RemoveFromWorkspace



**Parameters:** WorkspaceHandle workspace, AssetHandle asset

---

### ValidateWorkspace



**Parameters:** WorkspaceHandle handle

---

### RepairWorkspace



**Parameters:** WorkspaceHandle handle

---

### OptimizeWorkspace



**Parameters:** WorkspaceHandle handle

---

### EnableWorkspaceTracking



**Parameters:** bool enable

---

### DumpWorkspaceState



**Parameters:** WorkspaceHandle handle, const std::string& path

---

### AnalyzeWorkspaceUsage



**Parameters:** WorkspaceHandle handle

---
