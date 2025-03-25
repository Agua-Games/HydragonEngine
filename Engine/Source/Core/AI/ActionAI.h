/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ActionAI.h
 * @brief Header file for the ActionAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - ActionAI is a class that represents a behavior AI action in Hydragon.
 * - It is used to represent and process AI actions.
 * - It uses the Vulkan API for action AI processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct ActionAIInfo : public NodeInfo {
    ActionAIInfo() {
        NodeType = "AI/ActionAI";
        
        inputs = {
            "Action",          // Action data
            "Environment",     // Environment data
            "ProceduralParams" // Procedural parameters
        };
        
        outputs = {
            "ActionResult",    // Result of the action
            "BehaviorMetrics", // Performance and quality metrics
            "ProceduralData"   // Generated procedural data
        };
    }
};

class ActionAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ActionAI(const ActionAIInfo& info = ActionAIInfo())
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
    ~ActionAI() = default;     // Default destructor
};

} // namespace hd
