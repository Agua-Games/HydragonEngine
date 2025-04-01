/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MagicAttackAbility.h
 * @brief Header file for the MagicAttackAbility class.
 * 
 * ARCHITECTURAL NOTES:
 * - MagicAttackAbility is a class that represents a magic attack ability in Hydragon.
 * - It is used to represent any magic attack ability in the game world, such as spells, powers, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other magic attack abilities, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Ability.h"

namespace hd {

struct MagicAttackAbilityInfo : public AbilityInfo {
    MagicAttackAbilityInfo() {
        nodeType = "Gameplay/MagicAttackAbility";
        
        inputs = {
            "magicAttackData",  // Magic attack data
            "environment",      // Environment data
            "characterData",    // Character data
            "magicAttackState"  // Magic attack state
        };
        
        outputs = {
            "magicAttackStatus",  // Magic attack status
            "magicAttackMetrics"  // Magic attack performance metrics
        };
    }
};

class MagicAttackAbility : public Ability {
public:
    // === Allocation, Initialization, Loading ===
    explicit MagicAttackAbility(const MagicAttackAbilityInfo& info = MagicAttackAbilityInfo())
        : Ability(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float damage = 0.0f;
    float cooldown = 0.0f;
    float range = 0.0f;
    float radius = 0.0f;
    int maxTargets = 0;
    float chainRange = 0.0f;  // Range for chain reaction ability to target additional targets
    float cost = 0.0f;
    float force = 0.0f;
    float momentum = 0.0f;
    float duration = 0.0f;
    float healing = 0.0f;
    float freezeChance = 0.0f;
    float protection = 0.0f;
    float damageReduction = 0.0f;  // Each subsequent target takes 20% less damage
    std::string targeting = "single";
    float tickRate = 0.0f;

    DataTable magicAttackData;
    DataTable environment;
    DataTable characterData;
    DataTable magicAttackState;

    // === Processing ===
    void processNode() override { }
    void cast();
    void target();
    void cooldown();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MagicAttackAbility() = default;     // Default destructor
};

} // namespace hd
