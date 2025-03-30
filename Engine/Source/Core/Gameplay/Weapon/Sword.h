/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Sword.h
 * @brief Header file for the Sword class.
 * 
 * ARCHITECTURAL NOTES:
 * - Sword is a class that represents a sword in Hydragon.
 * - It is used to represent any sword in the game world, such as longswords, shortswords, etc.
 * - It supports interactive features, such as slashing, blocking, and parrying. Also supports two-way messaging with other swords, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct SwordInfo : public WeaponInfo {
    SwordInfo() {
        nodeType = "Gameplay/Sword";
        
        inputs = {
            "swordType",  // Type of sword (longsword, shortsword, etc.)
            "swordData",  // Sword data
            "environment", // Environment data
            "characterData", // Character data
            "swordState" // Sword state
        };
        
        outputs = {
            "swordStatus", // Sword status
            "swordMetrics" // Sword performance metrics
        };
    }
};

class Sword : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Sword(const SwordInfo& info = SwordInfo())
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
    ~Sword() = default;     // Default destructor
};

} // namespace hd