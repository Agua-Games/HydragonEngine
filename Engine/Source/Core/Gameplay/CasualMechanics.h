/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CasualMechanics.h
 * @brief Header file for the CasualMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - CasualMechanics is a class that represents casual mechanics in Hydragon.
 * - It is used to represent any casual mechanics like seen in casual games, like character socialization, etc.
 * - It supports interactive features, such as socializing, interacting, and communicating. Also supports two-way messaging with other casual mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct CasualMechanicsInfo : public NodeInfo {
    CasualMechanicsInfo() {
        nodeType = "Gameplay/CasualMechanics";
        
        inputs = {
            "casualData",        // Casual data
            "environment",       // Environment data
            "characterData",     // Character data
            "casualState"        // Casual state
        };
        
        outputs = {
            "casualStatus",      // Casual status
            "casualMetrics"      // Casual performance metrics
        };
    }
};

class CasualMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    CasualMechanics();
    void initialize();
    void load();

    // === Processing ===
    void processNode() override;
    void update();

    // === Cleanup ===
    void unload() override;
    void cleanup() override;
    ~CasualMechanics() = default;     // Default destructor
};

}