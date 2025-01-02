/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Authentication and authorization for collaboration
 */

#pragma once
#include "Core/Collaboration/CollaborationTypes.h"
#include "Core/Security/SecurityTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Collaboration {

struct AuthConfig {
    bool enableTokenAuth = true;
    bool enableRoleBasedAccess = true;
    bool enableAuditLog = true;
    uint32_t tokenExpiryTime = 3600;  // seconds
    std::string authServerUrl;
};

class IAuthProvider {
public:
    virtual ~IAuthProvider() = default;

    virtual bool Initialize(const AuthConfig& config) = 0;
    virtual void Shutdown() = 0;

    // Authentication
    virtual bool Authenticate(const UserCredentials& credentials, AuthToken& token) = 0;
    virtual bool ValidateToken(const AuthToken& token) = 0;
    virtual bool RevokeToken(const AuthToken& token) = 0;
    virtual bool RefreshToken(AuthToken& token) = 0;

    // Authorization
    virtual bool CheckPermission(const AuthToken& token, const ResourceId& resource, 
                               const Permission& permission) = 0;
    virtual bool GrantPermission(const AuthToken& token, const ResourceId& resource, 
                               const Permission& permission) = 0;
    virtual bool RevokePermission(const AuthToken& token, const ResourceId& resource, 
                                const Permission& permission) = 0;

    // Role management
    virtual bool AssignRole(const UserId& userId, const std::string& role) = 0;
    virtual bool RemoveRole(const UserId& userId, const std::string& role) = 0;
    virtual std::vector<std::string> GetUserRoles(const UserId& userId) const = 0;

    // Audit
    virtual bool LogAction(const UserId& userId, const std::string& action, 
                         const ResourceId& resource) = 0;
    virtual std::vector<AuditLogEntry> GetAuditLog(const AuditLogQuery& query) const = 0;
};

} // namespace Hydragon::Collaboration 