/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Ladder.h
 * @brief Header file for the Ladder class.
 * 
 * ARCHITECTURAL NOTES:
 * - Ladder is a class that represents a ladder in Hydragon.
 * - It is used to represent any ladder in the game world, such as building ladders, bridge ladders, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other ladders, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {
    
struct LadderInfo : public BuildingElementInfo {
    LadderInfo() {
        nodeType = "Gameplay/Ladder";
        
        inputs = {
            "ladderType",  // Type of ladder (building ladder, bridge ladder, etc.)
            "ladderData",  // Ladder data
            "environment", // Environment data
            "characterData", // Character data
            "ladderState" // Ladder state
        };
        
        outputs = {
            "ladderStatus", // Ladder status
            "ladderMetrics" // Ladder performance metrics
        };
    }
};

class Ladder : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Ladder(const LadderInfo& info = LadderInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
    void interact();     // Interact with the ladder (e.g., climb up, climb down, etc.)
};

} // namespace hd
