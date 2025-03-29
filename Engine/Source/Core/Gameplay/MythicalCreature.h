/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MythicalCreature.h
 * @brief Header file for the MythicalCreature class.
 * 
 * ARCHITECTURAL NOTES:
 * - MythicalCreature is a class that represents a mythical creature in Hydragon.
 * - It is a customized version of the Creature class.
 * - It is used to represent any mythical creature in the game world, such as dragons, unicorns, etc.
 * - It supports interactive features, such as flying, breathing fire, and casting spells. Also supports two-way messaging with other mythical creatures, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Creature.h"

namespace hd {

struct MythicalCreatureInfo : public CreatureInfo {
    MythicalCreatureInfo() {
        nodeType = "Gameplay/MythicalCreature";
        
        inputs = {
            "MythicalCreatureType",  // Type of mythical creature (dragon, unicorn, etc.)
            "MythicalCreatureData",  // mythical creature data
            "environment",            // Environment data
            "characterData",          // Character data
            "MythicalCreatureState"  // mythical creature state
        };
        
        outputs = {
            "MythicalCreatureStatus",  // mythical creature status
            "MythicalCreatureMetrics"  // mythical creature performance metrics
        };
    }
};

class MythicalCreature : public Creature {
public:
    // === Allocation, Initialization, Loading ===
    explicit MythicalCreature(const MythicalCreatureInfo& info = MythicalCreatureInfo())
        : Creature(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float mana = 0.0f;  // Mythical creatures may have mana, which is used for spells and other abilities.

    // === Processing ===
    void processNode() override {}
    void update() override {}

    // === Cleanup ===
    void cleanup() override {}
    ~MythicalCreature() = default;     // Default destructor
};

} // namespace hd
