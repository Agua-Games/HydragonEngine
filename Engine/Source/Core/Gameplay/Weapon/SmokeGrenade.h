/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SmokeGrenade.h
 * @brief Header file for the SmokeGrenade class.
 * 
 * ARCHITECTURAL NOTES:
 * - SmokeGrenade is a class that represents a smoke grenade in Hydragon.
 * - It is used to represent any smoke grenade in the game world, such as smoke grenades, flashbangs, etc.
 * - It supports interactive features, such as throwing, deploying, and detonating. Also supports two-way messaging with other smoke grenades, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "Grenade.h"

namespace hd {

struct SmokeGrenadeInfo : public GrenadeInfo {
    SmokeGrenadeInfo() {
        nodeType = "Gameplay/SmokeGrenade";
        
        inputs = {
            "smokeGrenadeType",  // Type of smoke grenade (smoke, flashbang, etc.)
            "smokeGrenadeData",  // Smoke grenade data
            "environment", // Environment data
            "characterData", // Character data
            "smokeGrenadeState" // Smoke grenade state
        };
        
        outputs = {
            "smokeGrenadeStatus", // Smoke grenade status
            "smokeGrenadeMetrics" // Smoke grenade performance metrics
        };
    }
};

class SmokeGrenade : public Grenade {
public:
    // === Allocation, Initialization, Loading ===
    explicit SmokeGrenade(const SmokeGrenadeInfo& info = SmokeGrenadeInfo())
        : Grenade(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processSmokeGrenade();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SmokeGrenade() = default;     // Default destructor
};

} // namespace hd
