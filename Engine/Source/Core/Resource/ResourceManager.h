/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ResourceManager.h
 * @brief Header file for the ResourceManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - ResourceManager is a singleton class that manages the loading and management of resources.
 * - It is responsible for loading and managing resources such as textures, models, and shaders.
 * - It uses the Vulkan API for resource management.
 * 
 * TODO:
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
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
    
    /**
     * @brief Get the path to an icon font
     * @param iconFontName The name of the icon font
     * @returns The path to the icon font
     */
    static std::string GetIconFontPath(const std::string& iconFontName);
    
    /**
     * @brief Load fonts for ImGui
     */
    void LoadFonts();

    // Getters for fonts
    ImFont* GetDefaultFont() const { return m_defaultFont; }
    ImFont* GetIconFont() const { return m_iconFont; }

private:
    // Private constructor to prevent instantiation
    ResourceManager() : m_defaultFont(nullptr), m_iconFont(nullptr) {}

    // ResourceManager is a singleton, so delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // default member initializers
    ImFont* m_defaultFont = nullptr;
    ImFont* m_iconFont = nullptr;
};
}
