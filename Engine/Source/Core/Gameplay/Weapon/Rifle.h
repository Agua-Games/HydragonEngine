/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Rifle.h
 * @brief Header file for the Rifle class.
 * 
 * ARCHITECTURAL NOTES:
 * - Rifle is a class that represents a rifle in Hydragon.
 * - It is used to represent any rifle in the game world, such as assault rifles, sniper rifles, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other rifles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct RifleInfo : public WeaponInfo {
    RifleInfo() {
        nodeType = "Gameplay/Rifle";
        
        inputs = {
            "rifleType",  // Type of rifle (assault rifle, sniper rifle, etc.)
            "rifleData",  // Rifle data
            "environment", // Environment data
            "characterData", // Character data
            "rifleState" // Rifle state
        };
        
        outputs = {
            "rifleStatus", // Rifle status
            "rifleMetrics" // Rifle performance metrics
        };
    }
};

class Rifle : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Rifle(const RifleInfo& info = RifleInfo())
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
    ~Rifle() = default;     // Default destructor
};

} // namespace hd