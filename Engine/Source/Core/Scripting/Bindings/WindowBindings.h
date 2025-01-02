/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Window system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Window/WindowSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct WindowBindingsConfig {
    bool enableMultiWindow = true;
    bool enableFullscreen = true;
    bool enableResizing = true;
    bool enableDragAndDrop = true;
    uint32_t maxWindows = 16;
    uint32_t maxMonitors = 8;
    uint32_t maxDropFiles = 100;
    std::string windowConfigPath = "Config/Window";
};

class WindowBindings {
public:
    static WindowBindings& Get();
    
    void Initialize(const WindowBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Window management
    WindowHandle CreateWindow(const WindowDesc& desc);
    void DestroyWindow(WindowHandle handle);
    void ShowWindow(WindowHandle handle);
    void HideWindow(WindowHandle handle);
    
    // Window properties
    void SetTitle(WindowHandle handle, const std::string& title);
    void SetSize(WindowHandle handle, uint32_t width, uint32_t height);
    void SetPosition(WindowHandle handle, int32_t x, int32_t y);
    void SetFullscreen(WindowHandle handle, bool fullscreen);
    void SetBorderless(WindowHandle handle, bool borderless);
    void SetResizable(WindowHandle handle, bool resizable);
    void SetMinSize(WindowHandle handle, uint32_t width, uint32_t height);
    void SetMaxSize(WindowHandle handle, uint32_t width, uint32_t height);
    
    // Window state queries
    bool IsWindowVisible(WindowHandle handle) const;
    bool IsWindowFocused(WindowHandle handle) const;
    bool IsWindowFullscreen(WindowHandle handle) const;
    bool IsWindowMinimized(WindowHandle handle) const;
    bool IsWindowMaximized(WindowHandle handle) const;
    
    // Monitor management
    MonitorHandle GetPrimaryMonitor() const;
    std::vector<MonitorHandle> GetMonitors() const;
    MonitorInfo GetMonitorInfo(MonitorHandle handle) const;
    
    // Event handling
    void RegisterWindowCallback(WindowHandle handle, const WindowCallback& callback);
    void UnregisterWindowCallback(WindowHandle handle);
    void RegisterDropCallback(WindowHandle handle, const DropCallback& callback);
    void UnregisterDropCallback(WindowHandle handle);
    
    // Cursor control
    void ShowCursor(WindowHandle handle, bool show);
    void SetCursorPosition(WindowHandle handle, int32_t x, int32_t y);
    void GetCursorPosition(WindowHandle handle, int32_t& x, int32_t& y) const;
    void SetCursorMode(WindowHandle handle, CursorMode mode);
    
    const WindowBindingsStats& GetStats() const { return m_Stats; }

private:
    WindowBindings() = default;
    
    WindowBindingsConfig m_Config;
    WindowBindingsStats m_Stats;
    std::unordered_map<WindowHandle, WindowInstance> m_Windows;
    std::unordered_map<WindowHandle, WindowCallback> m_WindowCallbacks;
    std::unordered_map<WindowHandle, DropCallback> m_DropCallbacks;
    std::vector<MonitorHandle> m_Monitors;
    std::unique_ptr<Window::IWindowManager> m_WindowManager;
    std::unique_ptr<Window::IMonitorManager> m_MonitorManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 