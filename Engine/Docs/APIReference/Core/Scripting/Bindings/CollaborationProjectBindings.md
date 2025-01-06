# Classes
---

## CollaborationProjectBindings
---




# Variables
---

### `enableTeamManagement`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTaskTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTimeTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableNotifications`

- **Type:** `bool`

- **Default Value:** `true`



### `maxTeams`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `maxTasksPerTeam`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxMembersPerTeam`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `projectCachePath`

- **Type:** `string`

- **Default Value:** `"Cache/Projects"`



### `config`

- **Type:** `const CollaborationProjectBindingsConfig&`

- **Default Value:** `{})`



### `desc`

- **Type:** `const ProjectDesc&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `CollaborationProjectBindingsConfig`



### `m_Stats`

- **Type:** `CollaborationProjectBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `CollaborationProjectBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static CollaborationProjectBindings&`

---

## Initialize



- **Parameters:** `const CollaborationProjectBindingsConfig& config = {}`

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

## CreateProject



- **Parameters:** `const std::string& name`, `const ProjectDesc& desc = {}`

- **Return:** `ProjectHandle`

---

## OpenProject



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## CloseProject



- **Parameters:** `ProjectHandle handle`

- **Return:** `void`

---

## SaveProject



- **Parameters:** `ProjectHandle handle`, `const std::string& path`

- **Return:** `void`

---

## CreateTeam



- **Parameters:** `ProjectHandle project`, `const std::string& name`

- **Return:** `TeamHandle`

---

## AddTeamMember



- **Parameters:** `TeamHandle team`, `const UserInfo& member`

- **Return:** `void`

---

## RemoveTeamMember



- **Parameters:** `TeamHandle team`, `const UserInfo& member`

- **Return:** `void`

---

## CreateTask



- **Parameters:** `TeamHandle team`, `const TaskDesc& desc`

- **Return:** `TaskHandle`

---

## AssignTask



- **Parameters:** `TaskHandle task`, `const UserInfo& assignee`

- **Return:** `void`

---

## UpdateTaskStatus



- **Parameters:** `TaskHandle task`, `TaskStatus status`

- **Return:** `void`

---

## SetTaskPriority



- **Parameters:** `TaskHandle task`, `TaskPriority priority`

- **Return:** `void`

---

## StartTimeTracking



- **Parameters:** `TaskHandle task`

- **Return:** `void`

---

## StopTimeTracking



- **Parameters:** `TaskHandle task`

- **Return:** `void`

---

## AddTimeEntry



- **Parameters:** `TaskHandle task`, `const TimeEntry& entry`

- **Return:** `void`

---

## UpdateTaskProgress



- **Parameters:** `TaskHandle task`, `float progress`

- **Return:** `void`

---

## SetTaskMilestone



- **Parameters:** `TaskHandle task`, `const std::string& milestone`

- **Return:** `void`

---

## AddTaskComment



- **Parameters:** `TaskHandle task`, `const TaskComment& comment`

- **Return:** `void`

---

## ValidateProject



- **Parameters:** `ProjectHandle handle`

- **Return:** `void`

---

## OptimizeProject



- **Parameters:** `ProjectHandle handle`

- **Return:** `void`

---

## ArchiveCompletedTasks



- **Parameters:** `ProjectHandle handle`

- **Return:** `void`

---

## SendTaskNotification



- **Parameters:** `TaskHandle task`, `const NotificationDesc& desc`

- **Return:** `void`

---

## SetNotificationCallback



- **Parameters:** `const NotificationCallback& callback`

- **Return:** `void`

---

## EnableNotifications



- **Parameters:** `bool enable`

- **Return:** `void`

---

## EnableProjectTracking



- **Parameters:** `bool enable`

- **Return:** `void`

---

## DumpProjectState



- **Parameters:** `ProjectHandle handle`, `const std::string& path`

- **Return:** `void`

---

## AnalyzeProjectMetrics



- **Parameters:** `ProjectHandle handle`

- **Return:** `void`

---
