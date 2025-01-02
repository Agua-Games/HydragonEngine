/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor UI management and layout system
 */

#pragma once
#include "UITypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct EditorUIConfig {
    bool enableDocking = true;
    bool enableTheming = true;
    bool enableCustomization = true;
    bool enableLayoutSaving = true;
    uint32_t maxWindows = 100;
    uint32_t maxLayouts = 10;
    float defaultDpiScale = 1.0f;
    std::string defaultTheme = "Dark";
};

class EditorUISystem {
public:
    static EditorUISystem& Get();
    
    void Initialize(const EditorUIConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterWindow(const std::string& name, std::unique_ptr<IEditorWindow> window);
    void UnregisterWindow(const std::string& name);
    
    void ShowWindow(const std::string& name);
    void HideWindow(const std::string& name);
    void FocusWindow(const std::string& name);
    
    void SaveLayout(const std::string& name);
    void LoadLayout(const std::string& name);
    void ResetLayout();
    
    void SetTheme(const std::string& name);
    void CustomizeTheme(const ThemeCustomization& customization);
    
    const EditorUIStats& GetStats() const { return m_Stats; }

private:
    EditorUISystem() = default;
    
    EditorUIConfig m_Config;
    EditorUIStats m_Stats;
    std::unordered_map<std::string, std::unique_ptr<IEditorWindow>> m_Windows;
    std::unique_ptr<IUIManager> m_Manager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 