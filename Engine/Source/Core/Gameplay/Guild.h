/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Guild.h
 * @brief Header file for the Guild class.
 * 
 * ARCHITECTURAL NOTES:
 * - Guild is a class that represents a guild in Hydragon.
 * - It is used to represent any guild in the game world, such as player guilds, enemy guilds, brotherhoods, mafias, inner circles, etc.
 * - It supports interactive features, such as guild management, guild communication, guild objectives, etc. Also supports two-way messaging with other guilds, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct GuildInfo : public NodeInfo {
    GuildInfo() {
        nodeType = "Gameplay/Guild";
        
        inputs = {
            "guildType",       // Type of guild (player, enemy, etc.)
            "guildData",       // Guild data
            "environment",     // Environment data
            "characterData",   // Character data
            "guildState"       // Guild state
        };
        
        outputs = {
            "guildStatus",     // Guild status
            "guildMetrics"     // Guild performance metrics
        };
    }
};

class Guild : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Guild(const GuildInfo& info = GuildInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addMember(const std::string& memberName);
    void removeMember(const std::string& memberName);
    void addObjective(const std::string& objectiveName);
    void removeObjective(const std::string& objectiveName);
    void addCommunication(const std::string& communicationName);
    void removeCommunication(const std::string& communicationName);
    void addInteraction(const std::string& interactionName);
    void removeInteraction(const std::string& interactionName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Guild() = default;     // Default destructor
};

} // namespace hd