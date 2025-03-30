/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BattleManager.h
 * @brief Header file for the BattleManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - BattleManager is a singleton class that manages the battles in the game.
 * - It is responsible for managing the battles in the game.
 * - It supports interactive features, such as starting, ending, and managing battles. Also supports two-way messaging with other battles, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {
struct BattleManagerInfo : public NodeInfo {
    BattleManagerInfo() {
        nodeType = "Gameplay/BattleManager"; 
        
        inputs = {
            "battleData",        // Battle data
            "environment",       // Environment data
            "characterData",     // Character data
            "battleState"        // Battle state
        };
        outputs = {
            "battleStatus",      // Battle status
            "battleMetrics"      // Battle performance metrics
        };       
    }
};

class BattleManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BattleManager(const BattleManagerInfo& info = BattleManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void startBattle();
    void endBattle();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BattleManager() = default;     // Default destructor
};

} // namespace hd