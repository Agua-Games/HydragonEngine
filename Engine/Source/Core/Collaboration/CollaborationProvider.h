/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Network provider interface for collaboration system
 */

#pragma once
#include "Core/Collaboration/CollaborationTypes.h"
#include "Core/Network/NetworkTypes.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::Collaboration {

class ICollaborationProvider {
public:
    virtual ~ICollaborationProvider() = default;

    virtual bool Initialize(const std::string& serverAddress, uint16_t port) = 0;
    virtual void Shutdown() = 0;
    virtual bool IsConnected() const = 0;

    // Session operations
    virtual SessionHandle CreateSession(const std::string& identifier) = 0;
    virtual bool JoinSession(SessionHandle handle, const UserInfo& user) = 0;
    virtual bool LeaveSession(SessionHandle handle) = 0;
    virtual bool CloseSession(SessionHandle handle) = 0;

    // Data synchronization
    virtual bool SendChanges(SessionHandle handle, const std::vector<Change>& changes) = 0;
    virtual bool ReceiveChanges(SessionHandle handle, std::vector<Change>& changes) = 0;
    virtual bool SynchronizeSession(SessionHandle handle) = 0;

    // Resource management
    virtual bool LockResource(SessionHandle handle, const ResourceId& resourceId) = 0;
    virtual bool UnlockResource(SessionHandle handle, const ResourceId& resourceId) = 0;

    // Network statistics
    virtual NetworkStats GetNetworkStats() const = 0;
};

} // namespace Hydragon::Collaboration 