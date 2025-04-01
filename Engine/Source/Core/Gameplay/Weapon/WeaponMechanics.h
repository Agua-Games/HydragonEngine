/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WeaponMechanics.h
 * @brief Header file for the WeaponMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - WeaponMechanics is a class that represents weapon mechanics in Hydragon.
 * - It is used to represent any weapon mechanics in the game world, such as damage, accuracy, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other weapon mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct WeaponMechanicsInfo : public NodeInfo {
    WeaponMechanicsInfo() {
        nodeType = "Gameplay/WeaponMechanics";
        
        inputs = {
            "weaponData",        // Weapon data
            "environment",       // Environment data
            "characterData",     // Character data
            "weaponState"        // Weapon state
        };
        
        outputs = {
            "weaponStatus",      // Weapon status
            "weaponMetrics"      // Weapon performance metrics
        };
    }
};

class WeaponMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit WeaponMechanics(const WeaponMechanicsInfo& info = WeaponMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable weaponMechanicsData;
    std::vector<Weapon> weapons;
    float accuracy = 0.0f;
    float lockTime = 0.0f;
    DataTable weaponMechanicsState;
    DataTable weaponMechanicsStatus;
    DataTable weaponMechanicsMetrics;

    // === Processing ===
    void processNode() override { }
    void processWeaponMechanics();     // Process the weapon mechanics. This function is called by the processNode() function. It is responsible for updating the weapon mechanics state.
    void addWeapon(const std::string& weaponName);
    void removeWeapon(const std::string& weaponName);
    void fire();
    void recoil();
    void reload();
    void aim();
    void lockOnTarget();
    void drop();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WeaponMechanics() = default;     // Default destructor
};

} // namespace hd