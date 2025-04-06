/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatAbility.h
 * @brief Header file for the CombatAbility class.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatAbility is a class that represents a combat ability in Hydragon.
 * - It is used to represent any combat ability in the game world, such as combat moves, combat spells, and combat items.
 * - It supports interactive features, such as combat moves, combat spells, and combat items. Also supports two-way messaging with other combat abilities, environment, character, etc.
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

struct CombatAbilityInfo : public AbilityInfo {
    CombatAbilityInfo() {
        nodeType = "Gameplay/CombatAbility";
        
        inputs = {
            "combatAbilityData",  // Combat ability data
            "environment",        // Environment data
            "characterData",      // Character data
            "combatAbilityState"  // Combat ability state
        };
        
        outputs = {
            "combatAbilityStatus",  // Combat ability status
            "combatAbilityMetrics"  // Combat ability performance metrics
        };
    }
};

class CombatAbility : public Ability {
public:
    // === Allocation, Initialization, Loading === 
    explicit CombatAbility(const CombatAbilityInfo& info = CombatAbilityInfo())
        : Ability(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float damage = 0.0f;
    float cooldown = 0.0f;
    float range = 0.0f;
    float radius = 0.0f;
    float cost = 0.0f;
    float force = 0.0f;
    float duration = 0.0f;
    float movementPenalty = 0.0f;

    // === Processing ===
    void processNode() override { }
    void addCombatMove(const std::string& combatAbilityName);
    void removeCombatMove(const std::string& combatAbilityName);
    void processCombatMove();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CombatAbility() = default;     // Default destructor
};

} // namespace hd
