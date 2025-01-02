/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Plugin isolation and security system
 */

#pragma once
#include "PluginTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct PluginSandboxConfig {
    bool enableMemoryIsolation = true;
    bool enableFileSystemIsolation = true;
    bool enableNetworkIsolation = true;
    bool enableResourceLimiting = true;
    uint32_t maxConcurrentSandboxes = 10;
    size_t sandboxMemoryLimit = 128 * 1024 * 1024;  // 128MB
    std::string sandboxRoot = "Editor/Sandbox";
};

class PluginSandboxSystem {
public:
    static PluginSandboxSystem& Get();
    
    void Initialize(const PluginSandboxConfig& config = {});
    void Shutdown();
    
    void Update();
    
    SandboxHandle CreateSandbox(const std::string& pluginId);
    void DestroySandbox(SandboxHandle handle);
    
    void SetResourceLimits(SandboxHandle handle, const ResourceLimits& limits);
    void SetPermissions(SandboxHandle handle, const SandboxPermissions& permissions);
    
    void MonitorResource(SandboxHandle handle, ResourceType type);
    ResourceUsage GetResourceUsage(SandboxHandle handle) const;
    
    bool IsSandboxActive(SandboxHandle handle) const;
    const SandboxStats& GetSandboxStats(SandboxHandle handle) const;
    
    const PluginSandboxStats& GetStats() const { return m_Stats; }

private:
    PluginSandboxSystem() = default;
    
    PluginSandboxConfig m_Config;
    PluginSandboxStats m_Stats;
    std::unordered_map<SandboxHandle, Sandbox> m_Sandboxes;
    std::unique_ptr<ISandboxManager> m_Manager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 