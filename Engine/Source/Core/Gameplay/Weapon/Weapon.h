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
#include "CombatTypes.h"

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
    // === Structure Definitions ===
    enum AimType {
        EyeLevel,
        HipFire,
        Scoped,
        Custom
    };

    enum AimStance {
        Standing,
        Crouching,
        Prone,
        Custom
    };

    struct Aim {
        float time;
        vec3 direction;
        vec3 position;
        vec3 velocity;
        AimType type;
        AimStance stance;
    };

    struct Breathing {
        bool enabled;
        bool noise;
        float intensity;
        float frequency;
        vec3 offset;
    };

    struct Recoil {
        float intensity;
        float recovery;
        vec3 direction;
    };

    struct WeaponRangeType {
        float range;
        float spread;
        float accuracy;
    };

    // === Allocation, Initialization, Loading ===
    explicit Weapon(const WeaponInfo& info = WeaponInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    WeaponInfo weaponInfo = {};  // Initialize weapon info struct with default values
    // Temporarily calling them out of the struct for now.
    WeaponRangeType type = WeaponRangeType::custom;
    WeaponType weaponType = WeaponType::custom;
    AimType aimType = AimType::EyeLevel; // Aim type of the weapon. This is used for weapons that require a specific aim type, such as sniper rifles and assault rifles. Default: EyeLevel.
    int capacity = 0;
    float damage = 0.0f;
    float reloadTime = 0.0f;
    float chargeTime = 0.0f;    // Time to charge the weapon before firing. This is used for weapons that require a charge time before firing, such as bows and crossbows. Default: 0.0f.
    float chargeBonus = 0.0f;   // Bonus damage for the weapon. This is used for weapons that require a charge bonus, such as bows and crossbows. Default: 0.0f. This value is added to the damage of the weapon.
    float recoil = 0.0f;        // Recoil of the weapon. This is used for weapons that require a recoil, such as rifles and machine guns. Default: 0.0f. This value is added to the spread cone of the weapon.
    DataTable weaponData = DataTable();

    // === Processing ===
    void processNode() override {
        
    }
    void aim();
    void charge();                 // Charge the weapon before firing. This is used for weapons that require a charge time before firing, such as bows and crossbows. Default: no charge.
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
