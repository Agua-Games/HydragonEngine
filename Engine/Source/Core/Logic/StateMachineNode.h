/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file StateMachineNode.h
 * @brief StateMachineNode represents a state machine node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - State machine nodes are used to represent and process state machines.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once
#include "Node.h"
#include "Object.h"

namespace hd {

struct StateMachineNodeInfo : public NodeInfo {
    StateMachineNodeInfo() {
        NodeType = "StateMachineNode";
        
        inputs = {
            "CurrentState",  // Current state of the state machine
            "Transition",    // Transition event
            "Parameters"     // Parameters for the transition
        };
        
        outputs = {
            "NextState",     // Next state of the state machine
            "TransitionData" // Data associated with the transition
        };
    }
};

class StateMachineNode : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit StateMachineNode(const StateMachineNodeInfo& info = StateMachineNodeInfo())
        : Node(info) {}
    void initialize() override {}
    void load() override {}

    // === Processing ===
    void stream() override {}
    void process() override {}
    void update() override {}
    void processNodeGraph() override {}

    // === Cleanup ===
    void cleanup() override {}
};

} // namespace hd