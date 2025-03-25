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
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct SequenceAIInfo : public NodeInfo {
    SequenceAIInfo() {
        NodeType = "AI/SequenceAI";
        
        inputs = {
            "RootNode",       // Root node of the behavior tree
            "Environment",    // Environment data
            "ProceduralParams" // Procedural parameters
        };
        
        outputs = {
            "BehaviorMetrics", // Performance and quality metrics
            "ProceduralData"   // Generated procedural data
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

    std::vector<Node> queuedActions;        // Queued actions std::vector. Uses Node class to accept ConditionAI, ActionAI, etc

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SequenceAI() = default;                // Default destructor
};

} // namespace hd
