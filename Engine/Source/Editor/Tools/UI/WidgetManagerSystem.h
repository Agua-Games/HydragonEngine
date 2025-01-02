/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor widget and control management system
 */

#pragma once
#include "UITypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct WidgetManagerConfig {
    bool enableCustomWidgets = true;
    bool enableWidgetCache = true;
    bool enableDragDrop = true;
    bool enableTooltips = true;
    uint32_t maxCustomWidgets = 500;
    uint32_t cacheSize = 1024 * 1024;  // 1MB
    float tooltipDelay = 0.5f;
};

class WidgetManagerSystem {
public:
    static WidgetManagerSystem& Get();
    
    void Initialize(const WidgetManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterWidget(const std::string& type, std::unique_ptr<IWidget> widget);
    void UnregisterWidget(const std::string& type);
    
    WidgetHandle CreateWidget(const std::string& type, const WidgetDesc& desc);
    void DestroyWidget(WidgetHandle handle);
    
    void SetWidgetProperty(WidgetHandle handle, const std::string& name, 
                          const PropertyValue& value);
    PropertyValue GetWidgetProperty(WidgetHandle handle, const std::string& name) const;
    
    void EnableDragDrop(WidgetHandle handle, const DragDropConfig& config);
    void DisableDragDrop(WidgetHandle handle);
    
    const WidgetManagerStats& GetStats() const { return m_Stats; }

private:
    WidgetManagerSystem() = default;
    
    WidgetManagerConfig m_Config;
    WidgetManagerStats m_Stats;
    std::unordered_map<std::string, std::unique_ptr<IWidget>> m_WidgetTypes;
    std::unordered_map<WidgetHandle, WidgetInstance> m_Widgets;
    std::unique_ptr<IWidgetFactory> m_Factory;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 