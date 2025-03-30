/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Dagger.h
 * @brief Header file for the Dagger class.
 * 
 * ARCHITECTURAL NOTES:
 * - Dagger is a class that represents a dagger in Hydragon.
 * - It is used to represent any dagger in the game world, such as short swords, knives, etc.
 * - It supports interactive features, such as slashing, blocking, and parrying. Also supports two-way messaging with other daggers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct DaggerInfo : public WeaponInfo {
    DaggerInfo() {
        nodeType = "Gameplay/Dagger";
        
        inputs = {
            "daggerType",  // Type of dagger (short sword, knife, etc.)
            "daggerData",  // Dagger data
            "environment", // Environment data
            "characterData", // Character data
            "daggerState" // Dagger state
        };
        
        outputs = {
            "daggerStatus", // Dagger status
            "daggerMetrics" // Dagger performance metrics
        };
    }
};

class Dagger : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Dagger(const DaggerInfo& info = DaggerInfo())
        : Weapon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void slash();
    void block();
    void parry();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Dagger() = default;     // Default destructor
};

} // namespace hd