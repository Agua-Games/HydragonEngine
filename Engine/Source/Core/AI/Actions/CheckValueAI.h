/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CheckValueAI.h
 * @brief Header file for the CheckValueAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - CheckValueAI is a class that represents a behavior AI action in Hydragon.
 * - It is used to represent and process AI actions.
 * - It uses the Vulkan API for action AI processing.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */
#pragma once
#include <vulkan/vulkan.h>  // If not used, remove
#include <vector>           // If not used, remove
#include "Node.h"
#include "DataTable.h"      // If not used, remove
#include "glm/glm.hpp"      // If not used, remove

namespace hd {

struct CheckValueAIInfo : public NodeInfo {
    CheckValueAIInfo() {
        nodeType = "AI/CheckValueAI";
        
        inputs = {
            "value",            // Value to be checked
            "operator",         // Operator to be used for comparison
            "targetValue",      // Target value for comparison
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "status",           // Status of the check
            "metrics"           // Performance metrics
        };
    }
};

class CheckValueAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CheckValueAI(const CheckValueAIInfo& info = CheckValueAIInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable value;         // Value to be checked. Default: empty object.
    DataTable operator;      // Operator to be used for comparison. Default: empty object.
    DataTable targetValue;   // Target value for comparison. Default: empty object.
    DataTable environment;   // Environment data. Default: empty object.
    DataTable proceduralParams; // Procedural parameters. Default: empty object.

    // === Processing ===   
    void processNode() override {
        // Process the check AI action.
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CheckValueAI() = default;     // Default destructor
};

} // namespace hd
