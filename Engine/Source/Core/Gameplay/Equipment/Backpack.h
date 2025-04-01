/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Backpack.h
 * @brief Header file for the Backpack class.
 * 
 * ARCHITECTURAL NOTES:
 * - Backpack is a class that represents a backpack in Hydragon.
 * - It is used to represent any backpack in the game world, such as backpacks, vests, etc.
 * - It supports interactive features, such as equipping, unequipping, and upgrading. Also supports two-way messaging with other backpacks, environment, character, etc.
 */
#pragma once
#include "Equipment.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct BackpackInfo : public NodeInfo {
    BackpackInfo() {
        nodeType = "Gameplay/Backpack";
        
        inputs = {
            "backpackType",  // Type of backpack (backpack, vest, etc.)
            "backpackData",  // Backpack data
            "environment",   // Environment data
            "characterData", // Character data
            "backpackState"  // Backpack state
        };
        
        outputs = {
            "backpackStatus",  // Backpack status
            "backpackMetrics"  // Backpack performance metrics
        };
    }
};

class Backpack : public Equipment {
public:
    // === Allocation, Initialization, Loading ===
    explicit Backpack(const BackpackInfo& info = BackpackInfo())
        : Equipment(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processBackpack();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Backpack() = default;     // Default destructor
};

} // namespace hd
