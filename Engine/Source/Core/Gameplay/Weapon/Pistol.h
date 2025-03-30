/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Pistol.h
 * @brief Header file for the Pistol class.
 * 
 * ARCHITECTURAL NOTES:
 * - Pistol is a class that represents a pistol in Hydragon.
 * - It is used to represent any pistol in the game world, such as handguns, revolvers, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other pistols, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct PistolInfo : public WeaponInfo {
    PistolInfo() {
        nodeType = "Gameplay/Pistol";
        
        inputs = {
            "pistolType",  // Type of pistol (handgun, revolver, etc.)
            "pistolData",  // Pistol data
            "environment", // Environment data
            "characterData", // Character data
            "pistolState" // Pistol state
        };
        
        outputs = {
            "pistolStatus", // Pistol status
            "pistolMetrics" // Pistol performance metrics
        };
    }
};

class Pistol : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Pistol(const PistolInfo& info = PistolInfo())
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
    ~Pistol() = default;     // Default destructor
};

} // namespace hd