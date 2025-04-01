/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Rank.h
 * @brief Header file for the Rank class.
 * 
 * ARCHITECTURAL NOTES:
 * - Rank is a class that represents a rank in Hydragon.
 * - It is used to represent any rank in the game world, such as player ranks, enemy ranks, etc.
 * - It supports interactive features, such as ranking, leveling, and experience. Also supports two-way messaging with other ranks, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct RankInfo : public NodeInfo {
    RankInfo() {
        nodeType = "Gameplay/Rank";
        
        inputs = {
            "rankData",        // Rank data
            "environment",     // Environment data
            "characterData",   // Character data
            "rankState"        // Rank state
        };
        
        outputs = {
            "rankStatus",      // Rank status
            "rankMetrics"      // Rank performance metrics
        };
    }
};

class Rank : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Rank(const RankInfo& info = RankInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addRank(const std::string& rankName, int rankLevel);
    void removeRank(const std::string& rankName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Rank() = default;     // Default destructor
};

} // namespace hd