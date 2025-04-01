/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ScoreManager.h
 * @brief Header file for the ScoreManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - ScoreManager is a singleton class that manages the scores in the game.
 * - It is responsible for updating the scores and managing the score system.
 * - It uses the Vulkan API for score management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct Score {
    std::string name;
    int value;
    int rank;
};

struct ScoreManagerInfo : public NodeInfo {
    ScoreManagerInfo() {
        nodeType = "Gameplay/ScoreManager";
        
        inputs = {
            "scoreData",        // Score data
            "environment",      // Environment data
            "characterData",    // Character data
            "scoreState"        // Score state
        };
        
        outputs = {
            "scoreStatus",      // Score status
            "scoreMetrics"      // Score performance metrics
        };
    }
};

class ScoreManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ScoreManager(const ScoreManagerInfo& info = ScoreManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void addScore(const std::string& scoreName, int scoreValue);
    void removeScore(const std::string& scoreName);
    void processScore();
    void update();

    // Set default values
    DataTable scoreData;
    Score score;

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ScoreManager() = default;     // Default destructor
};

} // namespace hd
