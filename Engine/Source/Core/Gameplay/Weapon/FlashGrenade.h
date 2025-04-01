/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FlashGrenade.h
 * @brief Header file for the FlashGrenade class.
 * 
 * ARCHITECTURAL NOTES:
 * - FlashGrenade is a class that represents a flash grenade in Hydragon.
 * - It is used to represent any flash grenade in the game world, such as flashbangs, stun grenades, etc.
 * - It supports interactive features, such as throwing, deploying, and detonating. Also supports two-way messaging with other flash grenades, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "Grenade.h"

namespace hd {

struct FlashGrenadeInfo : public GrenadeInfo {
    FlashGrenadeInfo() {
        nodeType = "Gameplay/FlashGrenade";
        
        inputs = {
            "flashGrenadeType",  // Type of flash grenade (flashbang, stun grenade, etc.)
            "flashGrenadeData",  // Flash grenade data
            "environment", // Environment data
            "characterData", // Character data
            "flashGrenadeState" // Flash grenade state
        };
        
        outputs = {
            "flashGrenadeStatus", // Flash grenade status
            "flashGrenadeMetrics" // Flash grenade performance metrics
        };
    }
};

class FlashGrenade : public Grenade {
public:
    // === Allocation, Initialization, Loading ===
    explicit FlashGrenade(const FlashGrenadeInfo& info = FlashGrenadeInfo())
        : Grenade(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processFlashGrenade();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FlashGrenade() = default;     // Default destructor
};

} // namespace hd
