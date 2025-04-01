/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Torpedo.h
 * @brief Header file for the Torpedo class.
 * 
 * ARCHITECTURAL NOTES:
 * - Torpedo is a class that represents a torpedo in Hydragon.
 * - It is used to represent any torpedo in the game world, such as torpedoes, missiles, etc.
 * - It supports interactive features, such as firing, reloading, and aiming. Also supports two-way messaging with other torpedoes, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Weapon.h"
#include "DataTable.h"

namespace hd {

struct TorpedoInfo : public WeaponInfo {
    TorpedoInfo() {
        nodeType = "Gameplay/Torpedo";
        
        inputs = {
            "torpedoType",  // Type of torpedo (torpedo, missile, etc.)
            "torpedoData",  // Torpedo data
            "environment", // Environment data
            "characterData", // Character data
            "torpedoState" // Torpedo state
        };
        
        outputs = {
            "torpedoStatus", // Torpedo status
            "torpedoMetrics" // Torpedo performance metrics
        };
    }
};

class Torpedo : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Torpedo(const TorpedoInfo& info = TorpedoInfo())
        : Weapon(info) {}
    initialize() override;
    load() override;

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }
    void processTorpedo();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Torpedo() = default;     // Default destructor
};

} // namespace hd
