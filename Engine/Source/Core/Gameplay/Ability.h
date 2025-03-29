/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Ability.h
 * @brief Header file for the Ability class.
 * 
 * ARCHITECTURAL NOTES:
 * - Ability is a class that represents an ability in Hydragon.
 * - It is used to represent any ability in the game world, such as skills, powers, spells, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other abilities, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

enum class AbilityType {
    skill,
    power,
    spell,
    melee,
    ranged,
    custom
};

struct AbilityInfo : public NodeInfo {
    AbilityInfo() {
        nodeType = "Gameplay/Ability";
        
        inputs = {
            "type",            // Type of ability (skill, power, spell, etc.)
            "damage",          // Damage dealt by the ability
            "cooldown",        // Cooldown time of the ability
            "range",           // Range of the ability (melee, ranged, etc.)
            "cost",            // Cost of the ability (mana, energy, etc.)
            "targeting",       // Targeting type of the ability (single, area, etc.)
            "tickRate",        // Tick rate of the ability (for continuous effects)
            "abilityData",     // Ability data (stats, etc.)
            "environment",     // Environment data (stats, etc.)
            "characterData",   // Character data (stats, etc.)
            "abilityState"     // Ability state (stats, etc.)
        };
        
        outputs = {
            "abilityStatus",   // Ability status
            "abilityMetrics"   // Ability performance metrics
        };
    }
};

/**
 * @class Ability
 * @brief Represents an ability in the game world, such as skills, powers, spells, etc.
 */
class Ability : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Ability(const AbilityInfo& info = AbilityInfo())
        : Node(info) {}
    void initialize() override {}
    void load() override {}

    // Set default values
    AbilityType type = AbilityType::custom;
    float force = 0.0f;
    float momentum = 0.0f;
    float damage = 0.0f;
    float duration = 0.0f;
    float cooldown = 0.0f;
    float range = 0.0f;
    float radius = 0.0f;
    float cost = 0.0f;
    std::string targeting = "single";
    float tickRate = 0.0f;
    DataTable abilityData = DataTable();
    DataTable environment = DataTable();
    DataTable characterData = DataTable();
    DataTable abilityState = DataTable();

    // === Processing ===
    void processNode() override {
        type = getInputValue<AbilityType>("type");
        force = getInputValue<float>("force");
        momentum = getInputValue<float>("momentum");
        damage = getInputValue<float>("damage");
        cooldown = getInputValue<float>("cooldown");
        range = getInputValue<float>("range");
        cost = getInputValue<float>("cost");
        targeting = getInputValue<std::string>("targeting");
        tickRate = getInputValue<float>("tickRate");
        abilityData = getInputValue<DataTable>("abilityData");
        environment = getInputValue<DataTable>("environment");
        characterData = getInputValue<DataTable>("characterData");
        abilityState = getInputValue<DataTable>("abilityState");
        
        // Process ability
        auto abilityStatus = updateAbility(type, abilityData, environment, characterData, abilityState);
        
        // Set outputs
        setOutputValue("abilityStatus", abilityStatus);
        setOutputValue("abilityMetrics", computeAbilityMetrics(abilityStatus));
    }

    void update() override {}

    // Ability processing
    DataTable updateAbility(const AbilityType& type, const DataTable& abilityData, const DataTable& environment, const DataTable& characterData, const DataTable& abilityState);
    DataTable computeAbilityMetrics(const DataTable& abilityStatus);

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Ability() = default;     // Default destructor
};

} // namespace hd
