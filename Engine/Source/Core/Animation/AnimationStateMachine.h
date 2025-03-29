/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AnimationStateMachine.h
 * @brief Header file for the AnimationStateMachine class.
 * 
 * ARCHITECTURAL NOTES:
 * - AnimationStateMachine is a class that represents an animation state machine in Hydragon.
 * - It inherits from the more general StateMachine class.
 * - It is used to represent and process animation state machines.
 * - It uses the Vulkan API for animation state machine processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include "StateMachine.h"
#include "Animation.h"
#include <vulkan/vulkan.h>

namespace hd {

struct AnimationStateMachineInfo : public StateMachineInfo {
    AnimationStateMachineInfo() {
        nodeType = "Animation/AnimationStateMachine";
        
        inputs = {
            "CurrentState",  // Current state of the animation state machine
            "Transition",    // Transition event
            "Parameters"     // Parameters for the transition
        };
        
        outputs = {
            "NextState",     // Next state of the animation state machine
            "TransitionData" // Data associated with the transition
        };
    }
};

class AnimationStateMachine : public StateMachine {
public:
    // === Allocation, Initialization, Loading ===
    explicit AnimationStateMachine(const AnimationStateMachineInfo& info = AnimationStateMachineInfo())
        : StateMachine(info) {}
    initialize() override {}
    load() override {}

    // Set default values

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AnimationStateMachine() = default;     // Default destructor
};

} // namespace hd

