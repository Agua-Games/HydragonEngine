/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FleeAI.h
 * @brief Header file for the FleeAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - FleeAI is a class that represents a behavior AI action in Hydragon.
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

struct FleeAIInfo : public NodeInfo {
    FleeAIInfo() {
        nodeType = "AI/FleeAI";
        
        inputs = {
            "target",           // Target data
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "status",           // Status of the flee
            "metrics"           // Performance metrics
        };
    }
};

class FleeAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit FleeAI(const FleeAIInfo& info = FleeAIInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable target;         // Target data for fleeing. Default: empty object.
    DataTable environment;    // Environment data. Default: empty object.
    DataTable proceduralParams; // Procedural parameters. Default: empty object.

    // === Processing ===   
    void processNode() override {
        // Process the flee AI action.
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FleeAI() = default;     // Default destructor
};

} // namespace hd
