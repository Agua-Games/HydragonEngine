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
    // TODO: Organize better the struct, use nested structs for categories. Maybe get rid of some too specific properties.
    WeaponInfo() {
        NodeType = "Gameplay/Weapon";
        inputs = {
            "Ammo",         // Ammo of the weapon
            "AmmoType",     // Ammo type of the weapon
            "AmmoCount",    // Ammo count of the weapon
            "AmmoCapacity", // Ammo capacity of the weapon
            "AmmoAimTime",  // Ammo aim time of the weapon
            "AmmoFire",     // Ammo fire of the weapon
            "AmmoFireRate", // Ammo fire rate of the weapon
            "AmmoReload",   // Ammo reload of the weapon
            "AmmoReloadTime", // Ammo reload time of the weapon
            "AmmoAim",      // Ammo aim of the weapon
            "AmmoAccuracy", // Ammo accuracy of the weapon
            "AmmoDamage",   // Ammo damage of the weapon
            "AmmoSpeed",    // Ammo speed of the weapon
            "AmmoRange",    // Ammo range of the weapon
            "AmmoSpread",   // Ammo spread of the weapon
            "AmmoPenetration", // Ammo penetration of the weapon
            "AmmoDrop",     // Ammo drop of the weapon
            "AmmoDropTime", // Ammo drop time of the weapon
            "AmmoDropRate", // Ammo drop rate of the weapon
            "AmmoDropChance", // Ammo drop chance of the weapon
            "AmmoDropAmount", // Ammo drop amount of the weapon
            "AmmoDropType", // Ammo drop type of the weapon
            "AmmoDropPosition", // Ammo drop position of the weapon
            "AmmoDropVelocity", // Ammo drop velocity of the weapon
            "AmmoDropAngularVelocity", // Ammo drop angular velocity of the weapon
            "AmmoDropRotation", // Ammo drop rotation of the weapon
            "AmmoDropScale", // Ammo drop scale of the weapon
            "AmmoDropMaterial", // Ammo drop material of the weapon
            "AmmoDropAnimation", // Ammo drop animation of the weapon
            "AmmoDropPhysics", // Ammo drop physics of the weapon
            "AmmoDropCollision", // Ammo drop collision of the weapon
            "AmmoDropScript", // Ammo drop script of the weapon
        };
        outputs = {
            "Ammo",         // Ammo of the weapon
            "AmmoType",     // Ammo type of the weapon
            "AmmoCount",    // Ammo count of the weapon
            "AmmoCapacity", // Ammo capacity of the weapon
            "AmmoAimTime",  // Ammo aim time of the weapon
            "AmmoFire",     // Ammo fire of the weapon
            "AmmoFireRate", // Ammo fire rate of the weapon
            "AmmoReload",   // Ammo reload of the weapon
            "AmmoReloadTime", // Ammo reload time of the weapon
            "AmmoAim",      // Ammo aim of the weapon
            "AmmoAccuracy", // Ammo accuracy of the weapon
            "AmmoDamage",   // Ammo damage of the weapon
            "AmmoSpeed",    // Ammo speed of the weapon
            "AmmoRange",    // Ammo range of the weapon
            "AmmoSpread",   // Ammo spread of the weapon
            "AmmoPenetration", // Ammo penetration of the weapon
            "AmmoDrop",     // Ammo drop of the weapon
            "AmmoDropTime", // Ammo drop time of the weapon
            "AmmoDropRate", // Ammo drop rate of the weapon
            "AmmoDropChance", // Ammo drop chance of the weapon
            "AmmoDropAmount", // Ammo drop amount of the weapon
            "AmmoDropType", // Ammo drop type of the weapon
            "AmmoDropPosition", // Ammo drop position of the weapon
            "AmmoDropVelocity", // Ammo drop velocity of the weapon
            "AmmoDropAngularVelocity", // Ammo drop angular velocity of the weapon
            "AmmoDropRotation", // Ammo drop rotation of the weapon
            "AmmoDropScale", // Ammo drop scale of the weapon
            "AmmoDropMaterial", // Ammo drop material of the weapon
            "AmmoDropAnimation", // Ammo drop animation of the weapon
            "AmmoDropPhysics", // Ammo drop physics of the weapon
            "AmmoDropCollision", // Ammo drop collision of the weapon
            "AmmoDropScript", // Ammo drop script of the weapon
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

    // === Processing ===
    void reload();
    void fire();
    void processWeapon();
    void processNodeGraph() override {
        processWeapon(); 
    }    
    void update() override { // Override the update() function to call processWeapon() instead of update().
        processWeapon(); // Call the processWeapon() function to update the weapon state.
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Weapon() = default;     // Default destructor
};


} // namespace hd
