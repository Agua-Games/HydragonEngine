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
#include "IconsFontAwesome6.h"

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

std::string ResourceManager::GetIconFontPath(const std::string& iconFontName) {
    fs::path engineRoot = ResourceManager::GetInstance().GetEngineRootPath();
    return (engineRoot / "Assets" / "Fonts" / "Icons" / iconFontName).string();
}

void ResourceManager::LoadFonts() {
    ImGuiIO& io = ImGui::GetIO();

    // Load default font
    m_defaultFont = io.Fonts->AddFontDefault();
    if (!m_defaultFont) {
        throw std::runtime_error("Failed to load default ImGui font");
    }

    // Load Font Awesome icon font
    std::string iconFontPath = GetIconFontPath(FONT_ICON_FILE_NAME_FAR);  // This will use "fa-regular-400.ttf"
    
    // Icon font configuration
    ImFontConfig icons_config;
    icons_config.MergeMode = true;           // Merge icons into the previous font
    icons_config.PixelSnapH = true;          // Align icons to pixel grid
    icons_config.GlyphMinAdvanceX = 16.0f;   // Make icons monospaced
    icons_config.GlyphOffset = ImVec2(0, 2); // Fine-tune icon position
    
    // Define icon range (from IconsFontAwesome6.h)
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    
    // Load the icon font
    m_iconFont = io.Fonts->AddFontFromFileTTF(
        iconFontPath.c_str(),
        16.0f,              // Font size
        &icons_config,      // Font config
        icons_ranges        // Icon ranges
    );
    
    if (!m_iconFont) {
        throw std::runtime_error("Failed to load icon font: " + iconFontPath);
    }

    // Build font atlas
    io.Fonts->Build();
}
}

