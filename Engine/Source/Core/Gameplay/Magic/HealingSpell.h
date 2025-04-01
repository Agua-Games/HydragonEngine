/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HealingSpell.h
 * @brief Header file for the HealingSpell class.
 * 
 * ARCHITECTURAL NOTES:
 * - HealingSpell is a class that represents a healing spell magic ability in Hydragon.
 * - It is used to represent any healing spell magic ability in the game world, such as spells, powers, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other healing spells, environment, character, etc.
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

struct HealingSpellInfo : public AbilityInfo {
    HealingSpellInfo() {
        nodeType = "Gameplay/HealingSpell";
        
        inputs = {
            "healingSpellData",  // Healing spell data
            "environment",       // Environment data
            "characterData",     // Character data
            "healingSpellState"  // Healing spell state
        };
        
        outputs = {
            "healingSpellStatus",  // Healing spell status
            "healingSpellMetrics"  // Healing spell performance metrics
        };
    }
};

class HealingSpell : public Ability {
public:
    // === Allocation, Initialization, Loading ===
    explicit HealingSpell(const HealingSpellInfo& info = HealingSpellInfo())
        : Ability(info) {}    // Default constructor
    initialize() override {}
    load() override {}

    // Set default values
    float radius = 0.0f;       
    float range = 0.0f;         
    float healing = 0.0f;     
    float cooldown = 0.0f;        
    float regeneration = 0.0f; 
    float conversion = 0.0f;      // Conversion of damage to healing
    DataTable healingSpellData; 

    // === Processing ===
    void processNode() override { }
    void processHealingSpell();
    void regenerate();        
    void cooldown();      
    void activate();           
    void update();        

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~HealingSpell() = default;     // Default destructor
};

} // namespace hd
