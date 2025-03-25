/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FullBodyIK.h
 * @brief Header file for the FullBodyIK class.
 * 
 * ARCHITECTURAL NOTES:
 * - FullBodyIK is a class that represents a full body inverse kinematics in Hydragon.
 * - It is used to animate a full body and its associated geometry, joints or any other arbitrary data.
 * - It offers built-in support for procedural modification of imported animations, and for procedural animation generation.
 * - It uses the Vulkan API for animation processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"
#include "SkeletonDeformer.h"

namespace hd {

struct FullBodyIKInfo : public AnimationInfo {
    FullBodyIKInfo() {
        NodeType = "Animation/FullBodyIK";
        
        inputs = {
            "Body",            // Body data
            "AnimationClip",   // Animation clip data
            "Time",            // Current time
            "PlaybackSpeed",   // Playback speed
            "Looping"          // Whether to loop the animation
        };
        
        outputs = {
            "AnimatedBody",    // Animated body data
            "AnimationMetrics" // Performance and quality metrics
        };
    }
};

class FullBodyIK : public Animation {
public:
    // === Allocation, Initialization, Loading ===
    explicit FullBodyIK(const FullBodyIKInfo& info = FullBodyIKInfo())
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
    ~FullBodyIK() = default;     // Default destructor
};

} // namespace hd
