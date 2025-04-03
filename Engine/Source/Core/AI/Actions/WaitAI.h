/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WaitAI.h
 * @brief Header file for the WaitAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - WaitAI is a class that represents a behavior AI action in Hydragon.
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

struct WaitAIInfo : public NodeInfo {
    WaitAIInfo() {
        nodeType = "AI/WaitAI";
        
        inputs = {
            "duration",         // Duration of the wait
            "environment",      // Environment data
            "proceduralParams"  // Procedural parameters
        };
        
        outputs = {
            "status",           // Status of the wait
            "metrics"           // Performance metrics
        };
    }
};

class WaitAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit WaitAI(const WaitAIInfo& info = WaitAIInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float duration;
    DataTable environment;
    DataTable proceduralParams;

    // === Processing ===   
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WaitAI() = default;     // Default destructor
};

} // namespace hd
