/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Collaboration project system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Collaboration/CollaborationProjectSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct CollaborationProjectBindingsConfig {
    bool enableTeamManagement = true;
    bool enableTaskTracking = true;
    bool enableTimeTracking = true;
    bool enableNotifications = true;
    uint32_t maxTeams = 16;
    uint32_t maxTasksPerTeam = 1000;
    uint32_t maxMembersPerTeam = 50;
    std::string projectCachePath = "Cache/Projects";
};

class CollaborationProjectBindings {
public:
    static CollaborationProjectBindings& Get();
    
    void Initialize(const CollaborationProjectBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Project management
    ProjectHandle CreateProject(const std::string& name, const ProjectDesc& desc = {});
    void OpenProject(const std::string& path);
    void CloseProject(ProjectHandle handle);
    void SaveProject(ProjectHandle handle, const std::string& path);
    
    // Team management
    TeamHandle CreateTeam(ProjectHandle project, const std::string& name);
    void AddTeamMember(TeamHandle team, const UserInfo& member);
    void RemoveTeamMember(TeamHandle team, const UserInfo& member);
    std::vector<UserInfo> GetTeamMembers(TeamHandle team) const;
    
    // Task management
    TaskHandle CreateTask(TeamHandle team, const TaskDesc& desc);
    void AssignTask(TaskHandle task, const UserInfo& assignee);
    void UpdateTaskStatus(TaskHandle task, TaskStatus status);
    void SetTaskPriority(TaskHandle task, TaskPriority priority);
    std::vector<TaskInfo> GetTeamTasks(TeamHandle team) const;
    
    // Time tracking
    void StartTimeTracking(TaskHandle task);
    void StopTimeTracking(TaskHandle task);
    void AddTimeEntry(TaskHandle task, const TimeEntry& entry);
    std::vector<TimeEntry> GetTaskTimeEntries(TaskHandle task) const;
    
    // Progress tracking
    void UpdateTaskProgress(TaskHandle task, float progress);
    void SetTaskMilestone(TaskHandle task, const std::string& milestone);
    void AddTaskComment(TaskHandle task, const TaskComment& comment);
    std::vector<TaskComment> GetTaskComments(TaskHandle task) const;
    
    // Project queries
    ProjectInfo GetProjectInfo(ProjectHandle handle) const;
    std::vector<TeamHandle> GetProjectTeams(ProjectHandle handle) const;
    ProjectStats GetProjectStats() const;
    
    // Project utilities
    void ValidateProject(ProjectHandle handle);
    void OptimizeProject(ProjectHandle handle);
    void ArchiveCompletedTasks(ProjectHandle handle);
    
    // Notification system
    void SendTaskNotification(TaskHandle task, const NotificationDesc& desc);
    void SetNotificationCallback(const NotificationCallback& callback);
    void EnableNotifications(bool enable);
    
    // Debug utilities
    void EnableProjectTracking(bool enable);
    void DumpProjectState(ProjectHandle handle, const std::string& path);
    void AnalyzeProjectMetrics(ProjectHandle handle);
    
    const CollaborationProjectBindingsStats& GetStats() const { return m_Stats; }

private:
    CollaborationProjectBindings() = default;
    
    CollaborationProjectBindingsConfig m_Config;
    CollaborationProjectBindingsStats m_Stats;
    std::unordered_map<ProjectHandle, ProjectInstance> m_Projects;
    std::unordered_map<TeamHandle, TeamInstance> m_Teams;
    std::unordered_map<TaskHandle, TaskInstance> m_Tasks;
    std::unordered_map<std::string, ProjectHandle> m_ProjectMap;
    std::unique_ptr<Collaboration::IProjectProcessor> m_Processor;
    std::unique_ptr<Collaboration::ITeamManager> m_TeamManager;
    std::unique_ptr<Collaboration::ITaskTracker> m_TaskTracker;
    std::unique_ptr<Collaboration::ITimeTracker> m_TimeTracker;
    std::unique_ptr<Collaboration::INotificationManager> m_NotificationManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 