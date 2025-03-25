/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SingleChainIK.h
 * @brief Header file for the SingleChainIK class.
 * 
 * ARCHITECTURAL NOTES:
 * - SingleChainIK is a class that represents a single chain inverse kinematics in Hydragon.
 * - It is used to animate a single chain and its associated geometry, joints or any other arbitrary data.
 * - It offers built-in support for procedural modification of imported animations, and for procedural animation generation.
 * - It uses the Vulkan API for animation processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"
#include "SkeletonDeformer.h"

namespace hd {

struct SingleChainIKInfo : public AnimationInfo {
    SingleChainIKInfo() {
        NodeType = "Animation/SingleChainIK";
        
        inputs = {
            "Chain",           // Chain data
            "AnimationClip",   // Animation clip data
            "Time",            // Current time
            "PlaybackSpeed",   // Playback speed
            "Looping"          // Whether to loop the animation
        };
        
        outputs = {
            "AnimatedChain",   // Animated chain data
            "AnimationMetrics" // Performance and quality metrics
        };
    }
};

class SingleChainIK : public Animation {
public:
    // === Allocation, Initialization, Loading ===
    explicit SingleChainIK(const SingleChainIKInfo& info = SingleChainIKInfo())
        : Animation(info) {}
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
    ~SingleChainIK() = default;     // Default destructor
};

} // namespace hd
