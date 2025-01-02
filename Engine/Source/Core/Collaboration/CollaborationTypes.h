/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Type definitions for collaboration system
 */

#pragma once
#include <string>
#include <vector>
#include <chrono>

namespace Hydragon::Collaboration {

using SessionHandle = uint64_t;
using ChangeId = uint64_t;
using ResourceId = std::string;
using UserId = std::string;
using Timestamp = std::chrono::system_clock::time_point;

// User information
struct UserInfo {
    UserId id;
    std::string name;
    std::string role;
    std::string avatar;
    bool isActive = true;
};

// Change tracking
struct Change {
    ChangeId id;
    UserId author;
    Timestamp timestamp;
    std::string description;
    std::vector<uint8_t> data;
    bool isAccepted = false;
};

struct ChangeSet {
    std::vector<Change> changes;
    std::string comment;
    bool requiresReview = false;
};

// Statistics
struct CollaborationStats {
    uint32_t activeSessions = 0;
    uint32_t totalUsers = 0;
    uint32_t pendingChanges = 0;
    uint32_t conflictCount = 0;
    float averageSyncTime = 0.0f;
    uint64_t totalBytesTransferred = 0;
};

// Event handling interface
class ICollaborationEventHandler {
public:
    virtual ~ICollaborationEventHandler() = default;
    
    virtual void OnUserJoined(SessionHandle session, const UserInfo& user) = 0;
    virtual void OnUserLeft(SessionHandle session, const UserInfo& user) = 0;
    virtual void OnChangeSubmitted(SessionHandle session, const Change& change) = 0;
    virtual void OnChangeAccepted(SessionHandle session, ChangeId changeId) = 0;
    virtual void OnChangeRejected(SessionHandle session, ChangeId changeId) = 0;
    virtual void OnConflictDetected(SessionHandle session, const std::vector<Change>& conflicts) = 0;
    virtual void OnSyncComplete(SessionHandle session) = 0;
};

} // namespace Hydragon::Collaboration 