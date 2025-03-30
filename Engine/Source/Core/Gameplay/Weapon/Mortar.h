/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Mortar.h
 * @brief Header file for the Mortar class.
 * 
 * ARCHITECTURAL NOTES:
 * - Mortar is a class that represents a mortar in Hydragon.
 * - It is used to represent any mortar in the game world, such as mortars, howitzers, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other mortars, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct MortarInfo : public WeaponInfo {
    MortarInfo() {
        nodeType = "Gameplay/Mortar";
        
        inputs = {
            "mortarType",  // Type of mortar (mortar, howitzer, etc.)
            "mortarData",  // Mortar data
            "environment", // Environment data
            "characterData", // Character data
            "mortarState" // Mortar state
        };
        
        outputs = {
            "mortarStatus", // Mortar status
            "mortarMetrics" // Mortar performance metrics
        };
    }
};

class Mortar : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Mortar(const MortarInfo& info = MortarInfo())
        : Weapon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void fire();
    void reload();
    void aim();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Mortar() = default;     // Default destructor
};

} // namespace hd