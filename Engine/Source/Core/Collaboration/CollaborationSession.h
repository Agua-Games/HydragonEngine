/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Collaboration session management and synchronization
 */

#pragma once
#include "Core/Collaboration/CollaborationTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

namespace Hydragon::Collaboration {

struct SessionConfig {
    bool enableAutoSync = true;
    bool enableChangeTracking = true;
    bool enableLocking = true;
    bool enableCompression = true;
    uint32_t maxChangesPerSync = 1000;
    uint32_t syncBufferSize = 1024 * 1024;  // 1MB
    float syncInterval = 0.1f;  // seconds
};

class CollaborationSession {
public:
    explicit CollaborationSession(const SessionConfig& config = {});
    ~CollaborationSession();

    // Session lifecycle
    bool Initialize(const std::string& identifier);
    void Shutdown();
    void Update(float deltaTime);

    // User management
    bool AddUser(const UserInfo& user);
    bool RemoveUser(const UserId& userId);
    bool UpdateUserStatus(const UserId& userId, bool active);
    const std::vector<UserInfo>& GetActiveUsers() const;

    // Change management
    bool SubmitChange(const Change& change);
    bool RevertChange(ChangeId changeId);
    bool AcceptChange(ChangeId changeId);
    bool RejectChange(ChangeId changeId);
    std::vector<Change> GetPendingChanges() const;

    // Resource locking
    bool AcquireLock(const UserId& userId, const ResourceId& resourceId);
    bool ReleaseLock(const UserId& userId, const ResourceId& resourceId);
    bool IsLocked(const ResourceId& resourceId) const;
    UserId GetLockOwner(const ResourceId& resourceId) const;

    // Synchronization
    bool Synchronize();
    void SetSyncInterval(float interval);
    float GetLastSyncTime() const;
    const SessionStats& GetStats() const;

    // Event handling
    void RegisterEventHandler(std::shared_ptr<ICollaborationEventHandler> handler);
    void UnregisterEventHandler(const std::string& handlerId);

private:
    // Internal helpers
    bool ValidateChange(const Change& change) const;
    bool ResolveConflicts(const std::vector<Change>& changes);
    void NotifyEventHandlers(const CollaborationEvent& event);
    bool CompressChanges(const std::vector<Change>& changes, std::vector<uint8_t>& compressed);
    bool DecompressChanges(const std::vector<uint8_t>& compressed, std::vector<Change>& changes);

    SessionConfig m_Config;
    std::string m_Identifier;
    SessionStats m_Stats;
    float m_SyncTimer;
    bool m_Initialized;

    std::mutex m_SessionMutex;
    std::vector<UserInfo> m_ActiveUsers;
    std::unordered_map<ChangeId, Change> m_PendingChanges;
    std::unordered_map<ResourceId, LockInfo> m_ResourceLocks;
    std::vector<std::shared_ptr<ICollaborationEventHandler>> m_EventHandlers;
    std::unique_ptr<IChangeManager> m_ChangeManager;
    std::unique_ptr<ISyncProvider> m_SyncProvider;
};

} // namespace Hydragon::Collaboration 