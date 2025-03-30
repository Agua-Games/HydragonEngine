/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TradingManager.h
 * @brief Header file for the TradingManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - TradingManager is a singleton class that manages the trading in the game.
 * - It is responsible for managing the trading in the game.
 * - It supports interactive features, such as buying, selling, and trading. Also supports two-way messaging with other trading, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct TradingManagerInfo : public NodeInfo {
    TradingManagerInfo() {
        nodeType = "Gameplay/TradingManager";
        
        inputs = {
            "tradingData",        // Trading data
            "environment",        // Environment data
            "characterData",      // Character data
            "tradingState"        // Trading state
        };
        
        outputs = {
            "tradingStatus",      // Trading status
            "tradingMetrics"      // Trading performance metrics
        };
    }
};

class TradingManager : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit TradingManager(const TradingManagerInfo& info = TradingManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void buy();
    void sell();
    void trade();
    void processTrading();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TradingManager() = default;     // Default destructor
};

} // namespace hd
