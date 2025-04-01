/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file QuestBranch.h
 * @brief Header file for the QuestBranch class.
 * 
 * ARCHITECTURAL NOTES:
 * - QuestBranch is a class that represents a quest branch in Hydragon.
 * - Quest branch is a versatile self contained quest "atom" (a statement by a character). At the same time it can be used as a single statement by a character,
 * or a monologue, it can be used as a fork leading to quest choices (its outputs), and thus chained together to form a quest tree.
 * - It works together with the QuestCheckpoint to form a complete modular versatile quest system with the ability to store checkpoint flags for gameplay events
 * (e.g. "information sent to player") and send information exchanged in the quest to external nodes, like the MemoryBank.
 * - It is used to represent any quest branch in the game world, such as player quest branches, enemy quest branches, etc.
 * - It is meant to be used in conjunction with the QuestCheckpoint to form a complete quest system. Example setups to illustrate the basic quest flow:
 * 
 *      (QuestCheckpoint: "Find the Monster") --> (QuestCheckpoint: "Defeat the Monster") --> (QuestCheckpoint: "Bring the Monster's Head to the Quest Giver") --> 
 * (QuestCheckpoint: "Quest Completed")
 * 
 *      (QuestCheckpoint: "Cross the Bridge") --> (QuestBranch: "Bridge is broken". Option1: "Fix the Bridge". Option2: "Cross the River") --> (QuestCheckpoint: "Quest Completed")
 *
 * - It supports interactive features, such as branching, decision making, and quest flow. Also supports two-way messaging with other quest branches, environment, character, etc.
 * 
 *  @todo Define if the quest, objectives and rewards names should be directly stored as strings with spaces, capital letter, etc, or if they should have an internal name
 * and a display name.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "QuestCheckpoint.h"

namespace hd {

struct QuestBranchInfo : public NodeInfo {
    QuestBranchInfo() {
        nodeType = "Gameplay/QuestBranch";
        
        inputs = {
            "questBranchData",  // Quest branch data
            "questBranchState"  // Quest branch state
        };
        
        outputs = {
            "questBranchStatus",  // Quest branch status
            "questBranchMetrics"  // Quest branch performance metrics
        };
    }
};

class QuestBranch : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit QuestBranch(const QuestBranchInfo& info = QuestBranchInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    Quest quest = {};
    Quest.name = "Quest";
    Quest.description = "Quest description";
    Quest.objectives = {"Objective 1", "Objective 2", "Objective 3"};
    Quest.rewards = {"Reward 1", "Reward 2", "Reward 3"};
    DataTable questData = DataTable();

    // === Processing ===
    void processNode() override {
 
    }
    void addObjective(const std::string& objectiveName);
    void removeObjective(const std::string& objectiveName);
    void addReward(const std::string& rewardName);
    void removeReward(const std::string& rewardName);
    void addOption(const std::string& optionName);
    void removeOption(const std::string& optionName);
    void processQuest();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~QuestBranch() = default;     // Default destructor
};

} // namespace hd
