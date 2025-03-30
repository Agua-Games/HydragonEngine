/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TradingStation.h
 * @brief Header file for the TradingStation class.
 * 
 * ARCHITECTURAL NOTES:
 * - TradingStation is a class that represents a trading station in Hydragon.
 * - It is used to represent any trading station in the game world, such as trading posts, ATMs, currency exchanges, markets, etc.
 * - It supports interactive features, such as buying, selling, and trading. Also supports two-way messaging with other trading stations, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"

namespace hd {

enum class TradingStationType {
    tradingPost,
    ATM,
    market,
    creditsExchange,
    creditsTerminal,
    cargoExchange,
    cargoTerminal,
    custom
};

struct TradingStationInfo : public NodeInfo {
    TradingStationInfo() {
        nodeType = "Gameplay/TradingStation";
        
        inputs = {
            "tradingStationType",  // Type of trading station (trading post, market, etc.)
            "tradingStationData",  // Trading station data
            "tradingMechanicsData", // Trading mechanics data
            "environment",         // Environment data
            "characterData",       // Character data
            "tradingStationState"  // Trading station state
        };
        
        outputs = {
            "tradingStationStatus",  // Trading station status
            "tradingStationMetrics"  // Trading station performance metrics
        };
    }
};

class TradingStation : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit TradingStation(const TradingStationInfo& info = TradingStationInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    TradingStationType type = TradingStationType::custom;
    float capacity = 0.0f;
    float credits = 0.0f;
    DataTable cargo;

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
    ~TradingStation() = default;     // Default destructor
};

} // namespace hd
