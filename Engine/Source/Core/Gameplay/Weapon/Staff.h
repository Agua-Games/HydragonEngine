/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Staff.h
 * @brief Header file for the Staff class.
 * 
 * ARCHITECTURAL NOTES:
 * - Staff is a class that represents a staff in Hydragon.
 * - It is used to represent any staff in the game world, such as quarterstaffs, rods, etc.
 * - It supports interactive features, such as slashing, blocking, and parrying. Also supports two-way messaging with other staffs, environment, character, etc.
 */
#pragma once
#include "Weapon.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct StaffInfo : public WeaponInfo {
    StaffInfo() {
        nodeType = "Gameplay/Staff";
        
        inputs = {
            "staffType",  // Type of staff (quarterstaff, rod, etc.)
            "staffData",  // Staff data
            "environment", // Environment data
            "characterData", // Character data
            "staffState" // Staff state
        };
        
        outputs = {
            "staffStatus", // Staff status
            "staffMetrics" // Staff performance metrics
        };
    }
};

class Staff : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Staff(const StaffInfo& info = StaffInfo())
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
    ~Staff() = default;     // Default destructor
};

} // namespace hd
