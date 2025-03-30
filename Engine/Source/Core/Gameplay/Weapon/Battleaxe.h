/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Battleaxe.h
 * @brief Header file for the Battleaxe class.
 * 
 * ARCHITECTURAL NOTES:
 * - Battleaxe is a class that represents a battleaxe in Hydragon.
 * - It is used to represent any battleaxe in the game world, such as greatswords, battleaxes, etc.
 * - It supports interactive features, such as slashing, blocking, and parrying. Also supports two-way messaging with other battleaxes, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Weapon.h"

namespace hd {

struct BattleaxeInfo : public WeaponInfo {
    BattleaxeInfo() {
        nodeType = "Gameplay/Battleaxe";
        
        inputs = {
            "battleaxeType",  // Type of battleaxe (greatsword, battleaxe, etc.)
            "battleaxeData",  // Battleaxe data
            "environment", // Environment data
            "characterData", // Character data
            "battleaxeState" // Battleaxe state
        };
        
        outputs = {
            "battleaxeStatus", // Battleaxe status
            "battleaxeMetrics" // Battleaxe performance metrics
        };
    }
};

class Battleaxe : public Weapon {
public:
    // === Allocation, Initialization, Loading ===
    explicit Battleaxe(const BattleaxeInfo& info = BattleaxeInfo())
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
    ~Battleaxe() = default;     // Default destructor
};

} // namespace hd

