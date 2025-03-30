/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TradingMechanics.h
 * @brief Header file for the TradingMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - TradingMechanics is a class that represents trading mechanics in Hydragon.
 * - It is used to represent any trading mechanics in the game world, such as trading, buying, selling, etc.
 * - It supports interactive features, such as trading, buying, selling, and managing cargo. Also supports two-way messaging with other trading mechanics, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct TradingMechanicsInfo : public NodeInfo {
    TradingMechanicsInfo() {
        nodeType = "Gameplay/TradingMechanics";
        
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

class TradingMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit TradingMechanics(const TradingMechanicsInfo& info = TradingMechanicsInfo())
        : Node(info) { } // Constructor with default values
    initialize() override {}
    load() override {}

    // Set default values
    DataTable tradingMechanicsData;
    float capacity = 0.0f;
    float credits = 0.0f;
    DataTable cargo;
    bool autoTrade = false;
    DataTable tradingStationData;

    // === Processing ===
    void processNode() override { }
    void buy();
    void sell();
    void trade();
    void exchange();
    void queryCredits();
    void queryCargo();
    void manageCargo();
    void chargeCredits();
    void processTrading();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TradingMechanics() = default;     // Default destructor
};

} // namespace hd
