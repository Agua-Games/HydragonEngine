/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file StateMachine.h
 * @brief StateMachine represents a state machine node in the engine's node graph.
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
#include <unordered_map>

namespace hd {

struct StateMachineInfo : public NodeInfo {
    StateMachineInfo() {
        NodeType = "StateMachine";
        
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

class StateMachine : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit StateMachine(const StateMachineInfo& info = StateMachineInfo())
        : Node(info) {}
    void initialize() override {}
    void load() override {}

    std::unordered_map<std::string, std::string> states;    // Map of states and their corresponding animation paths
    std::string currentState;                               // Current state of the state machine
    std::string nextState;                                  // Next state of the state machine

    // === Processing ===
    void addState(const std::string& name, const std::string& animationPath); // Add an animation state to the state machine
    void addTransition(const std::string& fromState, 
        const std::string& toState, 
        const std::string& condition, 
        float crossfadeDuration); // Add a transition between two states
    void loadAsync() override {}
    void stream() override {}
    void processNode() override {}
    void update() override {}
    void () override {}

    // === Cleanup ===
    void unload() override {} 
    void cleanup() override {}
    ~StateMachine() = default;     // Default destructor
};

} // namespace hd