/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Tent.h
 * @brief Header file for the Tent class.
 * 
 * ARCHITECTURAL NOTES:
 * - Tent is a class that represents a tent in Hydragon.
 * - It is used to represent any tent in the game world, such as camping tents, military tents, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other tents, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"

namespace hd {

struct TentInfo : public BuildingInfo {
    TentInfo() {
        nodeType = "Gameplay/Tent";
        
        inputs = {
            "tentType",      // Type of tent (camping tent, military tent, etc.)
            "tentData",      // Tent data
            "environment",   // Environment data
            "characterData", // Character data
            "tentState"      // Tent state
        };
        
        outputs = {
            "tentStatus",    // Tent status
            "tentMetrics"    // Tent performance metrics
        };
    }
};

class Tent : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Tent(const TentInfo& info = TentInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Tent() = default;     // Default destructor
};

} // namespace hd
