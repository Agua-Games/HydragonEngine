/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Real-time collaboration system for multi-user editing
 */

#pragma once
#include "Core/Collaboration/CollaborationTypes.h"
#include "Core/Collaboration/CollaborationSession.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Collaboration {

struct CollaborationContext {
    VersionControlHandle vcs;
    ConflictResolutionStrategy strategy;
    SyncPolicy syncPolicy;
};

struct CollaborationConfig {
    bool enableRealTimeSync = true;
    bool enableVersionControl = true;
    bool enableConflictResolution = true;
    bool enableChangeTracking = true;
    uint32_t syncInterval = 100;  // milliseconds
    uint32_t maxParticipants = 32;
    std::string serverAddress = "localhost";
    uint16_t serverPort = 8080;
};

class CollaborationSystem {
public:
    static CollaborationSystem& Get();
    
    void Initialize(const CollaborationConfig& config = {});
    void Shutdown();
    void Update();

    // Session management
    SessionHandle CreateSession(const std::string& identifier);
    void JoinSession(SessionHandle handle, const UserInfo& user);
    void LeaveSession(SessionHandle handle);
    void CloseSession(SessionHandle handle);

    // Change management
    void SubmitChange(SessionHandle handle, const ChangeSet& changes);
    void RevertChange(SessionHandle handle, const ChangeId& changeId);
    void AcceptChange(SessionHandle handle, const ChangeId& changeId);
    void RejectChange(SessionHandle handle, const ChangeId& changeId);

    // Synchronization
    void Synchronize(SessionHandle handle);
    void RequestLock(SessionHandle handle, const ResourceId& resourceId);
    void ReleaseLock(SessionHandle handle, const ResourceId& resourceId);

    // Event handling
    void RegisterEventHandler(std::unique_ptr<ICollaborationEventHandler> handler);
    void UnregisterEventHandler(const std::string& handlerId);

    const CollaborationStats& GetStats() const { return m_Stats; }

private:
    CollaborationSystem() = default;
    
    CollaborationConfig m_Config;
    CollaborationStats m_Stats;
    std::unordered_map<SessionHandle, std::unique_ptr<CollaborationSession>> m_Sessions;
    std::vector<std::unique_ptr<ICollaborationEventHandler>> m_EventHandlers;
    std::unique_ptr<ICollaborationProvider> m_Provider;
    bool m_Initialized = false;
};

} // namespace Hydragon::Collaboration 