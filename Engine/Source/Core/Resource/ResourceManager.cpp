/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include <string>
#include <filesystem>
#include <stdexcept>
#include <imgui.h>
#ifdef _WIN32           // If compiling on Windows
    #include <windows.h>
#endif

#include "ResourceManager.h"

//namespace fs = std::filesystem;
namespace hd {
ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

fs::path ResourceManager::GetEngineRootPath() {
    // Get the path to the executable, using a different approach for each platform
    char buffer[1024]; // Fixed-size path buffer for all platforms

    #ifdef _WIN32
        // Windows: Use GetModuleFileNameA
        if (GetModuleFileNameA(NULL, buffer, sizeof(buffer)) == 0) {
            throw std::runtime_error("Failed to get executable path on Windows");
        }
    #elif __linux__
        // Linux: Use /proc/self/exe
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len == -1) {
            throw std::runtime_error("Failed to get executable path on Linux");
        }
        buffer[len] = '\0'; // Null-terminate the string
    #elif __APPLE__
        // macOS: Use _NSGetExecutablePath
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) != 0) {
            throw std::runtime_error("Failed to get executable path on macOS");
        }
    #else
        #error "Unsupported platform"
    #endif
    
    // Resolve the canonical path
    fs::path executablePath = fs::canonical(fs::path(buffer));

    // The executable is located in /Engine/Bin/(platform)/(config)/, so we need to go up 3 levels
    // to get to /Engine.
    return executablePath.parent_path().parent_path().parent_path();
}

std::string ResourceManager::GetFontPath(const std::string& fontName) {
    fs::path engineRoot = ResourceManager::GetInstance().GetEngineRootPath();
    return (engineRoot / "Assets" / "Fonts" / fontName).string();
}

// =========== UI Resources Handling ===========
void LoadFonts() {
    // Get ImGuiIO (imgui's Input Output system)
    ImGuiIO& io = ImGui::GetIO();

    // *If we decide to have more than one font or font icon, we should put the commands below inside of a for loop.
    
    // Get the path to each font using the resource manager
    std::string fontPath = ResourceManager::GetFontPath("font.ttf");

    // Get the path to each font ICON using the resource manager
    std::string fontIconPath = ResourceManager::GetFontPath("font.ttf");

    // Load the font
    io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f);
}
}

