/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ControllerAI.h
 * @brief Header file for the ControllerAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - ControllerAI is a class that represents an AI controller in Hydragon.
 * - It is used to control, manage, modify, enhance the behavior of AI agents. It's the same as a character rig, with enhanced procedural features.
 * - It offers built-in support for procedural features, such as pathfinding, decision making, etc. Also procedural emergent behaviors, self-modification, etc.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct ControllerAIInfo : public BehaviorTreeInfo {
    ControllerAIInfo() {
        nodeType = "AI/ControllerAI";
        
        inputs = {
            "agent",           // AI agent data
            "behaviorTree",    // Behavior tree data
            "environment",     // Environment data
            "proceduralParams" // Procedural parameters
        };
        
        outputs = {
            "agentState",      // AI agent state
            "behaviorMetrics", // Behavior tree performance metrics
            "proceduralData"   // Generated procedural data
        };
    }
};

class ControllerAI : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit ControllerAI(const ControllerAIInfo& info = ControllerAIInfo())
        : BehaviorTree(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processControllerAI();
    void () override {
        processControllerAI(); 
    }    
    void update() override { // Override the update() function to call processControllerAI() instead of update().
        processControllerAI(); // Call the processControllerAI() function to update the AI controller state.
    }
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ControllerAI() = default;     // Default destructor
};

} // namespace hd
