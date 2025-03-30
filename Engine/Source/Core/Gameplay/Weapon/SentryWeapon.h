/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SentryWeapon.h
 * @brief Header file for the SentryWeapon class.
 * 
 * ARCHITECTURAL NOTES:
 * - SentryWeapon is a class that represents a sentry weapon in Hydragon.
 * - It is used to represent any sentry weapon in the game world, such as turrets, sentry guns, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other sentry weapons, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct SentryWeaponInfo : public WeaponInfo {
    SentryWeaponInfo() {
        nodeType = "Gameplay/SentryWeapon";
        
        inputs = {
            "sentryWeaponType",  // Type of sentry weapon (turret, sentry gun, etc.)
            "sentryWeaponData",  // Sentry weapon data
            "environment",       // Environment data
            "characterData",     // Character data
            "sentryWeaponState"  // Sentry weapon state
        };
        
        outputs = {
            "sentryWeaponStatus",  // Sentry weapon status
            "sentryWeaponMetrics"  // Sentry weapon performance metrics
        };
    }
};

class SentryWeapon : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit SentryWeapon(const SentryWeaponInfo& info = SentryWeaponInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SentryWeapon() = default;     // Default destructor
};

} // namespace hd
