/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor theme and style management system
 */

#pragma once
#include "UITypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct ThemeManagerConfig {
    bool enableCustomization = true;
    bool enablePresets = true;
    bool enableAnimations = true;
    bool enableColorSchemes = true;
    uint32_t maxThemes = 20;
    uint32_t maxColorSchemes = 10;
    std::string themeDirectory = "Editor/Themes";
};

class ThemeManagerSystem {
public:
    static ThemeManagerSystem& Get();
    
    void Initialize(const ThemeManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void LoadTheme(const std::string& name);
    void SaveTheme(const std::string& name);
    void DeleteTheme(const std::string& name);
    
    void SetColorScheme(const std::string& name);
    void CustomizeColors(const ColorCustomization& customization);
    
    void SetStyle(const std::string& element, const StyleDesc& style);
    void ResetStyle(const std::string& element);
    
    std::vector<std::string> GetAvailableThemes() const;
    const ThemeInfo& GetThemeInfo(const std::string& name) const;
    
    const ThemeManagerStats& GetStats() const { return m_Stats; }

private:
    ThemeManagerSystem() = default;
    
    ThemeManagerConfig m_Config;
    ThemeManagerStats m_Stats;
    std::unordered_map<std::string, ThemeData> m_Themes;
    std::unique_ptr<IThemeRenderer> m_Renderer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 