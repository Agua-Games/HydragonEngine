/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AnimationController.h
 * @brief Header file for the AnimationController class.
 * 
 * ARCHITECTURAL NOTES:
 * - AnimationController is a class that represents an animation controller in Hydragon.
 * - It is used to control, manage, modify, enhance the playback of animations. It's the same as a character rig, with enhanced procedural features.
 * - It offers built-in support for procedural modification of imported animations, and for procedural animation generation.
 * - It uses the Vulkan API for animation processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "Animation.h"
#include "SkeletonDeformer.h"

namespace hd {

struct AnimationControllerInfo : public AnimationInfo {
    AnimationControllerInfo() {
        NodeType = "Animation/AnimationController";
        
        inputs = {
            "AnimationClip",     // Animation clip data
            "Skeleton",          // Skeleton data 
            "AnimationSpeed",    // Speed of the animation
            "AnimationLoop",     // Whether the animation should loop
            "Controls",          // Control parameters/interfaces
            "Links",             // Links between animation, skeleton data (e.g. IK constraints), also arbitrary data - all with support for procedural modulation
            "Influences",        // External influence factors
            "Parameters",        // Animation parameters
            "Targets"            // Target poses/positions
        };
        
        outputs = {
            "AnimatedSkeleton", // Animated skeleton data
            "ControlState",     // Current control state
            "PoseModification", // Modified pose data
            "Feedback",         // Control feedback data
            "Diagnostics"       // Debug/monitoring data
        };
    }
};

class AnimationController : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit AnimationController(const AnimationControllerInfo& info = AnimationControllerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    // Control interfaces
    void setControlParameter(const std::string& name, float value);
    void enableControl(const std::string& name, bool enabled);
    void updateLinks();                     // Links are the same as constraints

    // Manipulation
    void applyPoseModification();
    void processInfluences();

    // Feedback
    float getControlValue(const std::string& name) const;
    bool isControlActive(const std::string& name) const;
    
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AnimationController() = default;     // Default destructor
};

} // namespace hd
