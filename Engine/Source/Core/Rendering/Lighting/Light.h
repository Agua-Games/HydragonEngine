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
#include <string>
#include "Node.h"

namespace hd {

struct LightInfo : public NodeInfo {
    LightInfo() {
        NodeType = "Rendering/Light";
        
        inputs = {
            "Type",          // Light type (point, directional, etc.)
            "Color",         // Light color
            "Intensity",     // Light intensity
            "Position",      // Light position
            "Direction",     // Light direction
            "Attenuation",   // Light attenuation
            "Shadow",        // Shadow casting
            "AreaSize"       // Area light size
        };
        
        outputs = {
            "LightData",     // Light data for rendering
            "ShadowMap",     // Shadow map data
            "LightMetrics"   // Performance metrics
        };
    }
};

class Light : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Light(const LightInfo& info = LightInfo())
        : Node(info), LightInfo(info) {}                 // Constructor with default info object
    void initialize() override {}                        // Initialize the node (optional)
    void load() override {}                              // Load the node (optional)

    // === Processing ===
    void processNodeGraph() override;
    void update();

    // === Cleanup ===
    void unload() override {}                            // Unload the node (optional)
    void cleanup() override {}                           // Cleanup the node (optional)
    ~Light() = default;                                  // Default destructor
};

} // namespace hd
