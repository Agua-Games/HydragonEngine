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
        nodeType = "Gameplay/Weapon";
        inputs = {
            "ammo",         // Ammo of the weapon
            "ammoType",     // Ammo type of the weapon
            "ammoCount",    // Ammo count of the weapon
            "ammoCapacity", // Ammo capacity of the weapon
            "ammoAimTime",  // Ammo aim time of the weapon
            "ammoFire",     // Ammo fire of the weapon
            "ammoFireRate", // Ammo fire rate of the weapon
            "ammoReload",   // Ammo reload of the weapon
            "ammoReloadTime", // Ammo reload time of the weapon
            "ammoAim",      // Ammo aim of the weapon
            "ammoAccuracy", // Ammo accuracy of the weapon
            "ammoDamage",   // Ammo damage of the weapon
            "ammoSpeed",    // Ammo speed of the weapon
            "ammoRange",    // Ammo range of the weapon
            "ammoSpread",   // Ammo spread of the weapon
            "ammoPenetration", // Ammo penetration of the weapon
            "ammoDrop",     // Ammo drop of the weapon
            "ammoDropTime", // Ammo drop time of the weapon
            "ammoDropRate", // Ammo drop rate of the weapon
            "ammoDropChance", // Ammo drop chance of the weapon
            "ammoDropAmount", // Ammo drop amount of the weapon
            "ammoDropType", // Ammo drop type of the weapon
            "ammoDropPosition", // Ammo drop position of the weapon
            "ammoDropVelocity", // Ammo drop velocity of the weapon
            "ammoDropAngularVelocity", // Ammo drop angular velocity of the weapon
            "ammoDropRotation", // Ammo drop rotation of the weapon
            "ammoDropScale", // Ammo drop scale of the weapon
            "ammoDropMaterial", // Ammo drop material of the weapon
            "ammoDropAnimation", // Ammo drop animation of the weapon
            "ammoDropPhysics", // Ammo drop physics of the weapon
            "ammoDropCollision", // Ammo drop collision of the weapon
            "ammoDropScript", // Ammo drop script of the weapon
        };
        outputs = {
            "ammo",         // Ammo of the weapon
            "ammoType",     // Ammo type of the weapon
            "ammoCount",    // Ammo count of the weapon
            "ammoCapacity", // Ammo capacity of the weapon
            "ammoAimTime",  // Ammo aim time of the weapon
            "ammoFire",     // Ammo fire of the weapon
            "ammoFireRate", // Ammo fire rate of the weapon
            "ammoReload",   // Ammo reload of the weapon
            "ammoReloadTime", // Ammo reload time of the weapon
            "ammoAim",      // Ammo aim of the weapon
            "ammoAccuracy", // Ammo accuracy of the weapon
            "ammoDamage",   // Ammo damage of the weapon
            "ammoSpeed",    // Ammo speed of the weapon
            "ammoRange",    // Ammo range of the weapon
            "ammoSpread",   // Ammo spread of the weapon
            "ammoPenetration", // Ammo penetration of the weapon
            "ammoDrop",     // Ammo drop of the weapon
            "ammoDropTime", // Ammo drop time of the weapon
            "ammoDropRate", // Ammo drop rate of the weapon
            "ammoDropChance", // Ammo drop chance of the weapon
            "ammoDropAmount", // Ammo drop amount of the weapon
            "ammoDropType", // Ammo drop type of the weapon
            "ammoDropPosition", // Ammo drop position of the weapon
            "ammoDropVelocity", // Ammo drop velocity of the weapon
            "ammoDropAngularVelocity", // Ammo drop angular velocity of the weapon
            "ammoDropRotation", // Ammo drop rotation of the weapon
            "ammoDropScale", // Ammo drop scale of the weapon
            "ammoDropMaterial", // Ammo drop material of the weapon
            "ammoDropAnimation", // Ammo drop animation of the weapon
            "ammoDropPhysics", // Ammo drop physics of the weapon
            "ammoDropCollision", // Ammo drop collision of the weapon
            "ammoDropScript", // Ammo drop script of the weapon
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
    void processNode() override {
        
    }
    void aim();
    void reload();
    void fire();
    void processWeapon();
    void () override {
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
