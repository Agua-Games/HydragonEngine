/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LightingSetup.h
 * @brief Header file for the LightingSetup class.
 * 
 * ARCHITECTURAL NOTES:
 * - LightingSetup is a node that represents a lighting setup (including, for instance, a light rig for studio lighting) in the scene.
 * - It can be used to create different types of lighting setups with various properties.
 * - It uses the Vulkan API for light management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "Node.h"
#include "Light.h"

namespace hd {

struct LightingSetupInfo : public NodeInfo {
    LightingSetupInfo() {
        NodeType = "Rendering/LightingSetup";
        inputs = {
            "Lights",           // Array of lights
            "EnvironmentMap"    // Environment map for global lighting
        };
        outputs = {
            "LightingData",     // Lighting data for rendering
            "LightingMetrics"   // Performance metrics
        };
    }
};

/**
 * @class LightingSetup.
 * @brief LightingSetup represents a ready-to-use customizable lighting setup node in the engine's node graph.
 */
class LightingSetup : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit LightingSetup(const LightingSetupInfo& info = LightingSetupInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    std::vector<std::shared_ptr<Light>> lights;
    std::string environmentMap;
    bool dynamicShadows = true;
    glm::vec3 ambientColor = glm::vec3(0.1f);
    glm::vec3 directionalLightDirection = glm::vec3(0, -1, 0);
    glm::vec3 directionalLightColor = glm::vec3(0.5f);

    // === Processing ===
    void enableDynamicShadows(bool enable);
    void addLight(const std::shared_ptr<Light>& light);
    void removeLight(const std::shared_ptr<Light>& light);
    void setAmbient(const glm::vec3& color);
    void setEnvironmentMap(const std::string& path);
    void processLights();
    void processEnvironmentMap();
    void processNode() override {
 
    }
    void update(
        processLights();
        processEnvironmentMap();
    );

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LightingSetup() = default;     // Default destructor
};

} // namespace hd