/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Version control system for collaboration
 */

#pragma once
#include "Core/Collaboration/CollaborationTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Collaboration {

struct VersionInfo {
    std::string identifier;
    Timestamp timestamp;
    UserId author;
    std::string description;
    uint32_t changeCount;
    bool isSnapshot;
};

class IVersionController {
public:
    virtual ~IVersionController() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    // Version management
    virtual bool CreateVersion(const std::string& identifier, const std::string& description) = 0;
    virtual bool DeleteVersion(const std::string& identifier) = 0;
    virtual bool RestoreVersion(const std::string& identifier) = 0;
    virtual std::vector<VersionInfo> GetVersionHistory() const = 0;

    // Branch management
    virtual bool CreateBranch(const std::string& name, const std::string& fromVersion) = 0;
    virtual bool MergeBranch(const std::string& source, const std::string& target) = 0;
    virtual bool SwitchBranch(const std::string& name) = 0;

    // Snapshot management
    virtual bool CreateSnapshot(const std::string& name) = 0;
    virtual bool RestoreSnapshot(const std::string& name) = 0;
    virtual std::vector<std::string> GetSnapshots() const = 0;

    // State queries
    virtual std::string GetCurrentBranch() const = 0;
    virtual std::string GetCurrentVersion() const = 0;
    virtual bool HasUnsavedChanges() const = 0;
};

} // namespace Hydragon::Collaboration 