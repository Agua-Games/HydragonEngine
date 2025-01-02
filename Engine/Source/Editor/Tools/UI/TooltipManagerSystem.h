/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor tooltip and help system
 */

#pragma once
#include "UITypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct TooltipManagerConfig {
    bool enableRichTooltips = true;
    bool enableDelayedShow = true;
    bool enableAnimation = true;
    bool enableKeyboardTips = true;
    float showDelay = 0.5f;
    float hideDelay = 0.1f;
    Vector2 offset = {10.0f, 10.0f};
    std::string styleSheet = "Editor/Styles/Tooltips.css";
};

class TooltipManagerSystem {
public:
    static TooltipManagerSystem& Get();
    
    void Initialize(const TooltipManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterTooltip(const std::string& id, const TooltipDesc& desc);
    void UnregisterTooltip(const std::string& id);
    
    void ShowTooltip(const std::string& id, const Vector2& position);
    void HideTooltip(const std::string& id);
    void HideAllTooltips();
    
    void SetTooltipContent(const std::string& id, const std::string& content);
    void SetTooltipStyle(const std::string& id, const TooltipStyle& style);
    
    bool IsTooltipVisible(const std::string& id) const;
    const TooltipDesc& GetTooltipDesc(const std::string& id) const;
    
    const TooltipManagerStats& GetStats() const { return m_Stats; }

private:
    TooltipManagerSystem() = default;
    
    TooltipManagerConfig m_Config;
    TooltipManagerStats m_Stats;
    std::unordered_map<std::string, TooltipInstance> m_Tooltips;
    std::unique_ptr<ITooltipRenderer> m_Renderer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 