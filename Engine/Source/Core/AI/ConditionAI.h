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
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct ConditionAIInfo : public NodeInfo {
    ConditionAIInfo() {
        nodeType = "AI/ConditionAI";
        
        inputs = {
            "condition",       // Condition data
            "environment",     // Environment data
            "proceduralParams" // Procedural parameters
        };
        
        outputs = {
            "conditionResult", // Result of the condition
            "behaviorMetrics", // Performance and quality metrics
            "proceduralData"   // Generated procedural data
        };
    }
};

class ConditionAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ConditionAI(const ConditionAIInfo& info = ConditionAIInfo())
        : Node(info) {}
    
    float range;
    float fov;
    
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ConditionAI() = default;     // Default destructor
};

} // namespace hd
