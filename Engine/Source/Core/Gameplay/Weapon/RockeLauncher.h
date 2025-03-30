/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RocketLauncher.h
 * @brief Header file for the RocketLauncher class.
 * 
 * ARCHITECTURAL NOTES:
 * - RocketLauncher is a class that represents a rocket launcher in Hydragon.
 * - It is used to represent any rocket launcher in the game world, such as RPGs, grenade launchers, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other rocket launchers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct RocketLauncherInfo : public WeaponInfo {
    RocketLauncherInfo() {
        nodeType = "Gameplay/RocketLauncher";
        
        inputs = {
            "rocketLauncherType",  // Type of rocket launcher (RPG, grenade launcher, etc.)
            "rocketLauncherData",  // Rocket launcher data
            "environment",         // Environment data
            "characterData",       // Character data
            "rocketLauncherState"  // Rocket launcher state
        };
        
        outputs = {
            "rocketLauncherStatus",  // Rocket launcher status
            "rocketLauncherMetrics"  // Rocket launcher performance metrics
        };
    }
};

class RocketLauncher : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit RocketLauncher(const RocketLauncherInfo& info = RocketLauncherInfo())
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
    ~RocketLauncher() = default;     // Default destructor
};

} // namespace hd