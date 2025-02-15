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
    
    // =========== UI Resources Handling ===========
    /**
     * @brief Load fonts
     */
    void LoadFonts();

private:
    // Private constructor to prevent instantiation
    ResourceManager() {}

    // ResourceManager is a singleton, so delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    // default member initializers
    // ...
};
}
