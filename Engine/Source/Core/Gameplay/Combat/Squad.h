/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Squad.h
 * @brief Header file for the Squad class.
 * 
 * ARCHITECTURAL NOTES:
 * - Squad is a class that represents a squad in Hydragon.
 * - It is used to represent any squad in the game world, such as player squads, enemy squads, etc.
 * - It supports interactive features, such as squad management, squad communication, squad objectives, etc. Also supports two-way messaging with other squads, environment, character, etc.
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

struct SquadInfo : public NodeInfo {
    SquadInfo() {
        nodeType = "Gameplay/Squad";
        
        inputs = {
            "squadData",        // Squad data
            "environment",      // Environment data
            "characterData",    // Character data
            "squadState"        // Squad state
        };
        
        outputs = {
            "squadStatus",      // Squad status
            "squadMetrics"      // Squad performance metrics
        };
    }
};

class Squad : public Node {
public:
    // === Structure Definitions ===
    enum class SquadRole {
        Leader,
        Support,
        Overwatch,
        Medic,
        Engineer,
        Sniper,
        Scout,
        Demolition,
        Other
    };

    enum class SquadStatus {
        Active,
        Inactive,
        Deployed,
        Engaged,
        Retreating,
        Reinforcing,
        Other
    };

    enum class SquadType {
        Infantry,
        Vehicle,
        Air,
        Naval,
        Other
    };

    enum class FormationType {
        Line,
        Wedge,
        Column,
        Skirmish,
        Other
    };

    // TODO: Maybe this struct is too complex. Optimize after actual usage.
    struct SquadMember {
        std::string name;
        SquadRole role;
        SquadStatus status;
        Rank rank;
        std::vector<Equipment> equipments;
        float health;
        int ammo;
        float morale;
        Experience experience;
        std::string squad;
        std::string team;
        std::string faction;
        std::string allegiance;
        std::string affiliation;

        std::string commander;
        std::string leader;
        std::string follower;
        std::vector<std::string> companion;
        std::vector<std::string> enemy;
        std::vector<std::string> friendly;
        std::vector<std::string> neutral;
    };     // struct SquadMember

    // === Allocation, Initialization, Loading ===
    explicit Squad(const SquadInfo& info = SquadInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    FormationType formationType = FormationType::Line;
    float formationSpacing = 0.0f;

    // === Processing ===
    void processNode() override { }
    void processSquad();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Squad() = default;     // Default destructor
};

} // namespace hd
