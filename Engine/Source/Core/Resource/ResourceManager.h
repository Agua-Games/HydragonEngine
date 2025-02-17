/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include <string>
#include <filesystem>
#include <imgui.h>

namespace fs = std::filesystem;

namespace hd 
{
/**
 * @brief The ResourceManager is a singleton class responsible for handling resource loading and management.
 */
class ResourceManager 
{
public:
    /**
     * @brief Get the singleton instance of ResourceManager
     * @returns The singleton instance
     */
    static ResourceManager& GetInstance();

    // =========== Store Resource Paths ===========
    /**
     * @brief Get the engine root path
     * @returns The engine root path
     */
    fs::path GetEngineRootPath();

    /**
     * @brief Get the path to a font
     * @param fontName The name of the font
     * @returns The path to the font
     */
    static std::string GetFontPath(const std::string& fontName);
    
    // Add new method for icon font loading
    static std::string GetIconFontPath(const std::string& iconFontName);
    
    // Modified to handle both regular fonts and icon fonts
    void LoadFonts();
    ImFont* GetDefaultFont() const { return m_defaultFont; }
    ImFont* GetIconFont() const { return m_iconFont; }

private:
    // Private constructor to prevent instantiation
    ResourceManager() : m_defaultFont(nullptr), m_iconFont(nullptr) {}

    // ResourceManager is a singleton, so delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    // default member initializers
    // ...
    ImFont* m_defaultFont;
    ImFont* m_iconFont;
};
}
