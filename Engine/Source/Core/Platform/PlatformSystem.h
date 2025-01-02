/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core platform abstraction system
 */

#pragma once
#include "PlatformTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <string>

namespace Hydragon::Platform {

class PlatformSystem {
public:
    struct PlatformConfig {
        bool enableHighDPI = true;
        bool enableRawInput = true;
        bool enableMultitouch = true;
        bool enableGamepad = true;
        bool enableClipboard = true;
        bool catchExceptions = true;
        uint32_t maxWindows = 16;
        size_t memoryBudget = 32 * 1024 * 1024;  // 32MB
    };

    static PlatformSystem& Get();
    
    void Initialize(const PlatformConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Window management
    WindowHandle CreateWindow(const WindowDesc& desc);
    void DestroyWindow(WindowHandle handle);
    void SetWindowTitle(WindowHandle handle, const std::string& title);
    void SetWindowSize(WindowHandle handle, uint32_t width, uint32_t height);
    
    // Input handling
    void PollEvents();
    bool GetKeyState(KeyCode key) const;
    bool GetMouseButton(MouseButton button) const;
    Vector2 GetMousePosition() const;
    
    // System info
    const SystemInfo& GetSystemInfo() const;
    const DisplayInfo& GetDisplayInfo() const;
    const ProcessorInfo& GetProcessorInfo() const;
    
    // File system
    bool FileExists(const std::string& path) const;
    bool CreateDirectory(const std::string& path);
    std::string GetExecutablePath() const;
    
    const PlatformStats& GetStats() const { return m_Stats; }

private:
    PlatformSystem() = default;
    
    PlatformConfig m_Config;
    PlatformStats m_Stats;
    std::unique_ptr<IPlatformLayer> m_Platform;
    bool m_Initialized = false;
};

} // namespace Hydragon::Platform 