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
 * - It supports procedural contact placement using raycasts/traces (e.g. for conforming to surfaces, terrain).
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
        nodeType = "Animation/SingleChainIK";
        
        inputs = {
            "chain",           // Chain data
            "animationClip",   // Animation clip data
            "time",            // Current time
            "playbackSpeed",   // Playback speed
            "looping"          // Whether to loop the animation
        };
        
        outputs = {
            "animatedChain",   // Animated chain data
            "animationMetrics" // Performance and quality metrics
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

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SingleChainIK() = default;     // Default destructor
};

} // namespace hd
