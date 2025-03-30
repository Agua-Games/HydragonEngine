/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Spear.h
 * @brief Header file for the Spear class.
 * 
 * ARCHITECTURAL NOTES:
 * - Spear is a class that represents a spear in Hydragon.
 * - It is used to represent any spear in the game world, such as javelins, spears, etc.
 * - It supports interactive features, such as throwing, reloading, and aiming. Also supports two-way messaging with other spears, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct SpearInfo : public WeaponInfo {
    SpearInfo() {
        nodeType = "Gameplay/Spear";
        
        inputs = {
            "spearType",  // Type of spear (javelin, spear, etc.)
            "spearData",  // Spear data
            "environment", // Environment data
            "characterData", // Character data
            "spearState" // Spear state
        };
        
        outputs = {
            "spearStatus", // Spear status
            "spearMetrics" // Spear performance metrics
        };
    }
};

class Spear : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Spear(const SpearInfo& info = SpearInfo())
        : Weapon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void throw();
    void reload();
    void aim();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Spear() = default;     // Default destructor
};

} // namespace hd