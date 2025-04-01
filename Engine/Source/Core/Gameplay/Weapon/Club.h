/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Club.h
 * @brief Header file for the Club class.
 * 
 * ARCHITECTURAL NOTES:
 * - Club is a class that represents a club in Hydragon.
 * - It is used to represent any club in the game world, such as maces, clubs, etc.
 * - It supports interactive features, such as slashing, blocking, and parrying. Also supports two-way messaging with other clubs, environment, character, etc.
 */
#pragma once
#include "Weapon.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct ClubInfo : public WeaponInfo {
    ClubInfo() {
        nodeType = "Gameplay/Club";
        
        inputs = {
            "clubType",  // Type of club (mace, club, etc.)
            "clubData",  // Club data
            "environment", // Environment data
            "characterData", // Character data
            "clubState" // Club state
        };
        
        outputs = {
            "clubStatus", // Club status
            "clubMetrics" // Club performance metrics
        };
    }
};

class Club : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Club(const ClubInfo& info = ClubInfo())
        : Weapon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void slash();
    void block();
    void parry();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Club() = default;     // Default destructor
};

} // namespace hd
