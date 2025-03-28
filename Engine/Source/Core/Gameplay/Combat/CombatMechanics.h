/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatMechanics.h
 * @brief Header file for the CombatMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatMechanics is a class that represents combat mechanics in Hydragon.
 * - It is used to represent any combat mechanics in the game world, such as damage, health, etc.
 * - It supports interactive features, such as dealing damage, taking damage, and healing. Also supports two-way messaging with other combat mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct CombatMechanicsInfo : public NodeInfo {
    CombatMechanicsInfo() {
        nodeType = "Gameplay/CombatMechanics";
        
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

class CombatMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit CombatMechanics(const CombatMechanicsInfo& info = CombatMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CombatMechanics() = default;     // Default destructor
};

} // namespace hd
