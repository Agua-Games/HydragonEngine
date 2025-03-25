/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Blendspace.h
 * @brief Header file for the Blendspace class.
 * 
 * ARCHITECTURAL NOTES:
 * - Blendspace is a class that represents a blend space in Hydragon.
 * - It is used to blend between different animation states.
 * - It uses the Vulkan API for blending.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"

namespace hd {

struct BlendspaceInfo : public NodeInfo {
    BlendspaceInfo() {
        NodeType = "Animation/Blendspace";
        
        inputs = {
            "AnimationStates",  // Array of animation states
            "BlendParameters",  // Parameters for blending
            "BlendWeights",     // Weights for each state
            "BlendMode"         // Blending mode
        };
        
        outputs = {
            "BlendedAnimation", // Resulting blended animation
            "BlendMetrics"      // Performance and quality metrics
        };
    }
};

class Blendspace : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Blendspace(const BlendspaceInfo& info = BlendspaceInfo())
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
    ~Blendspace() = default;     // Default destructor
};

} // namespace hd

