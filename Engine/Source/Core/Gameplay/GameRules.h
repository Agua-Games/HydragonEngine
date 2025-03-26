/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GameRules.h
 * @brief Header file for the GameRules class.
 * 
 * ARCHITECTURAL NOTES:
 * - GameRules is a class that represents the rules of the game in Hydragon.
 * - It is used to define and manage the rules of the game.
 * - It supports interactive features, such as rule management, rule validation, rule enforcement, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct GameRulesInfo : public NodeInfo {
    GameRulesInfo() {
        nodeType = "Gameplay/GameRules";
        
        inputs = {
            "rules",           // Array of game rules
            "ruleState",       // Current state of the rules
            "ruleSpeed",       // Speed of the rules
            "ruleLoop"         // Whether the rules should loop
        };
        
        outputs = {
            "ruleStatus",      // Status of the rules
            "ruleMetrics"      // Performance metrics of the rules
        };
    }
};

class GameRules : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit GameRules(const GameRulesInfo& info = GameRulesInfo())
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
    ~GameRules() = default;     // Default destructor
};

} // namespace hd

