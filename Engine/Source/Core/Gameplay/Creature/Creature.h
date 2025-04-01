/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Creature.h
 * @brief Header file for the Creature class.
 * 
 * ARCHITECTURAL NOTES:
 * - Creature is a class that represents a creature in Hydragon.
 * - It is a customized version of the Character class.
 * - It is used to represent any creature in the game world, such as animals, monsters, etc.
 * - It supports interactive features, such as moving, attacking, and defending. Also supports two-way messaging with other creatures, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Character.h"

namespace hd {

struct CreatureInfo : public CharacterInfo {
    CreatureInfo() {
        nodeType = "Gameplay/Creature";
        
        inputs = {
            "creatureType",     // Type of creature (animal, monster, etc.)
            "creatureData",     // Creature data
            "environment",      // Environment data
            "characterData",    // Character data
            "creatureState"     // Creature state
        };
        
        outputs = {
            "creatureStatus",   // Creature status
            "creatureMetrics"   // Creature performance metrics
        };
    }
};

class Creature : public Character {
public:
    // === Allocation, Initialization, Loading ===
    explicit Creature(const CreatureInfo& info = CreatureInfo())
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
    ~Creature() = default;     // Default destructor
};

} // namespace hd

