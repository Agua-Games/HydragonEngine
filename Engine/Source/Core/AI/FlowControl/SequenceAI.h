/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SequenceAI.h
 * @brief Header file for the SequenceAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - SequenceAI is a class that represents a behavior AI sequence in Hydragon.
 * - It is used to represent and process AI sequences.
 * - It uses the Vulkan API for sequence AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct SequenceAIInfo : public NodeInfo {
    SequenceAIInfo() {
        nodeType = "AI/SequenceAI";
        
        inputs = {
            "rootNode",       // Root node of the behavior tree
            "environment",    // Environment data
            "proceduralParams" // Procedural parameters
        };
        
        outputs = {
            "behaviorMetrics", // Performance and quality metrics
            "proceduralData"   // Generated procedural data
        };
    }
};

class SequenceAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SequenceAI(const SequenceAIInfo& info = SequenceAIInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)
    
    std::vector<Node> queuedActions;        // Queued actions vector. Uses Node class to accept ConditionAI, ActionAI, etc

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SequenceAI() = default;                // Default destructor
};

} // namespace hd
