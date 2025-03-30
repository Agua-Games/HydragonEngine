/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Team.h
 * @brief Header file for the Team class.
 * 
 * ARCHITECTURAL NOTES:
 * - Team is a class that represents a team in Hydragon.
 * - It is used to represent any team in the game world, such as player teams, enemy teams, etc.
 * - It supports interactive features, such as team management, team communication, team objectives, etc. Also supports two-way messaging with other teams, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct TeamInfo : public NodeInfo {
    TeamInfo() {
        nodeType = "Gameplay/Team";
        
        inputs = {
            "teamData",        // Team data
            "environment",     // Environment data
            "characterData",   // Character data
            "teamState"        // Team state
        };
        
        outputs = {
            "teamStatus",      // Team status
            "teamMetrics"      // Team performance metrics
        };
    }
};

class Team : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Team(const TeamInfo& info = TeamInfo())
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
    ~Team() = default;     // Default destructor
};

} // namespace hd
