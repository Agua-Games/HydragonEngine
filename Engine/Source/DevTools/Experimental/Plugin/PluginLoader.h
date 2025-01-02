/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Experimental plugin loading and management system
 */

#pragma once
#include "PluginTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

namespace Hydragon::DevTools::Experimental {

struct PluginLoaderConfig {
    bool enableHotReload = true;
    bool enableVersioning = true;
    bool enableSandboxing = true;
    bool enableCrashProtection = true;
    std::filesystem::path pluginDirectory = "Plugins";
    uint32_t maxPlugins = 100;
};

class PluginLoader {
public:
    static PluginLoader& Get();
    
    void Initialize(const PluginLoaderConfig& config = {});
    void Shutdown();
    
    void Update();
    
    PluginHandle LoadPlugin(const std::string& path);
    void UnloadPlugin(PluginHandle handle);
    void ReloadPlugin(PluginHandle handle);
    
    bool IsPluginLoaded(const std::string& name) const;
    PluginInfo GetPluginInfo(PluginHandle handle) const;
    
    void EnablePlugin(PluginHandle handle, bool enable);
    bool IsPluginEnabled(PluginHandle handle) const;
    
    const PluginLoaderStats& GetStats() const { return m_Stats; }

private:
    PluginLoader() = default;
    
    PluginLoaderConfig m_Config;
    PluginLoaderStats m_Stats;
    std::vector<Plugin> m_Plugins;
    std::unique_ptr<IPluginHost> m_Host;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools::Experimental 