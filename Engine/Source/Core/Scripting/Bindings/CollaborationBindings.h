/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Collaboration system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Collaboration/CollaborationSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct CollaborationBindingsConfig {
    bool enableRealTimeSync = true;
    bool enableVersionControl = true;
    bool enableConflictResolution = true;
    bool enableChangeTracking = true;
    uint32_t maxConcurrentUsers = 32;
    uint32_t maxPendingChanges = 1000;
    uint32_t syncBufferSize = 1024 * 1024;  // 1MB
    std::string collaborationCachePath = "Cache/Collaboration";
};

class CollaborationBindings {
public:
    static CollaborationBindings& Get();
    
    void Initialize(const CollaborationBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Session management
    SessionHandle CreateSession(const std::string& name, const SessionDesc& desc = {});
    void JoinSession(const std::string& sessionId, const JoinParams& params = {});
    void LeaveSession(SessionHandle handle);
    void CloseSession(SessionHandle handle);
    
    // User management
    void SetUserInfo(const UserInfo& info);
    void UpdateUserStatus(UserStatus status);
    std::vector<UserInfo> GetSessionUsers(SessionHandle handle) const;
    
    // Asset collaboration
    void LockAsset(AssetHandle asset);
    void UnlockAsset(AssetHandle asset);
    bool IsAssetLocked(AssetHandle asset) const;
    UserInfo GetAssetLockOwner(AssetHandle asset) const;
    
    // Change management
    ChangeHandle SubmitChange(const ChangeDesc& desc);
    void RevertChange(ChangeHandle handle);
    void AcceptChange(ChangeHandle handle);
    void RejectChange(ChangeHandle handle);
    
    // Conflict resolution
    void ResolveConflict(ConflictHandle handle, const ResolutionStrategy& strategy);
    std::vector<ConflictInfo> GetPendingConflicts() const;
    
    // Version control
    void CommitChanges(const std::string& message);
    void PullChanges();
    void PushChanges();
    std::vector<CommitInfo> GetCommitHistory() const;
    
    // Sync utilities
    void ForceSyncNow();
    void SetSyncInterval(float seconds);
    void EnableAutoSync(bool enable);
    float GetLastSyncTime() const;
    
    // Communication
    void SendMessage(const CollabMessage& message);
    void BroadcastNotification(const NotificationDesc& desc);
    void SetMessageCallback(const MessageCallback& callback);
    
    // Debug utilities
    void EnableCollabLogging(bool enable);
    void DumpSessionState(SessionHandle handle, const std::string& path);
    void ValidateCollabState();
    
    const CollaborationBindingsStats& GetStats() const { return m_Stats; }

private:
    CollaborationBindings() = default;
    
    CollaborationBindingsConfig m_Config;
    CollaborationBindingsStats m_Stats;
    std::unordered_map<SessionHandle, SessionInstance> m_Sessions;
    std::unordered_map<UserHandle, UserInstance> m_Users;
    std::unordered_map<AssetHandle, LockInfo> m_AssetLocks;
    std::unordered_map<ChangeHandle, ChangeInstance> m_Changes;
    std::unique_ptr<Collaboration::ISessionManager> m_SessionManager;
    std::unique_ptr<Collaboration::ISyncManager> m_SyncManager;
    std::unique_ptr<Collaboration::IVersionController> m_VersionController;
    std::unique_ptr<Collaboration::IConflictResolver> m_ConflictResolver;
    std::unique_ptr<Collaboration::IChangeTracker> m_ChangeTracker;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 