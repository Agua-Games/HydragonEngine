/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WarHorse.h
 * @brief Header file for the WarHorse class.
 * 
 * ARCHITECTURAL NOTES:
 * - WarHorse is a class that represents a warhorse in Hydragon.
 * - It is used to represent any warhorse in the game world, such as stallions, mares, geldings, etc.
 * - It supports interactive features, such as charging, galloping, and neighing. Also supports two-way messaging with other warhorses, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Horse.h"
#include "HorseTypes.h"
#include "WarHorseMechanics.h"
#include "Ability.h"

namespace hd {

struct WarHorseInfo : public HorseInfo {
    WarHorseInfo() {
        nodeType = "Gameplay/WarHorse";
        
        inputs = {
            "warhorseType",        // Type of warhorse (stallion, mare, gelding, etc.)
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

class WarHorse : public Horse {
public:
    // === Allocation, Initialization, Loading ===
    explicit WarHorse(const WarHorseInfo& info = WarHorseInfo())
        : Horse(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    WarHorseType type = WarHorseType::Other;
    WarHorseBreed breed = WarHorseBreed::Other;
    HorseArmorType armorType = HorseArmorType::None;

    // === Processing ===
    processNode() override {}
    void setArmor(HorseArmorType armorType);
    void train(Ability ability, float level);
    void charge(vec3 target);
    void evade(vec3 target);      // Evade the target. It's more battle-oriented than fleeing.
    void processWarHorse();    // Process the warhorse.

    // === Cleanup ===
    unload() override {}
    cleanup() override {}
};

}