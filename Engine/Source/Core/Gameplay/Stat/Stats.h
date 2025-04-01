/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Stats.h
 * @brief Header file for the Stats class.
 * 
 * ARCHITECTURAL NOTES:
 * - Stats is a class that represents a stats node in the engine's node graph.
 * - It is used to represent any stats in the game world, such as health, mana, etc.
 * - It supports interactive features, such as modifying, updating, and accessing. Also supports two-way messaging with other stats, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct StatsInfo : public NodeInfo {
    StatsInfo() {
        nodeType = "Gameplay/Stats";
        
        inputs = {
            "statsType",       // Type of stats (health, mana, etc.)
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

class Stats : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Stats(const StatsInfo& info = StatsInfo())
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
    ~Stats() = default;     // Default destructor
};

} // namespace hd
