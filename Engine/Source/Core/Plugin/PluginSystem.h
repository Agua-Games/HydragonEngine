/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core plugin management system
 */

#pragma once
#include "PluginTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::Plugin {

class PluginSystem {
public:
    struct PluginConfig {
        bool enableHotReload = true;
        bool enableVersionCheck = true;
        bool enableDependencyCheck = true;
        bool enableSandboxing = true;
        uint32_t maxPlugins = 100;
        size_t memoryBudget = 64 * 1024 * 1024;  // 64MB
        std::string pluginDirectory;
    };

    static PluginSystem& Get();
    
    void Initialize(const PluginConfig& config = {});
    void Shutdown();
    
    void Update();
    
    PluginHandle LoadPlugin(const std::string& path);
    void UnloadPlugin(PluginHandle handle);
    
    void EnablePlugin(PluginHandle handle);
    void DisablePlugin(PluginHandle handle);
    
    bool IsPluginLoaded(const std::string& name) const;
    bool IsPluginEnabled(PluginHandle handle) const;
    
    void* GetPluginFunction(PluginHandle handle, const std::string& name);
    const PluginInfo& GetPluginInfo(PluginHandle handle) const;
    
    void RegisterInterface(const std::string& name, void* interface);
    void* GetInterface(const std::string& name) const;
    
    const PluginStats& GetStats() const { return m_Stats; }

private:
    PluginSystem() = default;
    
    PluginConfig m_Config;
    PluginStats m_Stats;
    std::vector<Plugin> m_Plugins;
    std::unique_ptr<IPluginLoader> m_Loader;
    bool m_Initialized = false;
};

} // namespace Hydragon::Plugin 