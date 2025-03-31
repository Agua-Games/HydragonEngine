/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TeamMechanics.h
 * @brief Header file for the TeamMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - TeamMechanics is a class that represents team mechanics in Hydragon.
 * - It is used to represent any team mechanics in the game world, such as team management, team communication, team objectives, etc.
 * - It supports interactive features, such as team management, team communication, team objectives, etc. Also supports two-way messaging with other teams, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Team.h"
#include "TeamManager.h"

namespace hd {

struct TeamMechanicsInfo : public NodeInfo {
    TeamMechanicsInfo() {
        nodeType = "Gameplay/TeamMechanics";
        
        inputs = {
            "teamMechanicsData",// Team mechanics data
            "environment",      // Environment data
            "characterData",    // Character data
            "teamMechanicsState"// Team mechanics state
        };
        
        outputs = {
            "teamMechanicsStatus",// Team mechanics status
            "teamMechanicsMetrics"// Team mechanics performance metrics
        };
    }
};

class TeamMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit TeamMechanics(const TeamMechanicsInfo& info = TeamMechanicsInfo())
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
    ~TeamMechanics() = default;     // Default destructor
};

} // namespace hd
