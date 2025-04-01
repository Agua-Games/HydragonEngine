/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ArcheryAbility.h
 * @brief Header file for the ArcheryAbility class.
 * 
 * ARCHITECTURAL NOTES:
 * - ArcheryAbility is a class that represents an archery ability in Hydragon.
 * - It is used to represent any archery ability in the game world, such as firing arrows, reloading arrows, and aiming arrows.
 * - It supports interactive features, such as firing arrows, reloading arrows, and aiming arrows. Also supports two-way messaging with other archery abilities, environment, character, etc.
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

struct ArcheryAbilityInfo : public AbilityInfo {
    ArcheryAbilityInfo() {
        nodeType = "Gameplay/ArcheryAbility";
        
        inputs = {
            "archeryAbilityData",  // Archery ability data
            "environment",        // Environment data
            "characterData",      // Character data
            "archeryAbilityState"  // Archery ability state
        };
        
        outputs = {
            "archeryAbilityStatus",  // Archery ability status
            "archeryAbilityMetrics"  // Archery ability performance metrics
        };
    }
};

class ArcheryAbility : public Ability {
public:
    // === Allocation, Initialization, Loading === 
    explicit ArcheryAbility(const ArcheryAbilityInfo& info = ArcheryAbilityInfo())
        : Ability(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float accuracy = 0.0f;
    float stability = 0.0f;
    DataTable archeryAbilityData;

    // === Processing ===
    void processNode() override { }
    void processArcheryAbility();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ArcheryAbility() = default;     // Default destructor
};

} // namespace hd
