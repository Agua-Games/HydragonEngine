/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Platform system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Platform {

using WindowHandle = uint32_t;

enum class PlatformType {
    Windows,
    Linux,
    MacOS,
    Android,
    IOS,
    Web
};

enum class KeyCode {
    None = 0,
    Escape,
    Enter,
    Space,
    Backspace,
    Tab,
    // ... other key codes
};

enum class MouseButton {
    Left,
    Right,
    Middle,
    X1,
    X2
};

struct Vector2 {
    float x, y;
};

struct WindowDesc {
    std::string title;
    uint32_t width = 1280;
    uint32_t height = 720;
    bool fullscreen = false;
    bool resizable = true;
    bool decorated = true;
    bool vsync = true;
};

struct SystemInfo {
    PlatformType platform;
    std::string osName;
    std::string osVersion;
    size_t totalMemory;
    size_t availableMemory;
    std::string deviceName;
    std::string deviceModel;
};

struct DisplayInfo {
    uint32_t width;
    uint32_t height;
    uint32_t refreshRate;
    float dpiScale;
    bool hdrSupported;
    std::vector<std::string> supportedResolutions;
};

struct ProcessorInfo {
    std::string vendor;
    std::string model;
    uint32_t cores;
    uint32_t threads;
    uint32_t clockSpeed;
    bool hasAVX;
    bool hasSSE4;
    std::vector<std::string> extensions;
};

struct PlatformStats {
    uint32_t activeWindows;
    uint32_t eventCount;
    uint32_t frameCount;
    float frameTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Platform 