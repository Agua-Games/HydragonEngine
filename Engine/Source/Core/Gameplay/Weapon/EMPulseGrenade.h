/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EMPulseGrenade.h
 * @brief Header file for the EMPulseGrenade class.
 * 
 * ARCHITECTURAL NOTES:
 * - EMPulseGrenade is a class that represents an EMPulse grenade in Hydragon.
 * - It is used to represent any EMPulse grenade in the game world, such as EMPulse grenades, EMPulse mines, etc.
 * - It supports interactive features, such as throwing, deploying, and detonating. Also supports two-way messaging with other EMPulse grenades, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "Grenade.h"

namespace hd {

struct EMPulseGrenadeInfo : public GrenadeInfo {
    EMPulseGrenadeInfo() {
        nodeType = "Gameplay/EMPulseGrenade";
        
        inputs = {
            "EMPulseGrenadeType",  // Type of EMPulse grenade (EMPulse grenade, EMPulse mine, etc.)
            "EMPulseGrenadeData",  // EMPulse grenade data
            "environment", // Environment data
            "characterData", // Character data
            "EMPulseGrenadeState" // EMPulse grenade state
        };
        
        outputs = {
            "EMPulseGrenadeStatus", // EMPulse grenade status
            "EMPulseGrenadeMetrics" // EMPulse grenade performance metrics
        };
    }
};

class EMPulseGrenade : public Grenade {
public:
    // === Allocation, Initialization, Loading ===
    explicit EMPulseGrenade(const EMPulseGrenadeInfo& info = EMPulseGrenadeInfo())
        : Grenade(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processEMPulseGrenade();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EMPulseGrenade() = default;     // Default destructor
};

} // namespace hd
