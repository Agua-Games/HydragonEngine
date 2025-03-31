/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceNavMechanics.h
 * @brief Header file for the SpaceNavMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceNavMechanics is a class that represents space navigation mechanics in Hydragon.
 * - It is used to represent any space navigation mechanics in the game world, such as space travel, space combat, etc.
 * - It supports interactive features, such as space travel, space combat, and space exploration. Also supports two-way messaging with other space navigation mechanics, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "NavigationMechanics.h"

namespace hd {

struct SpaceNavMechanicsInfo : public NavigationMechanicsInfo {
    SpaceNavMechanicsInfo() {
        nodeType = "Gameplay/SpaceNavMechanics";
        
        inputs = {
            "spaceNavMechanicsData",// Space navigation mechanics data
            "environment",          // Environment data
            "characterData",        // Character data
            "spaceNavMechanicsState"// Space navigation mechanics state
        };
        
        outputs = {
            "spaceNavMechanicsStatus",// Space navigation mechanics status
            "spaceNavMechanicsMetrics"// Space navigation mechanics performance metrics
        };
    }
};

class SpaceNavMechanics : public NavigationMechanics {
public:
    // === Allocation, Initialization, Loading === 
    explicit SpaceNavMechanics(const SpaceNavMechanicsInfo& info = SpaceNavMechanicsInfo())
        : NavigationMechanics(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable spaceNavMechanicsData;
    float stellarMapping = 0.0f;
    bool warpCapability = false;
    float warpSpeed = 0.0f;
    float warpRange = 0.0f;
    float warpCooldown = 0.0f;
    float warpDuration = 0.0f;              // Duration of warp. Default: 0.0f.
    float warpTimer = 0.0f;                 // Timer for warp cooldown and warp cooldown duration. Default: 0.0f.
    DataTable spaceNavMechanicsState;  

    // === Processing ===
    void processNode() override { }
    void update();
    void warp();                            // Function to warp the character. Default: empty function.

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpaceNavMechanics() = default;     // Default destructor
};

} // namespace hd
