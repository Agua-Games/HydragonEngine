/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Camp.h
 * @brief Header file for the Camp class.
 * 
 * ARCHITECTURAL NOTES:
 * - Camp is a class that represents a camp in Hydragon.
 * - It is used to represent any camp in the game world, such as military camps, survival camps, tribal camps, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other camps, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"

namespace hd {
    
struct CampInfo : public BuildingInfo {
    CampInfo() {
        nodeType = "Gameplay/Camp";
        
        inputs = {
            "campType",      // Type of camp (military camp, survival camp, etc.)
            "campData",      // Camp data
            "environment",   // Environment data
            "characterData", // Character data
            "campState"      // Camp state
        };
        
        outputs = {
            "campStatus",    // Camp status
            "campMetrics"    // Camp performance metrics
        };
    }
};

class Camp : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Camp(const CampInfo& info = CampInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
};

}