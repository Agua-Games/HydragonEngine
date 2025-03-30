/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Shield.h
 * @brief Header file for the Shield class.
 * 
 * ARCHITECTURAL NOTES:
 * - Shield is a class that represents a shield in Hydragon.
 * - It is used to represent any shield in the game world, such as bucklers, round shields, etc.
 * - It supports interactive features, such as blocking, parrying, and deflecting. Also supports two-way messaging with other shields, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct ShieldInfo : public WeaponInfo {
    ShieldInfo() {
        nodeType = "Gameplay/Shield";
        
        inputs = {
            "shieldType",  // Type of shield (buckler, round shield, etc.)
            "shieldData",  // Shield data
            "environment", // Environment data
            "characterData", // Character data
            "shieldState" // Shield state
        };
        
        outputs = {
            "shieldStatus", // Shield status
            "shieldMetrics" // Shield performance metrics
        };
    }
};

class Shield : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Shield(const ShieldInfo& info = ShieldInfo())
        : Weapon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void block();
    void parry();
    void deflect();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Shield() = default;     // Default destructor
};

} // namespace hd