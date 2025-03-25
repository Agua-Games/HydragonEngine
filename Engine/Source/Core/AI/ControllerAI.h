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
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "BehaviorTree.h"

namespace hd {

struct ControllerAIInfo : public BehaviorTreeInfo {
    ControllerAIInfo() {
        NodeType = "AI/ControllerAI";
        
        inputs = {
            "Agent",           // AI agent data
            "BehaviorTree",    // Behavior tree data
            "Environment",     // Environment data
            "ProceduralParams" // Procedural parameters
        };
        
        outputs = {
            "AgentState",      // AI agent state
            "BehaviorMetrics", // Behavior tree performance metrics
            "ProceduralData"   // Generated procedural data
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

    // === Processing ===
    void processControllerAI();
    void processNodeGraph() override {
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
