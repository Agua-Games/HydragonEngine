/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Grenade.h
 * @brief Header file for the Grenade class.
 * 
 * ARCHITECTURAL NOTES:
 * - Grenade is a class that represents a grenade in Hydragon.
 * - It is used to represent any grenade in the game world, such as fragmentation grenades, smoke grenades, etc.
 * - It supports interactive features, such as throwing, reloading, and aiming. Also supports two-way messaging with other grenades, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Projectile.h"

namespace hd {

struct GrenadeInfo : public ProjectileInfo {
    GrenadeInfo() {
        nodeType = "Gameplay/Grenade";
        
        inputs = {
            "grenadeType",  // Type of grenade (fragmentation, smoke, etc.)
            "grenadeData",  // Grenade data
            "environment", // Environment data
            "characterData", // Character data
            "grenadeState" // Grenade state
        };
        
        outputs = {
            "grenadeStatus", // Grenade status
            "grenadeMetrics" // Grenade performance metrics
        };
    }
};

class Grenade : public Projectile {
    public:
        // === Allocation, Initialization, Loading ===
        explicit Grenade(const GrenadeInfo& info = GrenadeInfo())
            : Projectile(info) {}
        initialize() override {}
        load() override {}

        // Set default values
        GrenadeType type = GrenadeType::custom;
        float kineticPotential = 0.0f;
        float thermalPotential = 0.0f;
        float chemicalPotential = 0.0f;     // Maybe too technical, to be removed later
        float damage = 0.0f;
        float damageRadius = 0.0f;

        // === Processing ===
        void processNode() override { }
        void throw(); // Throw the grenade. This is used for weapons that require a charge time before firing, such as bows and crossbows. Default: no charge.
        void fire();
        void reload();
        void aim();
        void update();

        // === Cleanup ===
        void unload() override {}
        void cleanup() override {}
        ~Grenade() = default;     // Default destructor
};

} // namespace hd

