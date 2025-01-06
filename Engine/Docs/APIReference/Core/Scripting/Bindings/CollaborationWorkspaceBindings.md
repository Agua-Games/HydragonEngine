# Classes
---

## CollaborationWorkspaceBindings
---




# Variables
---

### `enableWorkspaceSharing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSnapshotting`

- **Type:** `bool`

- **Default Value:** `true`



### `enableBranchManagement`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMerging`

- **Type:** `bool`

- **Default Value:** `true`



### `maxWorkspaces`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `maxSnapshots`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxBranches`

- **Type:** `uint32_t`

- **Default Value:** `32`



### `workspaceCachePath`

- **Type:** `string`

- **Default Value:** `"Cache/Workspaces"`



### `config`

- **Type:** `const CollaborationWorkspaceBindingsConfig&`

- **Default Value:** `{})`



### `desc`

- **Type:** `const WorkspaceDesc&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `CollaborationWorkspaceBindingsConfig`



### `m_Stats`

- **Type:** `CollaborationWorkspaceBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `CollaborationWorkspaceBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static CollaborationWorkspaceBindings&`

---

## Initialize



- **Parameters:** `const CollaborationWorkspaceBindingsConfig& config = {}`

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

## CreateWorkspace



- **Parameters:** `const std::string& name`, `const WorkspaceDesc& desc = {}`

- **Return:** `WorkspaceHandle`

---

## OpenWorkspace



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## CloseWorkspace



- **Parameters:** `WorkspaceHandle handle`

- **Return:** `void`

---

## SaveWorkspace



- **Parameters:** `WorkspaceHandle handle`, `const std::string& path`

- **Return:** `void`

---

## ShareWorkspace



- **Parameters:** `WorkspaceHandle handle`, `const std::vector<UserInfo>& users`

- **Return:** `void`

---

## UnshareWorkspace



- **Parameters:** `WorkspaceHandle handle`, `const UserInfo& user`

- **Return:** `void`

---

## CreateSnapshot



- **Parameters:** `WorkspaceHandle workspace`, `const std::string& name`

- **Return:** `SnapshotHandle`

---

## RestoreSnapshot



- **Parameters:** `SnapshotHandle handle`

- **Return:** `void`

---

## DeleteSnapshot



- **Parameters:** `SnapshotHandle handle`

- **Return:** `void`

---

## CreateBranch



- **Parameters:** `WorkspaceHandle workspace`, `const std::string& name`

- **Return:** `BranchHandle`

---

## SwitchBranch



- **Parameters:** `WorkspaceHandle workspace`, `BranchHandle branch`

- **Return:** `void`

---

## MergeBranch



- **Parameters:** `BranchHandle source`, `BranchHandle target`

- **Return:** `void`

---

## DeleteBranch



- **Parameters:** `BranchHandle handle`

- **Return:** `void`

---

## AddToWorkspace



- **Parameters:** `WorkspaceHandle workspace`, `AssetHandle asset`

- **Return:** `void`

---

## RemoveFromWorkspace



- **Parameters:** `WorkspaceHandle workspace`, `AssetHandle asset`

- **Return:** `void`

---

## ValidateWorkspace



- **Parameters:** `WorkspaceHandle handle`

- **Return:** `void`

---

## RepairWorkspace



- **Parameters:** `WorkspaceHandle handle`

- **Return:** `void`

---

## OptimizeWorkspace



- **Parameters:** `WorkspaceHandle handle`

- **Return:** `void`

---

## EnableWorkspaceTracking



- **Parameters:** `bool enable`

- **Return:** `void`

---

## DumpWorkspaceState



- **Parameters:** `WorkspaceHandle handle`, `const std::string& path`

- **Return:** `void`

---

## AnalyzeWorkspaceUsage



- **Parameters:** `WorkspaceHandle handle`

- **Return:** `void`

---
