/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AttackAI.h
 * @brief Header file for the AttackAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - AttackAI is a class that represents a behavior AI action in Hydragon.
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

struct AttackAIInfo : public NodeInfo {
    AttackAIInfo() {
        nodeType = "AI/AttackAI";
        
        inputs = {
            "target",          // Target
            "attackData",      // Attack data
            "environment",     // Environment data
            "characterData",   // Character data
            "proceduralParams" // Procedural parameters
        };
        
        outputs = {
            "status",          // Status of the attack
            "metrics"          // Performance metrics
        };
    }
};

class AttackAI : public Node {
public:
    // === Allocation, Initialization, Loading ===
    AttackAI();
    ~AttackAI();
    
    // === Processing ===
    void processNode();
    void update();
    
    // === Cleanup ===
    void unload();
    void cleanup();
};

} // namespace hd