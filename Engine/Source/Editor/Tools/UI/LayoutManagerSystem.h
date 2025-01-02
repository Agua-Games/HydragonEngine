/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor layout management and persistence system
 */

#pragma once
#include "UITypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct LayoutManagerConfig {
    bool enableAutoSave = true;
    bool enableVersioning = true;
    bool enablePresets = true;
    bool enableSharing = true;
    uint32_t maxLayouts = 20;
    uint32_t autoSaveInterval = 300;  // seconds
    std::string layoutDirectory = "Editor/Layouts";
};

class LayoutManagerSystem {
public:
    static LayoutManagerSystem& Get();
    
    void Initialize(const LayoutManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void SaveLayout(const std::string& name);
    void LoadLayout(const std::string& name);
    void DeleteLayout(const std::string& name);
    
    void ExportLayout(const std::string& name, const std::string& path);
    void ImportLayout(const std::string& path);
    
    void SetDefaultLayout(const std::string& name);
    void ResetToDefault();
    
    std::vector<std::string> GetAvailableLayouts() const;
    const LayoutInfo& GetLayoutInfo(const std::string& name) const;
    
    const LayoutManagerStats& GetStats() const { return m_Stats; }

private:
    LayoutManagerSystem() = default;
    
    LayoutManagerConfig m_Config;
    LayoutManagerStats m_Stats;
    std::unordered_map<std::string, LayoutData> m_Layouts;
    std::unique_ptr<ILayoutSerializer> m_Serializer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 