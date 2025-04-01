/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EMJammingTurret.h
 * @brief Header file for the EMJammingTurret class.
 * 
 * ARCHITECTURAL NOTES:
 * - EMJammingTurret is a class that represents an EMJamming turret in Hydragon.
 * - It is used to represent any EMJamming turret in the game world, such as EMJamming turrets, EMJamming cannons, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other EMJamming turrets, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "SentryWeapon.h"

namespace hd {

struct EMJammingTurretInfo : public SentryWeaponInfo {
    EMJammingTurretInfo() {
        nodeType = "Gameplay/EMJammingTurret";
        
        inputs = {
            "EMJammingTurretType",  // Type of EMJamming turret (EMJamming turret, EMJamming cannon, etc.)
            "EMJammingTurretData",  // EMJamming turret data
            "environment", // Environment data
            "characterData", // Character data
            "EMJammingTurretState" // EMJamming turret state
        };
        
        outputs = {
            "EMJammingTurretStatus", // EMJamming turret status
            "EMJammingTurretMetrics" // EMJamming turret performance metrics
        };
    }
};

class EMJammingTurret : public SentryWeapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit EMJammingTurret(const EMJammingTurretInfo& info = EMJammingTurretInfo())
        : SentryWeapon(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processEMJammingTurret();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EMJammingTurret() = default;     // Default destructor
};

} // namespace hd
