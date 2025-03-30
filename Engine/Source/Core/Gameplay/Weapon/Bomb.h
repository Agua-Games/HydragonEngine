/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Bomb.h
 * @brief Header file for the Bomb class.
 * 
 * ARCHITECTURAL NOTES:
 * - Bomb is a class that represents a bomb in Hydragon.
 * - It is used to represent any bomb in the game world, such as grenades, mines, etc.
 * - It supports interactive features, such as throwing, reloading, and aiming. Also supports two-way messaging with other bombs, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct BombInfo : public WeaponInfo {
    BombInfo() {
        nodeType = "Gameplay/Bomb"; 
        
        inputs = {
            "bombType",  // Type of bomb (grenade, mine, etc.)
            "bombData",  // Bomb data
            "environment", // Environment data
            "characterData", // Character data
            "bombState" // Bomb state
        };
        
        outputs = {
            "bombStatus", // Bomb status
            "bombMetrics" // Bomb performance metrics
        };
    }
};

class Bomb : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Bomb(const BombInfo& info = BombInfo())
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
    ~Bomb() = default;     // Default destructor
};

} // namespace hd