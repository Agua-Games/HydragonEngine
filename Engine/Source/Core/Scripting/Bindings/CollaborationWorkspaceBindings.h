/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Collaboration workspace system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Collaboration/CollaborationWorkspaceSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct CollaborationWorkspaceBindingsConfig {
    bool enableWorkspaceSharing = true;
    bool enableSnapshotting = true;
    bool enableBranchManagement = true;
    bool enableMerging = true;
    uint32_t maxWorkspaces = 16;
    uint32_t maxSnapshots = 100;
    uint32_t maxBranches = 32;
    std::string workspaceCachePath = "Cache/Workspaces";
};

class CollaborationWorkspaceBindings {
public:
    static CollaborationWorkspaceBindings& Get();
    
    void Initialize(const CollaborationWorkspaceBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Workspace management
    WorkspaceHandle CreateWorkspace(const std::string& name, const WorkspaceDesc& desc = {});
    void OpenWorkspace(const std::string& path);
    void CloseWorkspace(WorkspaceHandle handle);
    void SaveWorkspace(WorkspaceHandle handle, const std::string& path);
    
    // Workspace sharing
    void ShareWorkspace(WorkspaceHandle handle, const std::vector<UserInfo>& users);
    void UnshareWorkspace(WorkspaceHandle handle, const UserInfo& user);
    bool IsWorkspaceShared(WorkspaceHandle handle) const;
    std::vector<UserInfo> GetWorkspaceUsers(WorkspaceHandle handle) const;
    
    // Snapshot management
    SnapshotHandle CreateSnapshot(WorkspaceHandle workspace, const std::string& name);
    void RestoreSnapshot(SnapshotHandle handle);
    void DeleteSnapshot(SnapshotHandle handle);
    std::vector<SnapshotInfo> GetWorkspaceSnapshots(WorkspaceHandle handle) const;
    
    // Branch management
    BranchHandle CreateBranch(WorkspaceHandle workspace, const std::string& name);
    void SwitchBranch(WorkspaceHandle workspace, BranchHandle branch);
    void MergeBranch(BranchHandle source, BranchHandle target);
    void DeleteBranch(BranchHandle handle);
    
    // Workspace operations
    void AddToWorkspace(WorkspaceHandle workspace, AssetHandle asset);
    void RemoveFromWorkspace(WorkspaceHandle workspace, AssetHandle asset);
    bool IsInWorkspace(WorkspaceHandle workspace, AssetHandle asset) const;
    std::vector<AssetHandle> GetWorkspaceAssets(WorkspaceHandle handle) const;
    
    // Workspace queries
    WorkspaceInfo GetWorkspaceInfo(WorkspaceHandle handle) const;
    std::vector<WorkspaceHandle> GetOpenWorkspaces() const;
    WorkspaceStats GetWorkspaceStats() const;
    
    // Workspace utilities
    void ValidateWorkspace(WorkspaceHandle handle);
    void RepairWorkspace(WorkspaceHandle handle);
    void OptimizeWorkspace(WorkspaceHandle handle);
    
    // Debug utilities
    void EnableWorkspaceTracking(bool enable);
    void DumpWorkspaceState(WorkspaceHandle handle, const std::string& path);
    void AnalyzeWorkspaceUsage(WorkspaceHandle handle);
    
    const CollaborationWorkspaceBindingsStats& GetStats() const { return m_Stats; }

private:
    CollaborationWorkspaceBindings() = default;
    
    CollaborationWorkspaceBindingsConfig m_Config;
    CollaborationWorkspaceBindingsStats m_Stats;
    std::unordered_map<WorkspaceHandle, WorkspaceInstance> m_Workspaces;
    std::unordered_map<std::string, WorkspaceHandle> m_WorkspaceMap;
    std::unordered_map<SnapshotHandle, SnapshotInstance> m_Snapshots;
    std::unordered_map<BranchHandle, BranchInstance> m_Branches;
    std::unique_ptr<Collaboration::IWorkspaceProcessor> m_Processor;
    std::unique_ptr<Collaboration::ISnapshotManager> m_SnapshotManager;
    std::unique_ptr<Collaboration::IBranchManager> m_BranchManager;
    std::unique_ptr<Collaboration::IMergeResolver> m_MergeResolver;
    std::unique_ptr<Collaboration::IWorkspaceValidator> m_Validator;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 