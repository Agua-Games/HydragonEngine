/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RescueRiderAbility.h
 * @brief Header file for the RescueRiderAbility class.
 * 
 * ARCHITECTURAL NOTES:
 * - RescueRiderAbility is a class that represents a rescue rider ability in Hydragon.
 * - It is used to represent any rescue rider ability in the game world, such as saving a rider from a dangerous situation.
 * - It supports interactive features, such as saving a rider, rescuing a rider, and escaping a dangerous situation. Also supports two-way messaging with other rescue rider abilities, environment, character, etc.
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

struct RescueRiderAbilityInfo : public AbilityInfo {
    RescueRiderAbilityInfo() {
        nodeType = "Gameplay/RescueRiderAbility";
        
        inputs = {
            "stability",       // Stability of the rescue rider ability
            "rearingAttack",   // Whether the rescue rider ability has a rearing attack
            "rescueRiderData", // Rescue rider data
            "environment",     // Environment data
            "characterData",   // Character data
            "rescueRiderState" // Rescue rider state
        };
        
        outputs = {
            "rescueRiderStatus", // Rescue rider status
            "rescueRiderMetrics" // Rescue rider performance metrics
        };
    }
};

class RescueRiderAbility : public Ability {
public:
    // === Allocation, Initialization, Loading ===
    explicit RescueRiderAbility(const RescueRiderAbilityInfo& info = RescueRiderAbilityInfo())
        : Ability(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void setStability(float stability);
    void enableRearingAttack(bool enable);
    void saveRider();
    void rescueRider();
    void escapeDangerousSituation();
    void processRescueRider();
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RescueRiderAbility() = default;     // Default destructor
};

} // namespace hd

