/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Farm.h
 * @brief Header file for the Farm class.
 * 
 * ARCHITECTURAL NOTES:
 * - Farm is a class that represents a farm in Hydragon.
 * - It is used to represent any farm in the game world, such as farms, orchards, etc. It leverages other nodes as elements to compose the farm (Building, Vegetation,
 * Mill, Barn, Plantation, Pasture, etc).
 * - You can also setup a farm using a code-node graph in fluent style, if you want a more extensible and simple way.
 * - It supports interactive features, such as planting, harvesting, and managing crops. Also supports two-way messaging with other farms, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vegetation.h"
#include "Building.h"
#include "Plantation.h"
#include "Pasture.h"
#include "EnvironmentManager.h"

namespace hd {
    
struct FarmInfo : public NodeInfo {
    FarmInfo() {
        nodeType = "Gameplay/Farm";
        
        inputs = {
            "farmType",        // Type of farm (farm, orchard, etc.)
            "farmData",        // Farm data
            "environment",     // Environment data
            "characterData",   // Character data
            "farmState"        // Farm state
        };
        
        outputs = {
            "farmStatus",      // Farm status
            "farmMetrics"      // Farm performance metrics
        };
    }
};

class Farm : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Farm(const FarmInfo& info = FarmInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processFarm();
    void processNode() override {
        processFarm(); 
    }
    void plant();
    void harvest();
    void manage();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Farm() = default;     // Default destructor
};

} // namespace hd
