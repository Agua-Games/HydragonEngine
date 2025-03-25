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
#include <string>
#include "Node.h"
#include "Light.h"

namespace hd {

struct LightingSetupInfo : public NodeInfo {
    LightingSetupInfo() {
        NodeType = "Rendering/LightingSetup";
        inputs = {
            "Lights",        // Array of lights
            "EnvironmentMap" // Environment map for global lighting
        };
        outputs = {
            "LightingData",  // Lighting data for rendering
            "LightingMetrics"// Performance metrics
        };
    }
};

class LightingSetup : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit LightingSetup(const LightingSetupInfo& info = LightingSetupInfo())
        : Node(info) {}   
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
    ~LightingSetup() = default;     // Default destructor
};

} // namespace hd