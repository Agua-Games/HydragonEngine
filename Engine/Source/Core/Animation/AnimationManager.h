/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AnimationManager.h
 * @brief Header file for the AnimationManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - AnimationManager is a singleton class that manages the animation system.
 * - It is responsible for loading, playing, pausing, and stopping animations.
 * - It uses the Vulkan API for animation processing.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"

namespace hd {

struct AnimationManagerInfo : public NodeInfo {
    AnimationManagerInfo() {
        NodeType = "Animation/AnimationManager";
        
        inputs = {
            "Animations",      // Array of animations
            "AnimationState",  // Current state of the animation
            "AnimationSpeed",  // Speed of the animation
            "AnimationLoop"    // Whether the animation should loop
        };
        
        outputs = {
            "ActiveAnimations", // Active animations
            "AnimationMetrics"  // Performance and quality metrics
        };
    }
};

class AnimationManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AnimationManager(const AnimationManagerInfo& info = AnimationManagerInfo())
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
    ~AnimationManager() = default;     // Default destructor
};

} // namespace hd
