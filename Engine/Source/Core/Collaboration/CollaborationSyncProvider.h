/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Synchronization provider for collaboration system
 */

#pragma once
#include "Core/Collaboration/CollaborationTypes.h"
#include "Core/Network/NetworkTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <queue>

namespace Hydragon::Collaboration {

struct SyncConfig {
    uint32_t maxBatchSize = 1024 * 1024;  // 1MB
    uint32_t compressionLevel = 6;
    uint32_t retryAttempts = 3;
    float retryDelay = 1.0f;  // seconds
    bool enableDeltaSync = true;
    bool enableCompression = true;
};

class ISyncProvider {
public:
    virtual ~ISyncProvider() = default;

    virtual bool Initialize(const SyncConfig& config = {}) = 0;
    virtual void Shutdown() = 0;
    virtual bool IsActive() const = 0;

    // Synchronization operations
    virtual bool PushChanges(const std::vector<Change>& changes) = 0;
    virtual bool PullChanges(std::vector<Change>& changes) = 0;
    virtual bool SynchronizeData(const ResourceId& resourceId) = 0;
    
    // State management
    virtual bool SaveState(const std::string& identifier) = 0;
    virtual bool LoadState(const std::string& identifier) = 0;
    virtual bool ResetState() = 0;

    // Diagnostics
    virtual SyncStats GetStats() const = 0;
    virtual void ClearStats() = 0;
};

} // namespace Hydragon::Collaboration 