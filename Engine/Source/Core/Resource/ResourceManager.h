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
        // ResourceManager is a singleton, so get the singleton instance (the only instance).
        static ResourceManager& GetInstance();

        // =========== Store Resource Paths ===========
        // Get the engine root path
        fs::path GetEngineRootPath();

        // Get the path to a font
        static std::string GetFontPath(const std::string& fontName);
        
        // =========== UI Resources Handling ===========
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
