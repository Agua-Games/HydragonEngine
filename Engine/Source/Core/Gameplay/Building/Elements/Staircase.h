/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Staircase.h
 * @brief Header file for the Staircase class.
 * 
 * ARCHITECTURAL NOTES:
 * - Staircase is a class that represents a staircase in Hydragon.
 * - It is used to represent any staircase in the game world, such as building staircases, bridge staircases, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other staircases, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {
    
struct StaircaseInfo : public BuildingElementInfo {
    StaircaseInfo() {
        nodeType = "Gameplay/Staircase";
        
        inputs = {
            "staircaseType",  // Type of staircase (building staircase, bridge staircase, etc.)
            "staircaseData",  // Staircase data
            "environment",    // Environment data
            "characterData",  // Character data
            "staircaseState"  // Staircase state
        };
        
        outputs = {
            "staircaseStatus",  // Staircase status
            "staircaseMetrics"  // Staircase performance metrics
        };
    }
};

class Staircase : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Staircase(const StaircaseInfo& info = StaircaseInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
    void interact();     // Interact with the staircase (e.g., climb up, climb down, etc.)
};

} // namespace hd
