/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DamageAbsorber.h
 * @brief Header file for the DamageAbsorber class.
 * 
 * ARCHITECTURAL NOTES:
 * - DamageAbsorber is an ability that can be used to absorb damage.
 * - DamageAbsorber is a subclass of Ability.
 */

#pragma once
#include "Ability.h"

namespace hd {

struct DamageAbsorberInfo : public AbilityInfo {
    DamageAbsorberInfo() {
        nodeType = "Gameplay/DamageAbsorber";
        
        inputs = {
            "absorption",       // Absorption of the damage absorber ability
            "duration",         // Duration of the damage absorber ability
            "damageAbsorberData",// Damage absorber data
            "environment",      // Environment data
            "characterData",    // Character data
            "damageAbsorberState"// Damage absorber state
        };
        
        outputs = {
            "damageAbsorberStatus",  // Damage absorber status
            "damageAbsorberMetrics"  // Damage absorber performance metrics
        };
    }
};

class DamageAbsorber : public Ability {
public:
    // === Allocation, Initialization, Loading === 
    explicit DamageAbsorber(const DamageAbsorberInfo& info = DamageAbsorberInfo())
        : Ability(info) {}    // Default constructor
    initialize() override {}
    load() override {}

    // Set default values
    float absorption = 0.0f;
    float capacity = 0.0f;
    float duration = 0.0f;
    float cooldown = 0.0f;
    float regeneration = 0.0f;
    DataTable damageAbsorberData;

    // === Processing ===
    void processNode() override { }
    void processDamageAbsorber();
    void regenerate();               // Regenerate the absorption of the damage absorber ability
    void cooldown();                 // Cooldown the damage absorber ability
    void activate();                 // Activate the damage absorber ability
    void update();                   // Update the state of the damage absorber ability

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DamageAbsorber() = default;     // Default destructor
};

} // namespace hd
