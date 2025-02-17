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
#include "IconsMaterialSymbols.h"

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
    return (engineRoot / "Engine" / "Assets" / "Fonts" / fontName).string();
}

std::string ResourceManager::GetIconFontPath(const std::string& iconFontName) {
    fs::path engineRoot = ResourceManager::GetInstance().GetEngineRootPath();
    fs::path fullPath = engineRoot / "Engine "/ "Assets" / "Fonts" / "Icons" / iconFontName;
    
    // Debug prints
    printf("Engine Root: %s\n", engineRoot.string().c_str());
    printf("Full Icon Font Path: %s\n", fullPath.string().c_str());
    printf("Path exists: %s\n", fs::exists(fullPath) ? "Yes" : "No");
    
    return fullPath.string();
}

void ResourceManager::LoadFonts() {
    ImGuiIO& io = ImGui::GetIO();

    // Load font
    m_defaultFont = io.Fonts->AddFontFromFileTTF("D:\\AguaGames\\HydragonEngine\\alpha\\Engine\\Assets\\Fonts\\Roboto-Regular.ttf", 15.0f);
    //m_defaultFont = io.Fonts->AddFontFromFileTTF(GetFontPath("Roboto-Regular.ttf").c_str(), 16.0f);
    if (!m_defaultFont) {
        throw std::runtime_error("Failed to load default ImGui font");
    }

    // Load Font Awesome icon font
    std::string iconFontPath = GetIconFontPath(FONT_ICON_FILE_NAME_FAR);
    
    // Debug print
    printf("Loading icon font from: %s\n", iconFontPath.c_str());
    
    // Configure and merge the icon font - FontAwesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = 0.0f;
    icons_config.GlyphMaxAdvanceX = 0.0f;
    icons_config.GlyphOffset = ImVec2(0, 12.0f);
    
    // Load the icon font - FontAwesome
    m_iconFont = io.Fonts->AddFontFromFileTTF(
        //iconFontPath.c_str(),
        //"D:\\AguaGames\\HydragonEngine\\alpha\\Engine\\Assets\\Fonts\\Icons\\fa-6-regular-solid-900.ttf",
        "D:\\AguaGames\\HydragonEngine\\alpha\\Engine\\Assets\\Fonts\\Icons\\ms-sharp-filled-regular.ttf",
        38.0f,
        &icons_config,
        icons_ranges
    );
    
    #if 0
    if (!m_iconFont) {
        printf("Failed to load icon font!\n");
        throw std::runtime_error("Failed to load icon font: " + iconFontPath);
    } else {
        printf("Successfully loaded icon font\n");
    }
    #endif

    // Build font atlas
    io.Fonts->Build();
}
}

