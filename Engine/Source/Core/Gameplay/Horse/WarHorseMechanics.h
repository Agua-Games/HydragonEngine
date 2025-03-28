/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WarHorseMechanics.h
 * @brief Header file for the WarHorseMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - WarHorseMechanics is a class that inherits from HorseMechanics and represents warhorse mechanics in Hydragon.
 * - It is used to represent any warhorse mechanics like seen in warhorse games, like horse combat, horse training, horse breeding, etc.
 * - It supports interactive features, such as charging, galloping, and neighing. Also supports two-way messaging with other warhorse mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "HorseMechanics.h"
#include "Ability.h"

namespace hd {

struct WarHorseMechanicsInfo : public HorseMechanicsInfo {
    WarHorseMechanicsInfo() {
        nodeType = "Gameplay/WarHorseMechanics";
        
        inputs = {
            "warhorseData",        // Warhorse data
            "environment",         // Environment data
            "characterData",       // Character data
            "warhorseState"        // Warhorse state
        };
        
        outputs = {
            "warhorseStatus",      // Warhorse status
            "warhorseMetrics"      // Warhorse performance metrics
        };
    }
};

class WarHorseMechanics : public HorseMechanics {
public:
    // === Allocation, Initialization, Loading === 
    explicit WarHorseMechanics(const WarHorseMechanicsInfo& info = WarHorseMechanicsInfo())
        : HorseMechanics(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void setArmor(std::string armorType);
    void train(Ability ability, float level);
    void charge(vec3 target);
    void evade(vec3 target);      // Evade the target. It's more battle-oriented than fleeing.
    void attack(vec3 target);     // Attack the target.
    void defend(vec3 target);     // Defend against the target.
    void processNode() override { }
    void update();
    void processWarHorse();    // Process the warhorse.

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WarHorseMechanics() = default;     // Default destructor
};

} // namespace hd
