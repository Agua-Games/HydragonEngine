/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Amphitheater.h
 * @brief Header file for the Amphitheater class.
 * 
 * ARCHITECTURAL NOTES:
 * - Amphitheater is a class that represents an amphitheater in Hydragon.
 * - It is used to represent any amphitheater in the game world, such as amphitheaters, stadiums, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other amphitheaters, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"

namespace hd {

struct AmphitheaterInfo : public BuildingInfo {
    AmphitheaterInfo() {
        nodeType = "Gameplay/Amphitheater";
        
        inputs = {
            "amphitheaterType",  // Type of amphitheater (amphitheater, stadium, etc.)
            "amphitheaterData",  // Amphitheater data
            "environment",       // Environment data
            "characterData",     // Character data
            "amphitheaterState"  // Amphitheater state
        };
        
        outputs = {
            "amphitheaterStatus",  // Amphitheater status
            "amphitheaterMetrics"  // Amphitheater performance metrics
        };
    }
};

class Amphitheater : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Amphitheater(const AmphitheaterInfo& info = AmphitheaterInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    
    }
    void update();
    
    void enter();
    void exit();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Amphitheater() = default;     // Default destructor
};

} // namespace hd
