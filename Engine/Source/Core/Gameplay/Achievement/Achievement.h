/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Achievement.h
 * @brief Header file for the Achievement class.
 * 
 * ARCHITECTURAL NOTES:
 * - Achievement is a class that represents an achievement in Hydragon.
 * - It is used to represent any achievement in the game world, such as quests, challenges, etc.
 * - It supports interactive features, such as completing, upgrading, and unlocking. Also supports two-way messaging with other achievements, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

enum class AchievementType {
    quest,
    challenge,
    custom
};

struct AchievementInfo : public NodeInfo {
    AchievementInfo() {
        nodeType = "Gameplay/Achievement";
        
        inputs = {
            "achievementType",  // Type of achievement (quest, challenge, etc.)
            "achievementData",  // Achievement data
            "environment",      // Environment data
            "characterData",    // Character data
            "achievementState"  // Achievement state
        };
        
        outputs = {
            "achievementStatus",  // Achievement status
            "achievementMetrics"  // Achievement performance metrics
        };
    }
};

class Achievement : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Achievement(const AchievementInfo& info = AchievementInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    AchievementType type = AchievementType::custom;
    int level = 0;
    int requiredLevel = 0;
    int progress = 0;
    int requiredProgress = 0;
    int reward = 0;
    int rewardType = 0;
    int rewardAmount = 0;
    int rewardRarity = 0;
    int rewardQuantity = 0;
    int rewardDuration = 0;
    int rewardDurationType = 0;

    // === Processing ===
    void processNode() override { }
    void update();
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Achievement() = default;     // Default destructor
};

} // namespace hd
