/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Window management system
 */

#pragma once
#include "../PlatformTypes.h"
#include <memory>
#include <vector>

namespace Hydragon::Platform {

class WindowSystem {
public:
    struct WindowConfig {
        bool enableTransparency = true;
        bool enableDragDrop = true;
        bool enableMultiWindow = true;
        uint32_t maxWindows = 16;
        uint32_t defaultWidth = 1280;
        uint32_t defaultHeight = 720;
    };

    static WindowSystem& Get();
    
    void Initialize(const WindowConfig& config = {});
    void Shutdown();
    
    void Update();
    
    WindowHandle CreateWindow(const WindowDesc& desc);
    void DestroyWindow(WindowHandle handle);
    
    void SetWindowPosition(WindowHandle handle, int x, int y);
    void SetWindowSize(WindowHandle handle, uint32_t width, uint32_t height);
    void SetWindowTitle(WindowHandle handle, const std::string& title);
    void SetWindowIcon(WindowHandle handle, const std::string& iconPath);
    
    void ShowWindow(WindowHandle handle);
    void HideWindow(WindowHandle handle);
    void MinimizeWindow(WindowHandle handle);
    void MaximizeWindow(WindowHandle handle);
    
    bool IsWindowFocused(WindowHandle handle) const;
    bool IsWindowMinimized(WindowHandle handle) const;
    bool IsWindowMaximized(WindowHandle handle) const;
    
    const WindowStats& GetStats() const { return m_Stats; }

private:
    WindowSystem() = default;
    
    WindowConfig m_Config;
    WindowStats m_Stats;
    std::vector<Window> m_Windows;
    std::unique_ptr<IWindowManager> m_Manager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Platform 