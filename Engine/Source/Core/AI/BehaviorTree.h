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
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct BehaviorTreeInfo : public NodeInfo {
    BehaviorTreeInfo() {
        NodeType = "AI/BehaviorTree";
        
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

class BehaviorTree : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BehaviorTree(const BehaviorTreeInfo& info = BehaviorTreeInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BehaviorTree() = default;     // Default destructor
};

} // namespace hd
