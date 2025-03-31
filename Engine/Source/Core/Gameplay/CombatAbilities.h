/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatAbilities.h
 * @brief Header file for the CombatAbilities class.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatAbilities is a class that represents combat abilities in Hydragon.
 * - It supports chaining abilities together in a single node, akin to how PostProcessChain chains PostProcess effects.
 * - It supports interactive features, such as dealing damage, taking damage, and healing. Also supports two-way messaging with other combat abilities, environment, character, etc.
 * - It is used to represent any combat abilities in the game world, such as damage, health, etc.
 */
 #pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Ability.h"

namespace hd {

struct CombatAbilitiesInfo : public NodeInfo {
    CombatAbilitiesInfo() {
        nodeType = "Gameplay/CombatAbilities";
        
        inputs = {
            "abilities",        // Array of abilities
            "abilityTypes",     // Ability type definitions
            "abilityData",      // Ability data
            "environment",      // Environment data
            "characterData",    // Character data
            "abilityState"      // Ability state
        };
        
        outputs = {
            "abilityStatus",    // Ability status
            "abilityMetrics"    // Ability performance metrics
        };
    }
};

class CombatAbilities : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit CombatAbilities(const CombatAbilitiesInfo& info = CombatAbilitiesInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    std::vector<Ability> abilities;
    std::unordered_map<std::string, AbilityType> abilityTypes;
    DataTable abilityData;
    DataTable environmentData;
    DataTable characterData;
    DataTable abilityState;

    // === Processing ===
    void processNode() override { }
    void addAbility(const Ability& ability);
    void removeAbility(const Ability& ability);
    void processAbilities();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CombatAbilities() = default;     // Default destructor
};

}