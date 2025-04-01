/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Shuriken.h
 * @brief Header file for the Shuriken class.
 * 
 * ARCHITECTURAL NOTES:
 * - Shuriken is a class that represents a shuriken in Hydragon.
 * - It is used to represent any shuriken in the game world, such as throwing stars, kunai, etc.
 * - It supports interactive features, such as throwing, reloading, and aiming. Also supports two-way messaging with other shurikens, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Projectile.h"
#include "DataTable.h"

namespace hd {

struct ShurikenInfo : public ProjectileInfo {
    ShurikenInfo() {
        nodeType = "Gameplay/Shuriken";
        
        inputs = {
            "shurikenType",  // Type of shuriken (throwing star, kunai, etc.)
            "shurikenData",  // Shuriken data
            "environment", // Environment data
            "characterData", // Character data
            "shurikenState" // Shuriken state
        };
        
        outputs = {
            "shurikenStatus", // Shuriken status
            "shurikenMetrics" // Shuriken performance metrics
        };
    }
};

class Shuriken : public Projectile {
public:
    // === Allocation, Initialization, Loading ===
    explicit Shuriken(const ShurikenInfo& info = ShurikenInfo())
        : Projectile(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processShuriken();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Shuriken() = default;     // Default destructor
};

} // namespace hd
