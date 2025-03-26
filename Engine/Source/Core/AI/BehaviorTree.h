/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BehaviorTree.h
 * @brief Header file for the BehaviorTree class.
 * 
 * ARCHITECTURAL NOTES:
 * - BehaviorTree is a class that represents a behavior tree in Hydragon.
 * - It is used to represent and process behavior trees.
 * - It uses the Vulkan API for behavior tree processing (Compute, parallel multi-threaded tree traversal, etc).
 *
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct BehaviorTreeInfo : public NodeInfo {
    BehaviorTreeInfo() {
        nodeType = "AI/BehaviorTree";
        
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

class BehaviorTree : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BehaviorTree(const BehaviorTreeInfo& info = BehaviorTreeInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BehaviorTree() = default;     // Default destructor
};

} // namespace hd
