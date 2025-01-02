/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Plugin marketplace and update management system
 */

#pragma once
#include "PluginTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct PluginManagerConfig {
    bool enableMarketplace = true;
    bool enableDependencyResolution = true;
    bool enableSignatureVerification = true;
    bool enableChangeTracking = true;
    uint32_t updateCheckInterval = 3600;  // 1 hour
    std::string marketplaceUrl;
    std::string cacheDirectory;
};

class PluginManagerSystem {
public:
    static PluginManagerSystem& Get();
    
    void Initialize(const PluginManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void InstallPlugin(const std::string& pluginId, const std::string& version = "latest");
    void UninstallPlugin(const std::string& pluginId);
    void UpdatePlugin(const std::string& pluginId);
    
    void CheckForUpdates();
    void SyncPluginRegistry();
    
    std::vector<PluginMetadata> SearchPlugins(const std::string& query);
    std::vector<PluginMetadata> GetAvailableUpdates() const;
    
    void RegisterUpdateCallback(const UpdateCallback& callback);
    void UnregisterUpdateCallback(const std::string& callbackId);
    
    const PluginManagerStats& GetStats() const { return m_Stats; }

private:
    PluginManagerSystem() = default;
    
    PluginManagerConfig m_Config;
    PluginManagerStats m_Stats;
    std::unique_ptr<IPluginRegistry> m_Registry;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 