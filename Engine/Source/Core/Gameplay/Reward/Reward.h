/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Reward.h
 * @brief Header file for the Reward class.
 * 
 * ARCHITECTURAL NOTES:
 * - Reward is a class that represents a reward in Hydragon.
 * - It is used to represent any reward in the game world, such as gold, experience, etc.
 * - It supports interactive features, such as rewarding, upgrading, and looting. Also supports two-way messaging with other rewards, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct RewardInfo : public NodeInfo {
    RewardInfo() {
        nodeType = "Gameplay/Reward";
        
        inputs = {
            "rewardType",      // Type of reward (gold, experience, etc.)
            "rewardData",      // Reward data
            "environment",     // Environment data
            "characterData",   // Character data
            "rewardState"      // Reward state
        };
        
        outputs = {
            "rewardStatus",    // Reward status
            "rewardMetrics"    // Reward performance metrics
        };
    }
};

class Reward : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Reward(const RewardInfo& info = RewardInfo())
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
    ~Reward() = default;     // Default destructor
};

} // namespace hd