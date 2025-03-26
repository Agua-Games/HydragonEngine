/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AnimationProcessor.h
 * @brief Header file for the AnimationProcessor class.
 * 
 * ARCHITECTURAL NOTES:
 * - AnimationProcessor is a class that represents an animation processor in Hydragon.
 * - It is used to process animations and their associated data, such as skeleton animations, curve IK, full body IK, single chain IK, etc.
 * - It offers built-in support for procedural modification of imported animations, and for procedural animation generation.
 * - It uses the Vulkan API for animation processing.
 * 
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"
#include "SkeletonDeformer.h"

namespace hd {

struct AnimationProcessorInfo : public AnimationInfo {
    AnimationProcessorInfo() {
        nodeType = "Animation/AnimationProcessor";
        
        inputs = {
            "animationClip",   // Animation clip data;;
            "time",            // Current time
            "playbackSpeed",   // Playback speed
            "looping"          // Whether to loop the animation
        };
        
        outputs = {
            "processedAnimation", // Processed animation data
            "animationMetrics"   // Performance and quality metrics
        };
    }
};

class AnimationProcessor : public Animation {
public:
    // === Allocation, Initialization, Loading ===
    explicit AnimationProcessor(const AnimationProcessorInfo& info = AnimationProcessorInfo())
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
    ~AnimationProcessor() = default;     // Default destructor
};

} // namespace hd
