/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Change tracking and conflict resolution
 */

#pragma once
#include "Core/Collaboration/CollaborationTypes.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Hydragon::Collaboration {

class IChangeManager {
public:
    virtual ~IChangeManager() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    // Change tracking
    virtual bool TrackChange(const Change& change) = 0;
    virtual bool UntrackChange(ChangeId changeId) = 0;
    virtual bool HasChange(ChangeId changeId) const = 0;
    virtual std::vector<Change> GetPendingChanges() const = 0;

    // Conflict management
    virtual bool DetectConflicts(const std::vector<Change>& changes, 
                               std::vector<Change>& conflicts) = 0;
    virtual bool ResolveConflict(const Change& change, 
                               ConflictResolution resolution) = 0;
    virtual bool MergeChanges(const std::vector<Change>& changes) = 0;

    // Version control
    virtual bool CreateSnapshot(const std::string& name) = 0;
    virtual bool RestoreSnapshot(const std::string& name) = 0;
    virtual bool GetChangeHistory(std::vector<Change>& history) const = 0;
};

} // namespace Hydragon::Collaboration 