/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor menu and toolbar management system
 */

#pragma once
#include "UITypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct MenuManagerConfig {
    bool enableCustomMenus = true;
    bool enableHotkeys = true;
    bool enableContextMenus = true;
    bool enableDynamicMenus = true;
    uint32_t maxMenuDepth = 5;
    uint32_t maxRecentItems = 10;
    std::string menuConfigPath = "Editor/Config/Menus";
};

class MenuManagerSystem {
public:
    static MenuManagerSystem& Get();
    
    void Initialize(const MenuManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    MenuHandle CreateMenu(const std::string& path, const MenuDesc& desc);
    void DestroyMenu(MenuHandle handle);
    
    void AddMenuItem(MenuHandle menu, const MenuItemDesc& desc);
    void RemoveMenuItem(MenuHandle menu, const std::string& id);
    
    void RegisterMenuAction(const std::string& id, const MenuActionCallback& callback);
    void UnregisterMenuAction(const std::string& id);
    
    void ShowContextMenu(const std::string& type, const Vector2& position);
    void HideContextMenu();
    
    const MenuManagerStats& GetStats() const { return m_Stats; }

private:
    MenuManagerSystem() = default;
    
    MenuManagerConfig m_Config;
    MenuManagerStats m_Stats;
    std::unordered_map<MenuHandle, MenuInstance> m_Menus;
    std::unordered_map<std::string, MenuActionCallback> m_Actions;
    std::unique_ptr<IMenuBuilder> m_Builder;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 