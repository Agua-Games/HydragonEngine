/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AchievementManager.h
 * @brief Header file for the AchievementManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - AchievementManager is a singleton class that manages the achievements in the game.
 * - It is responsible for updating the achievements and managing the achievement system.
 * - It uses the Vulkan API for achievement management.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct AchievementManagerInfo : public NodeInfo {
    AchievementManagerInfo() {
        nodeType = "Gameplay/AchievementManager";
        
        inputs = {
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

class AchievementManager : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit AchievementManager(const AchievementManagerInfo& info = AchievementManagerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AchievementManager() = default;     // Default destructor
};

} // namespace hd
