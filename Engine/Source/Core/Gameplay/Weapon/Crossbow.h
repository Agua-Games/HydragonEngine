/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Crossbow.h
 * @brief Header file for the Crossbow class.
 * 
 * ARCHITECTURAL NOTES:
 * - Crossbow is a class that represents a crossbow in Hydragon.
 * - It is used to represent any crossbow in the game world, such as longbows, shortbows, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other crossbows, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct CrossbowInfo : public WeaponInfo {
    CrossbowInfo() {
        nodeType = "Gameplay/Crossbow";
        
        inputs = {
            "crossbowType",  // Type of crossbow (longbow, shortbow, etc.)
            "crossbowData",  // Crossbow data
            "environment",   // Environment data
            "characterData", // Character data
            "crossbowState"  // Crossbow state
        };
        
        outputs = {
            "crossbowStatus",  // Crossbow status
            "crossbowMetrics"  // Crossbow performance metrics
        };
    }
};

class Crossbow : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Crossbow(const CrossbowInfo& info = CrossbowInfo())
        : Weapon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Crossbow() = default;     // Default destructor
};

} // namespace hd
