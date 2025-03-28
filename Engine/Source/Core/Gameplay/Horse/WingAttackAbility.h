/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WingAttackAbility.h
 * @brief Header file for the WingAttackAbility class.
 * 
 * ARCHITECTURAL NOTES:
 * - WingAttackAbility is a class that represents a wing attack ability in Hydragon.
 * - It is used to represent any wing attack ability in the game world, such as swooping down and attacking with wings.
 * - It supports interactive features, such as swooping, attacking, and dodging. Also supports two-way messaging with other wing attack abilities, environment, character, etc.
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

struct WingAttackAbilityInfo : public AbilityInfo {
    WingAttackAbilityInfo() {
        nodeType = "Gameplay/WingAttackAbility";
        
        inputs = {
            "wingAttackData",  // Wing attack data
            "environment",     // Environment data
            "characterData",   // Character data
            "wingAttackState"  // Wing attack state
        };
        
        outputs = {
            "wingAttackStatus",  // Wing attack status
            "wingAttackMetrics"  // Wing attack performance metrics
        };
    }
};

class WingAttackAbility : public Ability {
public:
    // === Allocation, Initialization, Loading === 
    explicit WingAttackAbility(const WingAttackAbilityInfo& info = WingAttackAbilityInfo())
        : Ability(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void swoop(); // Swoop down and attack with wings.
    void dodge(); // Dodge incoming attacks.
    void processWingAttack();
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WingAttackAbility() = default;     // Default destructor
};

} // namespace hd