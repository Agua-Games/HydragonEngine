/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RankMechanics.h
 * @brief Header file for the RankMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - RankMechanics is a class that represents rank mechanics in Hydragon.
 * - It is used to represent any rank mechanics in the game world, such as ranks, levels, experience, etc.
 * - It supports interactive features, such as ranking, leveling, and experience. Also supports two-way messaging with other rank mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct RankMechanicsInfo : public NodeInfo {
    RankMechanicsInfo() {
        nodeType = "Gameplay/RankMechanics";
        
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

class RankMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RankMechanics(const RankMechanicsInfo& info = RankMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
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
    ~RankMechanics() = default;     // Default destructor
};

} // namespace hd