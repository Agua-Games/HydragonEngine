/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RewardManager.h
 * @brief Header file for the RewardManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - RewardManager is a singleton class that manages the rewards in the game.
 * - It is responsible for updating the rewards and managing the reward system.
 * - It uses the Vulkan API for reward management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct RewardManagerInfo : public NodeInfo {
    RewardManagerInfo() {
        nodeType = "Gameplay/RewardManager";    
        
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

class RewardManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RewardManager(const RewardManagerInfo& info = RewardManagerInfo())
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
    ~RewardManager() = default;     // Default destructor
};

} // namespace hd