/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ReconnaissanceAbility.h
 * @brief Header file for the ReconnaissanceAbility class.
 * 
 * ARCHITECTURAL NOTES:
 * - ReconnaissanceAbility is a class that represents a reconnaissance ability in Hydragon.
 * - It is used to represent any reconnaissance ability in the game world, such as spotting, tracking, and scouting.
 * - It supports interactive features, such as spotting, tracking, and scouting. Also supports two-way messaging with other reconnaissance abilities, environment, character, etc.
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

struct ReconnaissanceAbilityInfo : public AbilityInfo {
    ReconnaissanceAbilityInfo() {
        nodeType = "Gameplay/ReconnaissanceAbility";
        
        inputs = {
            "spotting",          // Whether the reconnaissance ability has spotting
            "tracking",          // Whether the reconnaissance ability has tracking
            "scouting",          // Whether the reconnaissance ability has scouting
            "reconnaissanceData",// Reconnaissance data
            "environment",       // Environment data
            "characterData",     // Character data
            "reconnaissanceState"// Reconnaissance state
        };
        
        outputs = {
            "reconnaissanceStatus",// Reconnaissance status
            "reconnaissanceMetrics"// Reconnaissance performance metrics
        };
    }
};

class ReconnaissanceAbility : public Ability {
public:
    // === Allocation, Initialization, Loading === 
    explicit ReconnaissanceAbility(const ReconnaissanceAbilityInfo& info = ReconnaissanceAbilityInfo())
        : Ability(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    bool spotting = false;   
    bool tracking = false;
    bool scouting = false;
    DataTable reconnaissanceAbilityData;

    // === Processing ===
    void processNode() override { }
    void processReconnaissanceAbility();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ReconnaissanceAbility() = default;     // Default destructor
};

} // namespace hd
