/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TeamManager.h
 * @brief Header file for the TeamManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - TeamManager is a singleton class that manages the teams in the game.
 * - It is responsible for managing the teams in the game.
 * - It supports interactive features, such as team management, team communication, team objectives, etc. Also supports two-way messaging with other teams, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Team.h"

namespace hd {
struct TeamManagerInfo : public NodeInfo {
    TeamManagerInfo() {
        nodeType = "Gameplay/TeamManager"; 
        
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

class TeamManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit TeamManager(const TeamManagerInfo& info = TeamManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addTeam(const std::string& teamName);
    void removeTeam(const std::string& teamName);
    void processTeam();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TeamManager() = default;     // Default destructor
};

} // namespace hd