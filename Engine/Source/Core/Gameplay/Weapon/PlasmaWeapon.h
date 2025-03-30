/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PlasmaWeapon.h
 * @brief Header file for the PlasmaWeapon class.
 * 
 * ARCHITECTURAL NOTES:
 * - PlasmaWeapon is a class that represents a plasma weapon in Hydragon.
 * - It is used to represent any plasma weapon in the game world, such as plasma rifles, plasma cannons, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other plasma weapons, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct PlasmaWeaponInfo : public WeaponInfo {
    PlasmaWeaponInfo() {
        nodeType = "Gameplay/PlasmaWeapon";
        
        inputs = {
            "plasmaWeaponType",  // Type of plasma weapon (rifle, cannon, etc.)
            "plasmaWeaponData",  // Plasma weapon data
            "environment",       // Environment data
            "characterData",     // Character data
            "plasmaWeaponState"  // Plasma weapon state
        };
        
        outputs = {
            "plasmaWeaponStatus",  // Plasma weapon status
            "plasmaWeaponMetrics"  // Plasma weapon performance metrics
        };
    }
};

class PlasmaWeapon : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit PlasmaWeapon(const PlasmaWeaponInfo& info = PlasmaWeaponInfo())
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
    ~PlasmaWeapon() = default;     // Default destructor
};

} // namespace hd
