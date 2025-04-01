/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Lasso.h
 * @brief Header file for the Lasso class.
 * 
 * ARCHITECTURAL NOTES:
 * - Lasso is a class that represents a lasso in Hydragon.
 * - It is used to represent any lasso in the game world, such as lassos, ropes, etc.
 * - It supports interactive features, such as throwing, reloading, and aiming. Also supports two-way messaging with other lassos, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct LassoInfo : public WeaponInfo {
    LassoInfo() {
        nodeType = "Gameplay/Lasso";
        
        inputs = {
            "lassoType",  // Type of lasso (lasso, rope, etc.)
            "lassoData",  // Lasso data
            "environment", // Environment data
            "characterData", // Character data
            "lassoState" // Lasso state
        };
        
        outputs = {
            "lassoStatus", // Lasso status
            "lassoMetrics" // Lasso performance metrics
        };
    }
};

class Lasso : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Lasso(const LassoInfo& info = LassoInfo())
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
    ~Lasso() = default;     // Default destructor
};

} // namespace hd
