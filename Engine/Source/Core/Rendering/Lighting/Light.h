/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Light.h
 * @brief Header file for the Light class.
 * 
 * ARCHITECTURAL NOTES:
 * - Light is a node that represents a light source in the scene.
 * - It can be used to create different types of lights with various properties.
 * - It uses the Vulkan API for light management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/glm.hpp>
#include <string>
#include "Node.h"

namespace hd {

enum class LightType {
    Point,
    Directional,
    Spot,
    Area
};

struct LightInfo : public NodeInfo {
    LightInfo() {
        nodeType = "Rendering/Light";
        
        inputs = {
            "type",          // Light type (point, directional, etc.)
            "color",         // Light color
            "intensity",     // Light intensity
            "position",      // Light position
            "direction",     // Light direction
            "attenuation",   // Light attenuation
            "shadow",        // Shadow casting
            "areaSize"       // Area light size
        };
        
        outputs = {
            "lightData",     // Light data for rendering
            "shadowMap",     // Shadow map data
            "lightMetrics"   // Performance metrics
        };
    }
};

class Light : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Light(const LightInfo& info = LightInfo())
        : Node(info) {}                                  // Constructor with default info object
    void initialize() override {}                        // Initialize the node (optional)
    void load() override {}                              // Load the node (optional)

    // Set default values
    LightType type = LightType::Point;
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
    float attenuation = 1.0f;
    bool shadow = false;
    glm::vec2 areaSize = glm::vec2(1.0f);

    // === Processing ===
    void processNode() override {
        type = getInputValue<LightType>("type");
        color = getInputValue<glm::vec3>("color");
        intensity = getInputValue<float>("intensity");
        position = getInputValue<glm::vec3>("position");
        direction = getInputValue<glm::vec3>("direction");
        attenuation = getInputValue<float>("attenuation");
        shadow = getInputValue<bool>("shadow");
        areaSize = getInputValue<glm::vec2>("areaSize");

        // Process light
        auto lightData = updateLight(type, color, intensity, position, direction, attenuation, shadow, areaSize);

        // Set outputs
        setOutputValue("lightData", lightData);
        setOutputValue("shadowMap", computeShadowMap(lightData));
        setOutputValue("lightMetrics", computeLightMetrics(lightData));
    }
    void update();

    // === Cleanup ===
    void unload() override {}                            // Unload the node (optional)
    void cleanup() override {}                           // Cleanup the node (optional)
    ~Light() = default;                                  // Default destructor
};

} // namespace hd
