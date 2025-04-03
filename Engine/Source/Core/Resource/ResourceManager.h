/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
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
 * @todo Create .cpp file and move the implementation there.
 * @todo Organize the existing code into logical sections and functions.
 * @todo Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * @todo Flesh out the class and its methods, structs, enums, etc.
 */
#pragma once
#include <string>
#include <filesystem>
#include <stdexcept>
#include <imgui.h>
#include "Node.h"

namespace fs = std::filesystem;

namespace hd {

struct ResourceInfo : public NodeInfo {
    ResourceInfo() {
        NodeType = "Resource";
        inputs = {
            "ResourcePath",     // Path to the resource
            "ResourceData",     // Resource data
            "ResourceMetadata"  // Resource metadata
        };
        outputs = {
            "LoadedResource",   // Loaded resource
            "ResourceStatus",   // Resource loading status
            "ResourceMetadata"  // Resource metadata
        }
    }
};

/**
 * @brief The ResourceManager is a singleton class responsible for handling resource loading and management.
 */
class ResourceManager : public Node {
public:
    // === Structure Definitions ===
    enum class ResourceType {
        Texture,
        Model,
        Shader,
        Other
    };

    enum class PriorityScheme {
        Distance,
        UsageFrequency,
        Custom
    };

    enum class PriorityRuleType {
        Distance,
        UsageFrequency,
        Custom
    };

    struct PriorityRule {
        PriorityRuleType type;
        float weight;
    };

    // === Allocation, Initialization, Loading ===
    // Public static method to get the singleton instance of ResourceManager class. This method is thread-safe.
    static ResourceManager& getInstance();
    void initialize() override {}
    void load() override {}

    // Set default values.
    // In general, what takes up most gpu memory in a game are textures, including texture pages for shadows (like tiled/virtual shadows or depth map shadows), for
    // tiled atlas textures (aka megatextures, virtual textures)
    int memoryBudget = 1024;                  // in MB
    PriorityScheme priorityScheme = PriorityScheme::Distance;
    bool backgroundLoading = true;
    std::vector<PriorityRule> priorityRules;

    // === Resource Management ===
    // Getters for paths
    fs::path getEngineRootPath();
    static std::string getFontPath(const std::string& fontName);
    static std::string getIconFontPath(const std::string& iconFontName);
    
    /**
     * @brief Load fonts for ImGui
     */
    void loadFonts();
    
    // Getters for fonts. These fonts are used for ImGui.
    ImFont* getDefaultFont() const { return m_defaultFont; }
    ImFont* getIconFont() const { return m_iconFont; }

    // === Processing ===
    void processNode() override {
 
    }
    void addResource(const std::string& resourcePath);
    void removeResource(const std::string& resourcePath);
    void addRule(const PriorityRule& rule);
    void removeRule(const PriorityRule& rule);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ResourceManager() = default;     // Default destructor

private:
    // === Allocation, Initialization, Loading ===
    // Private constructor to prevent instantiation
    ResourceManager() : m_defaultFont(nullptr), m_iconFont(nullptr) {}

    // ResourceManager is a singleton, so delete copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // default member initializers
    ImFont* m_defaultFont = nullptr;
    ImFont* m_iconFont = nullptr;
};

} // namespace hd