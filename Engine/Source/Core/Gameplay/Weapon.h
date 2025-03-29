/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Weapon.h
 * @brief Header file for the Weapon class.
 * 
 * ARCHITECTURAL NOTES:
 * - Weapon is a class that represents a weapon in Hydragon.
 * - It is used to represent any weapon in the game world, such as guns, swords, bows, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other weapons, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Prop.h"

namespace hd {

struct WeaponInfo : public PropInfo {
    WeaponInfo() {
        nodeType = "Gameplay/Weapon";
        inputs = {
            "target",       // Target of the weapon
            "range",        // Range of the weapon
            "spread",       // Spread cone of the weapon
            "accuracy",     // Accuracy of the weapon
            "damage",       // Damage of the weapon
            "armorPiercing", // Armor piercing of the weapon
            "penetration",  // Penetration of the weapon
            "aimTime",      // Aim time of the weapon
            "fireRate",     // Fire rate of the weapon
            "reloadRate",   // Reload rate of the weapon
            "drop",         // Ammo drop of the weapon
            "dropTime",     // Ammo drop time of the weapon
            "dropType",     // Ammo drop type of the weapon
            "dropPosition", // Ammo drop position of the weapon
            "dropVelocity", // Ammo drop velocity of the weapon
            "ammo",         // Ammo of the weapon
            "ammoType",     // Ammo type of the weapon
            "ammoCount",    // Ammo count of the weapon
            "ammoCapacity", // Ammo capacity of the weapon
            "ammoDropTime", // Ammo drop time of the weapon
            "ammoDropRate", // Ammo drop rate of the weapon
            "ammoDropChance",           // Ammo drop chance of the weapon
            "ammoDropType",             // Ammo drop type of the weapon
            "ammoDropPosition",         // Ammo drop position of the weapon
            "ammoDropVelocity",         // Ammo drop velocity of the weapon
        };
        outputs = {
            "target",       // Target of the weapon
            "range",        // Range of the weapon
            "spread",       // Spread cone of the weapon
            "accuracy",     // Accuracy of the weapon
            "damage",       // Damage of the weapon
            "armorPiercing", // Armor piercing of the weapon            
            "penetration",  // Penetration of the weapon
            "aimTime",      // Aim time of the weapon
            "fireRate",     // Fire rate of the weapon
            "reloadRate",   // Reload rate of the weapon
            "drop",         // Ammo drop of the weapon
            "dropTime",     // Ammo drop time of the weapon
            "dropType",     // Ammo drop type of the weapon
            "dropPosition", // Ammo drop position of the weapon
            "dropVelocity", // Ammo drop velocity of the weapon
            "ammo",         // Ammo of the weapon
            "ammoType",     // Ammo type of the weapon
            "ammoCount",    // Ammo count of the weapon
            "ammoCapacity", // Ammo capacity of the weapon
            "ammoDropTime", // Ammo drop time of the weapon
            "ammoDropRate", // Ammo drop rate of the weapon
            "ammoDropChance",           // Ammo drop chance of the weapon
            "ammoDropType",             // Ammo drop type of the weapon
            "ammoDropPosition",         // Ammo drop position of the weapon
            "ammoDropVelocity",         // Ammo drop velocity of the weapon
            "weaponStatus", // Weapon status
            "weaponMetrics" // Weapon performance metrics
        };
    }
};

class Weapon : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Weapon(const WeaponInfo& info = WeaponInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values for weapon info struct
    WeaponInfo weaponInfo = {};  // Initialize weapon info struct with default values

    // === Processing ===
    void processNode() override {
        
    }
    void aim();
    void reload();
    void fire();
    void updateAmmo();
    void cooldown();
    void overheat();
    void equip(bool equip);         // to equip or unequip the weapon to the character, vehicle, building
    void equipWeapon(bool equip);   // to add equipment to the weapon
    void dropWeapon(bool drop);
    void dropAmmo(bool drop);
    void processWeapon();
    void update() override { // Override the update() function to call processWeapon() instead of update().
        processWeapon(); // Call the processWeapon() function to update the weapon state.
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Weapon() = default;     // Default destructor
};


} // namespace hd
