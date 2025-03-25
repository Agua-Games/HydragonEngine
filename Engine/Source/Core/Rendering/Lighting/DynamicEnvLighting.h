/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DynamicEnvLighting.h
 * @brief Header file for the DynamicEnvLighting class.
 * 
 * ARCHITECTURAL NOTES:
 * - DynamicEnvLighting is a node that represents dynamic environment lighting (Time-of-day, weather, etc.) in the scene.
 * - It can be used to create different types of dynamic environment lighting with various properties.
 * - It uses the Vulkan API for light management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Node.h"
#include "Light.h"

namespace hd {

struct DynamicEnvLightingInfo : public LightInfo {
    DynamicEnvLightingInfo() {
        NodeType = "Rendering/DynamicEnvLighting";
        inputs.insert(inputs.end(), {
            "TimeOfDay",       // Time of day (day, night, etc.)
            "Weather",         // Weather conditions (sunny, rainy, etc.)
            "Season",          // Season (spring, summer, etc.)
            "Location"         // Geographic location
        });
        outputs.insert(outputs.end(), {
            "DynamicLightData" // Dynamic light data for rendering
        });
    }
};

class DynamicEnvLighting : public Light {
public:
    // === Allocation, Initialization, Loading ===
    explicit DynamicEnvLighting(const DynamicEnvLightingInfo& info = DynamicEnvLightingInfo())
        : Light(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DynamicEnvLighting() = default;     // Default destructor
};

} // namespace hd