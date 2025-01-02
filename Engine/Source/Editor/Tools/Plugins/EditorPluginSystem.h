/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor plugin management and extension system
 */

#pragma once
#include "PluginTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct EditorPluginConfig {
    bool enableHotReload = true;
    bool enableVersioning = true;
    bool enableSandboxing = true;
    bool enableAutoUpdate = true;
    uint32_t maxPlugins = 100;
    uint32_t sandboxMemoryLimit = 256 * 1024 * 1024;  // 256MB
    std::string pluginDirectory = "Editor/Plugins";
};

class EditorPluginSystem {
public:
    static EditorPluginSystem& Get();
    
    void Initialize(const EditorPluginConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void LoadPlugin(const std::string& path);
    void UnloadPlugin(const std::string& pluginId);
    void ReloadPlugin(const std::string& pluginId);
    
    void EnablePlugin(const std::string& pluginId);
    void DisablePlugin(const std::string& pluginId);
    
    void RegisterExtensionPoint(const std::string& point, 
                              const ExtensionPointDesc& desc);
    void UnregisterExtensionPoint(const std::string& point);
    
    const PluginInfo& GetPluginInfo(const std::string& pluginId) const;
    std::vector<std::string> GetLoadedPlugins() const;
    
    const EditorPluginStats& GetStats() const { return m_Stats; }

private:
    EditorPluginSystem() = default;
    
    EditorPluginConfig m_Config;
    EditorPluginStats m_Stats;
    std::unordered_map<std::string, PluginInstance> m_Plugins;
    std::unique_ptr<IPluginLoader> m_Loader;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 