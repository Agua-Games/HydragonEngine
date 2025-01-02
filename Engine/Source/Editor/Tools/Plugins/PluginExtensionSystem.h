/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Plugin extension point and API management system
 */

#pragma once
#include "PluginTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct PluginExtensionConfig {
    bool enableVersionChecking = true;
    bool enableAPIValidation = true;
    bool enableEventRouting = true;
    bool enableMetricsCollection = true;
    uint32_t maxExtensionPoints = 1000;
    uint32_t maxExtensionsPerPoint = 100;
};

class PluginExtensionSystem {
public:
    static PluginExtensionSystem& Get();
    
    void Initialize(const PluginExtensionConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterExtensionPoint(const std::string& point, const ExtensionPointDesc& desc);
    void UnregisterExtensionPoint(const std::string& point);
    
    void AddExtension(const std::string& point, const Extension& extension);
    void RemoveExtension(const std::string& point, const std::string& extensionId);
    
    void EnableExtension(const std::string& point, const std::string& extensionId);
    void DisableExtension(const std::string& point, const std::string& extensionId);
    
    std::vector<Extension> GetExtensions(const std::string& point) const;
    bool IsExtensionEnabled(const std::string& point, 
                          const std::string& extensionId) const;
    
    const PluginExtensionStats& GetStats() const { return m_Stats; }

private:
    PluginExtensionSystem() = default;
    
    PluginExtensionConfig m_Config;
    PluginExtensionStats m_Stats;
    std::unique_ptr<IExtensionRegistry> m_Registry;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 