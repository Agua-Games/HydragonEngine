/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file StatsManager.h
 * @brief Header file for the StatsManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - StatsManager is a singleton class that manages the stats in the game.
 * - It is responsible for updating the stats and managing the stats system.
 * - It uses the Vulkan API for stats management.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct StatsManagerInfo : public NodeInfo {
    StatsManagerInfo() {
        nodeType = "Gameplay/StatsManager";
        
        inputs = {
            "statsData",       // Stats data
            "environment",     // Environment data
            "characterData",   // Character data
            "statsState"       // Stats state
        };
        
        outputs = {
            "statsStatus",     // Stats status
            "statsMetrics"     // Stats performance metrics
        };
    }
};

class StatsManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit StatsManager(const StatsManagerInfo& info = StatsManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    }
    void addStat(const std::string& statName, float statValue);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~StatsManager() = default;     // Default destructor
};

} // namespace hd
