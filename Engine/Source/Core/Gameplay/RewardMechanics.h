/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RewardMechanics.h
 * @brief Header file for the RewardMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - RewardMechanics is a class that represents reward mechanics in Hydragon.
 * - It is used to represent any reward mechanics in the game world, such as rewards, loot, etc.
 * - It supports interactive features, such as looting, rewarding, and upgrading. Also supports two-way messaging with other reward mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct RewardMechanicsInfo : public NodeInfo {
    RewardMechanicsInfo() {
        nodeType = "Gameplay/RewardMechanics";
        
        inputs = {
            "rewardData",        // Reward data
            "environment",       // Environment data
            "characterData",     // Character data
            "rewardState"        // Reward state
        };
        
        outputs = {
            "rewardStatus",      // Reward status
            "rewardMetrics"      // Reward performance metrics
        };
    }
};

class RewardMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RewardMechanics(const RewardMechanicsInfo& info = RewardMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addReward(const std::string& rewardName);
    void removeReward(const std::string& rewardName);
    void processReward();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RewardMechanics() = default;     // Default destructor
};

} // namespace hd