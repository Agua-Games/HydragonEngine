/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * UI system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/UI/UISystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct UIBindingsConfig {
    bool enableAnimations = true;
    bool enableLayoutCache = true;
    bool enableEventBubbling = true;
    bool enableStyleSheets = true;
    uint32_t maxWidgets = 1000;
    uint32_t maxAnimations = 100;
    uint32_t maxEventHandlers = 500;
    std::string uiResourcePath = "Data/UI";
};

class UIBindings {
public:
    static UIBindings& Get();
    
    void Initialize(const UIBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Widget management
    WidgetHandle CreateWidget(const std::string& type, const WidgetDesc& desc);
    void DestroyWidget(WidgetHandle handle);
    void SetParent(WidgetHandle child, WidgetHandle parent);
    
    // Layout and positioning
    void SetPosition(WidgetHandle handle, const Vector2& position);
    void SetSize(WidgetHandle handle, const Vector2& size);
    void SetScale(WidgetHandle handle, const Vector2& scale);
    void SetRotation(WidgetHandle handle, float rotation);
    
    // Appearance
    void SetVisible(WidgetHandle handle, bool visible);
    void SetOpacity(WidgetHandle handle, float opacity);
    void SetStyle(WidgetHandle handle, const std::string& style);
    void SetStyleSheet(const std::string& path);
    
    // Event handling
    void RegisterEventHandler(WidgetHandle handle, const std::string& event, const EventHandler& handler);
    void UnregisterEventHandler(WidgetHandle handle, const std::string& event);
    void TriggerEvent(WidgetHandle handle, const UIEvent& event);
    
    // Animation
    AnimationHandle CreateAnimation(const AnimationDesc& desc);
    void DestroyAnimation(AnimationHandle handle);
    void PlayAnimation(WidgetHandle widget, AnimationHandle animation);
    void StopAnimation(WidgetHandle widget, AnimationHandle animation);
    
    // Widget queries
    WidgetHandle FindWidget(const std::string& name) const;
    std::vector<WidgetHandle> QueryWidgets(const std::string& selector) const;
    bool IsWidgetVisible(WidgetHandle handle) const;
    
    // Layout utilities
    void InvalidateLayout(WidgetHandle handle);
    void UpdateLayout();
    void ClearLayoutCache();
    
    const UIBindingsStats& GetStats() const { return m_Stats; }

private:
    UIBindings() = default;
    
    UIBindingsConfig m_Config;
    UIBindingsStats m_Stats;
    std::unordered_map<WidgetHandle, WidgetInstance> m_Widgets;
    std::unordered_map<std::string, WidgetHandle> m_WidgetMap;
    std::unordered_map<AnimationHandle, AnimationInstance> m_Animations;
    std::unordered_map<WidgetHandle, std::unordered_map<std::string, EventHandler>> m_EventHandlers;
    std::unique_ptr<UI::ILayoutManager> m_LayoutManager;
    std::unique_ptr<UI::IStyleManager> m_StyleManager;
    std::unique_ptr<UI::IAnimationController> m_AnimationController;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 