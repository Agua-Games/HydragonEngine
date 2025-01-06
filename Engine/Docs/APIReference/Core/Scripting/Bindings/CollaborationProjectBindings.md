
## Classes

### CollaborationProjectBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const CollaborationProjectBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateProject



**Parameters:** const std::string& name, const ProjectDesc& desc = {}

---

### OpenProject



**Parameters:** const std::string& path

---

### CloseProject



**Parameters:** ProjectHandle handle

---

### SaveProject



**Parameters:** ProjectHandle handle, const std::string& path

---

### CreateTeam



**Parameters:** ProjectHandle project, const std::string& name

---

### AddTeamMember



**Parameters:** TeamHandle team, const UserInfo& member

---

### RemoveTeamMember



**Parameters:** TeamHandle team, const UserInfo& member

---

### CreateTask



**Parameters:** TeamHandle team, const TaskDesc& desc

---

### AssignTask



**Parameters:** TaskHandle task, const UserInfo& assignee

---

### UpdateTaskStatus



**Parameters:** TaskHandle task, TaskStatus status

---

### SetTaskPriority



**Parameters:** TaskHandle task, TaskPriority priority

---

### StartTimeTracking



**Parameters:** TaskHandle task

---

### StopTimeTracking



**Parameters:** TaskHandle task

---

### AddTimeEntry



**Parameters:** TaskHandle task, const TimeEntry& entry

---

### UpdateTaskProgress



**Parameters:** TaskHandle task, float progress

---

### SetTaskMilestone



**Parameters:** TaskHandle task, const std::string& milestone

---

### AddTaskComment



**Parameters:** TaskHandle task, const TaskComment& comment

---

### ValidateProject



**Parameters:** ProjectHandle handle

---

### OptimizeProject



**Parameters:** ProjectHandle handle

---

### ArchiveCompletedTasks



**Parameters:** ProjectHandle handle

---

### SendTaskNotification



**Parameters:** TaskHandle task, const NotificationDesc& desc

---

### SetNotificationCallback



**Parameters:** const NotificationCallback& callback

---

### EnableNotifications



**Parameters:** bool enable

---

### EnableProjectTracking



**Parameters:** bool enable

---

### DumpProjectState



**Parameters:** ProjectHandle handle, const std::string& path

---

### AnalyzeProjectMetrics



**Parameters:** ProjectHandle handle

---
