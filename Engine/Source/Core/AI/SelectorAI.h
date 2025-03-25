/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SelectorAI.h
 * @brief Header file for the SelectorAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - SelectorAI is a class that represents a behavior AI selector in Hydragon.
 * - It is used to represent and process AI selectors.
 * - It uses the Vulkan API for selector AI processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Node.h"
#include "SequenceAI.h"

namespace hd {

struct SelectorAIInfo : public NodeInfo {
    SelectorAIInfo() {
        NodeType = "AI/SelectorAI";
        
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

class SelectorAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SelectorAI(const SelectorAIInfo& info = SelectorAIInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    std::vector<SequenceAI> sequenceAI; // SequenceAI std::vector

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SelectorAI() = default;     // Default destructor
};

} // namespace hd
