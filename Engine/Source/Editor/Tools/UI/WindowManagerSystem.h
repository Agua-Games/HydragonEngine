/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor window management and docking system
 */

#pragma once
#include "UITypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct WindowManagerConfig {
    bool enableDocking = true;
    bool enableTabbing = true;
    bool enableSnapping = true;
    bool enableAutoHide = true;
    uint32_t maxDockedWindows = 50;
    uint32_t maxTabbedGroups = 20;
    float dockingThreshold = 20.0f;
};

class WindowManagerSystem {
public:
    static WindowManagerSystem& Get();
    
    void Initialize(const WindowManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    DockID CreateDockSpace(const std::string& name, const DockSpaceDesc& desc);
    void DestroyDockSpace(DockID id);
    
    void DockWindow(const std::string& window, DockID dock, DockSlot slot);
    void UndockWindow(const std::string& window);
    
    void GroupWindows(const std::vector<std::string>& windows);
    void UngroupWindows(const std::vector<std::string>& windows);
    
    void SetWindowVisibility(const std::string& window, bool visible);
    bool IsWindowVisible(const std::string& window) const;
    
    const WindowManagerStats& GetStats() const { return m_Stats; }

private:
    WindowManagerSystem() = default;
    
    WindowManagerConfig m_Config;
    WindowManagerStats m_Stats;
    std::unique_ptr<IDockingManager> m_DockingManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 