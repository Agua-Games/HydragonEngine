/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BlendSpace.h
 * @brief Header file for the BlendSpace class.
 * 
 * ARCHITECTURAL NOTES:
 * - BlendSpace is a class that represents a blend space in Hydragon.
 * - It has support to choose how many dimensions to blend on. (1D, 2D, 3D, etc.), adapting accordingly.
 * - It is used to blend between different animation states.
 * - It uses the Vulkan API for blending.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Node.h"
#include "Animation.h"

namespace hd {

struct BlendSpaceInfo : public NodeInfo {
    BlendSpaceInfo() {
        NodeType = "Animation/BlendSpace";
        
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

class BlendSpace : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BlendSpace(const BlendSpaceInfo& info = BlendSpaceInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    int numDimensions;                           // Number of dimensions to blend on
    std::vector<AnimationState> animationStates; // Animation states to blend
    std::vector<std::string> blendParameters;    // Parameters for blending
    std::vector<float> blendPosition;            // Position in the blend space to blend to
    std::vector<float> blendWeights;             // Weights for each state

    // === Processing ===
    void addState(const std::string &name, 
        const std::vector<float>& blendPosition, 
        const std::string& animationPath); // Add an animation state to blend on a specific position in the blend space
    
    void blendAnimations();

    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BlendSpace() = default;     // Default destructor
};

} // namespace hd

