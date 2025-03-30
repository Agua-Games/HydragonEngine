/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BettingMechanics.h
 * @brief Header file for the BettingMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - BettingMechanics is a class that represents betting mechanics in Hydragon.
 * - It is used to represent any betting mechanics in the game world, such as betting, odds, etc.
 * - It supports interactive features, such as betting, odds, and payouts. Also supports two-way messaging with other betting mechanics, environment, character, etc.
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

struct BettingMechanicsInfo : public NodeInfo {
    BettingMechanicsInfo() {
        nodeType = "Gameplay/BettingMechanics";
        
        inputs = {
            "bettingData",        // Betting data
            "environment",       // Environment data
            "characterData",     // Character data
            "bettingState"        // Betting state
        };
        
        outputs = {
            "bettingStatus",      // Betting status
            "bettingMetrics"      // Betting performance metrics
        };
    }
};

class BettingMechanics : public Node {
    public:
        // === Allocation, Initialization, Loading === 
        explicit BettingMechanics(const BettingMechanicsInfo& info = BettingMechanicsInfo())
            : Node(info) {}
        initialize() override {}
        load() override {}

        // Set default values
        DataTable bettingMechanicsData;
        float credits = 0.0f;
        bool oddsCalculation = false;
        DataTable betData;

        // === Processing ===
        void processNode() override { }
        void addBet(const std::string& betName);
        void removeBet(const std::string& betName);
        void processBet();
        void update();

        // === Cleanup ===
        void unload() override {}
        void cleanup() override {}
        ~BettingMechanics() = default;     // Default destructor
};

} // namespace hd