/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DodgeAttackAbility.h
 * @brief Header file for the DodgeAttackAbility class.
 * 
 * ARCHITECTURAL NOTES:
 * - DodgeAttackAbility is a class that represents a dodge attack ability in Hydragon.
 * - It is used to represent any dodge attack ability in the game world, such as dodging and attacking.
 * - It supports interactive features, such as dodging and attacking. Also supports two-way messaging with other dodge attack abilities, environment, character, etc.
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

struct DodgeAttackAbilityInfo : public AbilityInfo {
    DodgeAttackAbilityInfo() {
        nodeType = "Gameplay/DodgeAttackAbility";
        
        inputs = {
            "dodgeAttackData",  // Dodge attack data
            "environment",      // Environment data
            "characterData",    // Character data
            "dodgeAttackState"  // Dodge attack state
        };
        
        outputs = {
            "dodgeAttackStatus",  // Dodge attack status
            "dodgeAttackMetrics"  // Dodge attack performance metrics
        };
    }
};

class DodgeAttackAbility : public Ability {
public:
    // === Allocation, Initialization, Loading === 
    explicit DodgeAttackAbility(const DodgeAttackAbilityInfo& info = DodgeAttackAbilityInfo())
        : Ability(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float dodgeSpeed = 0.0f;
    float dodgeDuration = 0.0f;
    float dodgeRange = 0.0f;
    float dodgeRadius = 0.0f;
    float staminaCost = 0.0f;
    float speedBonus = 0.0f;

    DataTable dodgeAttackData;
    DataTable environment;
    DataTable characterData;
    DataTable dodgeAttackState;

    // === Processing ===
    void processNode() override { }
    void dodge();
    void attack();
    void cooldown();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DodgeAttackAbility() = default;     // Default destructor
};

} // namespace hd

