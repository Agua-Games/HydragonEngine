/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ConditionAI.h
 * @brief Header file for the ConditionAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - ConditionAI is a class that represents a AI condition in Hydragon.
 * - It is used to represent and process AI conditions.
 * - It uses the Vulkan API for condition AI processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct ConditionAIInfo : public NodeInfo {
    ConditionAIInfo() {
        NodeType = "AI/ConditionAI";
        
        inputs = {
            "Condition",       // Condition data
            "Environment",     // Environment data
            "ProceduralParams" // Procedural parameters
        };
        
        outputs = {
            "ConditionResult", // Result of the condition
            "BehaviorMetrics", // Performance and quality metrics
            "ProceduralData"   // Generated procedural data
        };
    }
};

class ConditionAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ConditionAI(const ConditionAIInfo& info = ConditionAIInfo())
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
    ~ConditionAI() = default;     // Default destructor
};

} // namespace hd
