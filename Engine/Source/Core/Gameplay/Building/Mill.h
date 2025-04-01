/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Mill.h
 * @brief Header file for the Mill class.
 * 
 * ARCHITECTURAL NOTES:
 * - Mill is a class that represents a mill in Hydragon.
 * - It is used to represent any mill in the game world, such as windmills, watermills, etc.
 * - It supports interactive features, such as grinding, milling, and managing crops. Also supports two-way messaging with other mills, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"
#include "Item.h"

namespace hd {

struct MillInfo : public BuildingInfo {
    MillInfo() {
        nodeType = "Gameplay/Mill";
        
        inputs = {
            "millType",      // Type of mill (windmill, watermill, etc.)
            "millData",      // Mill data
            "environment",   // Environment data
            "characterData", // Character data
            "millState"      // Mill state
        };
        
        outputs = {
            "millStatus",    // Mill status
            "millMetrics"    // Mill performance metrics
        };
    }
};

class Mill : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Mill(const MillInfo& info = MillInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processMill();
    void processNode() override {
        processMill(); 
    }
    void grind();
    void mill();
    void manageCrops();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Mill() = default;     // Default destructor
};

} // namespace hd
