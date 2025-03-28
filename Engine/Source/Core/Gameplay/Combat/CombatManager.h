/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatManager.h
 * @brief Header file for the CombatManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatManager is a singleton class that manages the combat in the game.
 * - It is responsible for updating the combat and managing the combat system.
 * - It uses the Vulkan API for combat management.
 */
#pragma once

#include "Core.h"
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct CombatManagerInfo : public NodeInfo {
    CombatManagerInfo() {
        nodeType = "Gameplay/CombatManager";
        
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

class CombatManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CombatManager(const CombatManagerInfo& info = CombatManagerInfo())
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
    ~CombatManager() = default;     // Default destructor
};

} // namespace hd