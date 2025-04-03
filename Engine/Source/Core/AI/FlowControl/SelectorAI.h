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
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "Node.h"
#include "SequenceAI.h"

namespace hd {

struct SelectorAIInfo : public NodeInfo {
    SelectorAIInfo() {
        nodeType = "AI/SelectorAI";
        
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

class SelectorAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SelectorAI(const SelectorAIInfo& info = SelectorAIInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    // (...)
    
    std::vector<SequenceAI> sequenceAI; // SequenceAI vector

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SelectorAI() = default;     // Default destructor
};

} // namespace hd
