/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MedievalCombat.h
 * @brief Header file for the MedievalCombat class.
 * 
 * ARCHITECTURAL NOTES:
 * - MedievalCombat is a class that represents medieval combat mechanics in Hydragon.
 * - It is used to represent any medieval combat mechanics in the game world, such as sword fighting, archery, etc.
 * - It supports interactive features, such as dealing damage, taking damage, and healing. Also supports two-way messaging with other medieval combat, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Combat/CombatMechanics.h"

namespace hd {

struct MedievalCombatInfo : public CombatMechanicsInfo {
    MedievalCombatInfo() {
        nodeType = "Gameplay/MedievalCombat";
        
        inputs = {
            "combatData",        // Combat data
            "environment",       // Environment data
            "characterData",     // Character data
            "combatState"        // Combat state
        };
        
        outputs = {
            "combatStatus",      // Combat status
            "combatMetrics"      // Combat performance metrics
        };
    }
};

class MedievalCombat : public CombatMechanics {
public:
    // === Allocation, Initialization, Loading === 
    explicit MedievalCombat(const MedievalCombatInfo& info = MedievalCombatInfo())
        : CombatMechanics(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addCombat(const std::string& combatName);
    void removeCombat(const std::string& combatName);
    void processCombat();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MedievalCombat() = default;     // Default destructor
};

} // namespace hd