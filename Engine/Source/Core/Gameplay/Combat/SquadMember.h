/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SquadMember.h
 * @brief Header file for the SquadMember class.
 * 
 * ARCHITECTURAL NOTES:
 * - SquadMember is a class that represents a squad member in Hydragon.
 * - It is used to represent any squad member in the game world, such as player squad members, enemy squad members, etc.
 * - It supports interactive features, such as squad member management, squad member communication, squad member objectives, etc. Also supports two-way messaging with other squad members, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Rank.h"
#include "Experience.h"
#include "Equipment.h"

namespace hd {

struct SquadMemberInfo : public NodeInfo {
    SquadMemberInfo() {
        nodeType = "Gameplay/SquadMember";
        
        inputs = {
            "squadMemberData",  // Squad member data
            "environment",      // Environment data
            "characterData",    // Character data
            "squadMemberState"  // Squad member state
        };
        
        outputs = {
            "squadMemberStatus", // Squad member status
            "squadMemberMetrics" // Squad member performance metrics
        };
    }
};

class SquadMember : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SquadMember(const SquadMemberInfo& info = SquadMemberInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    SquadRole role = SquadRole::Other;
    SquadStatus status = SquadStatus::Inactive;
    Rank rank = Rank::Private;
    std::vector<Equipment> equipments;
    float health = 100.0f;
    int ammo = 0;
    float morale = 1.0f;
    Experience experience = Experience();
    std::string squad = "";
    std::string team = "";
    std::string faction = "";
    std::string allegiance = "";
    std::string affiliation = "";
    std::string commander = "";
    std::string leader = "";
    std::string follower = "";
    std::vector<std::string> companion;
    std::vector<std::string> enemy;
    std::vector<std::string> friendly;
    std::vector<std::string> neutral;
    DataTable squadMemberData;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
};

} // namespace hd