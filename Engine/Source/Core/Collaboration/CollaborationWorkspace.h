/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Workspace management for collaboration system
 */

#pragma once
#include "Core/Collaboration/CollaborationTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Collaboration {

struct WorkspaceConfig {
    bool enableAutoSave = true;
    bool enableVersioning = true;
    bool enableBackups = true;
    uint32_t maxBackupCount = 10;
    uint32_t autoSaveInterval = 300;  // seconds
    std::string workspacePath;
};

class IWorkspaceManager {
public:
    virtual ~IWorkspaceManager() = default;

    virtual bool Initialize(const WorkspaceConfig& config) = 0;
    virtual void Shutdown() = 0;

    // Workspace operations
    virtual bool CreateWorkspace(const std::string& name, const std::string& path) = 0;
    virtual bool OpenWorkspace(const std::string& path) = 0;
    virtual bool CloseWorkspace() = 0;
    virtual bool SaveWorkspace() = 0;
    virtual bool ExportWorkspace(const std::string& path) = 0;

    // Resource management
    virtual bool AddResource(const ResourceId& id, const std::vector<uint8_t>& data) = 0;
    virtual bool RemoveResource(const ResourceId& id) = 0;
    virtual bool UpdateResource(const ResourceId& id, const std::vector<uint8_t>& data) = 0;
    virtual bool GetResource(const ResourceId& id, std::vector<uint8_t>& data) const = 0;

    // State management
    virtual bool CreateBackup(const std::string& name) = 0;
    virtual bool RestoreBackup(const std::string& name) = 0;
    virtual std::vector<std::string> GetBackups() const = 0;

    // Queries
    virtual bool IsModified() const = 0;
    virtual std::string GetCurrentWorkspace() const = 0;
    virtual WorkspaceStats GetStats() const = 0;
};

} // namespace Hydragon::Collaboration 