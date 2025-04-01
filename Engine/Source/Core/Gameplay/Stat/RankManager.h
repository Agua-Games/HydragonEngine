/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RankManager.h
 * @brief Header file for the RankManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - RankManager is a singleton class that manages the ranks in the game.
 * - It is responsible for updating the ranks and managing the rank system.
 * - It uses the Vulkan API for rank management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct RankManagerInfo : public NodeInfo {
    RankManagerInfo() {
        nodeType = "Gameplay/RankManager";
        
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

class RankManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RankManager(const RankManagerInfo& info = RankManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    // TODO: These methods are also implemented in RankMechanics. We must decide where they should go finally, based upon design.
    void processNode() override { }
    void addRank(const std::string& rankName, int rankLevel);
    void removeRank(const std::string& rankName);
    void addLevel(const std::string& levelName, int levelNumber);
    void removeLevel(const std::string& levelName);
    void addExperience(const std::string& experienceName, int experiencePoints);
    void removeExperience(const std::string& experienceName);
    void processRank();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RankManager() = default;     // Default destructor
};

} // namespace hd