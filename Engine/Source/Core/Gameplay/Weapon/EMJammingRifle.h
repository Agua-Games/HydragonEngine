/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EMJammingRifle.h
 * @brief Header file for the EMJammingRifle class.
 * 
 * ARCHITECTURAL NOTES:
 * - EMJammingRifle is a class that represents an EMJamming rifle in Hydragon.
 * - It is used to represent any EMJamming rifle in the game world, such as EMJamming rifles, EMJamming pistols, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other EMJamming rifles, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct EMJammingRifleInfo : public WeaponInfo {
    EMJammingRifleInfo() {
        nodeType = "Gameplay/EMJammingRifle";
        
        inputs = {
            "EMJammingRifleType",  // Type of EMJamming rifle (EMJamming rifle, EMJamming pistol, etc.)
            "EMJammingRifleData",  // EMJamming rifle data
            "environment", // Environment data
            "characterData", // Character data
            "EMJammingRifleState" // EMJamming rifle state
        };
        
        outputs = {
            "EMJammingRifleStatus", // EMJamming rifle status
            "EMJammingRifleMetrics" // EMJamming rifle performance metrics
        };
    }
};

class EMJammingRifle : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit EMJammingRifle(const EMJammingRifleInfo& info = EMJammingRifleInfo())
        : Weapon(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processEMJammingRifle();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EMJammingRifle() = default;     // Default destructor
};

} // namespace hd
