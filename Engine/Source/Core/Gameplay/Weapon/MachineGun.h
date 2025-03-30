/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MachineGun.h
 * @brief Header file for the MachineGun class.
 * 
 * ARCHITECTURAL NOTES:
 * - MachineGun is a class that represents a machine gun in Hydragon.
 * - It is used to represent any machine gun in the game world, such as assault rifles, submachine guns, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other machine guns, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct MachineGunInfo : public WeaponInfo {
    MachineGunInfo() {
        nodeType = "Gameplay/MachineGun";
        
        inputs = {
            "machineGunType",  // Type of machine gun (assault rifle, submachine gun, etc.)
            "machineGunData",  // Machine gun data
            "environment",     // Environment data
            "characterData",   // Character data
            "machineGunState"  // Machine gun state
        };
        
        outputs = {
            "machineGunStatus",  // Machine gun status
            "machineGunMetrics"  // Machine gun performance metrics
        };
    }
};

class MachineGun : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit MachineGun(const MachineGunInfo& info = MachineGunInfo())
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
    ~MachineGun() = default;     // Default destructor
};

} // namespace hd