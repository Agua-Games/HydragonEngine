/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file GuildMechanics.h
 * @brief Header file for the GuildMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - GuildMechanics is a class that represents guild mechanics in Hydragon.
 * - It is used to represent any guild mechanics in the game world, such as guild management, guild communication, guild objectives, etc.
 * - It supports interactive features, such as guild management, guild communication, guild objectives, etc. Also supports two-way messaging with other guild mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Guild.h"

namespace hd {

class GuildMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit GuildMechanics(const GuildMechanicsInfo& info = GuildMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addGuild(const std::string& guildName);
    void removeGuild(const std::string& guildName);
    void processGuild();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~GuildMechanics() = default;     // Default destructor
};

