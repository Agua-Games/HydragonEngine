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
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct AbilityInfo : public NodeInfo {
    AbilityInfo() {
        nodeType = "Gameplay/Ability";
        
        inputs = {
            "abilityType",     // Type of ability (skill, power, spell, etc.)
            "abilityData",     // Ability data
            "environment",     // Environment data
            "characterData",   // Character data
            "abilityState"     // Ability state
        };
        
        outputs = {
            "abilityStatus",   // Ability status
            "abilityMetrics"   // Ability performance metrics
        };
    }
};

class Ability : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Ability(const AbilityInfo& info = AbilityInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Ability() = default;     // Default destructor
};

} // namespace hd
