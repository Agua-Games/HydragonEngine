/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AgentAI.h
 * @brief Header file for the AgentAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - AgentAI is a class that represents an AI agent in Hydragon.
 * - It is used to represent any AI agent in the game world, such as characters, vehicles, etc.
 * - It is AI-enabled, via BehaviorTree.
 * 
 * @todo Change all input and output names to lowercase camelCase.
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <string>
#include "BehaviorTree.h"

namespace hd {

enum class AgentAIStrategy {
    Passive,
    Defensive,
    Aggressive
};

struct AgentAIInfo : public BehaviorTreeInfo {
    AgentAIInfo() {
        NodeType = "AI/AgentAI";
        
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

class AgentAI : public BehaviorTree {
public:
    // === Allocation, Initialization, Loading ===
    explicit AgentAI(const AgentAIInfo& info = AgentAIInfo())
        : BehaviorTree(info) {}    // Default constructor for AgentAI class
    initialize() override {}
    load() override {}

    // Set default values
    AgentAIStrategy strategy = AgentAIStrategy::Passive;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AgentAI() = default;     // Default destructor
};

} // namespace hd
