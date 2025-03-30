/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GrenadeLauncher.h
 * @brief Header file for the GrenadeLauncher class.
 * 
 * ARCHITECTURAL NOTES:
 * - GrenadeLauncher is a class that represents a grenade launcher in Hydragon.
 * - It is used to represent any grenade launcher in the game world, such as RPGs, grenade launchers, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other grenade launchers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct GrenadeLauncherInfo : public WeaponInfo {
    GrenadeLauncherInfo() {
        nodeType = "Gameplay/GrenadeLauncher";
        
        inputs = {
            "grenadeLauncherType",  // Type of grenade launcher (RPG, grenade launcher, etc.)
            "grenadeLauncherData",  // Grenade launcher data
            "environment",           // Environment data
            "characterData",         // Character data
            "grenadeLauncherState"   // Grenade launcher state
        };
        
        outputs = {
            "grenadeLauncherStatus",  // Grenade launcher status
            "grenadeLauncherMetrics"  // Grenade launcher performance metrics
        };
    }
};

class GrenadeLauncher : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit GrenadeLauncher(const GrenadeLauncherInfo& info = GrenadeLauncherInfo())
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
    ~GrenadeLauncher() = default;     // Default destructor
};

} // namespace hd