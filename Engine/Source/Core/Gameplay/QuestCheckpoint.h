/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file QuestCheckpoint.h
 * @brief Header file for the QuestCheckpoint class.
 * 
 * ARCHITECTURAL NOTES:
 * - QuestCheckpoint is a class that represents (the start of) a quest and at the same time quest checkpoints in Hydragon.
 * - For instance, if used without branches after it, it can represent the starting point of a quest, as it contains all the quest data needed - which it will then pass 
 * down the quest "pipeline", to the next quest checkpoint (quest completed), if it's a simple quest. Example setups to illustrate the basic quest flow:
 * 
 *      (QuestCheckpoint: "Find the Monster") --> (QuestCheckpoint: "Defeat the Monster") --> (QuestCheckpoint: "Bring the Monster's Head to the Quest Giver") --> 
 * (QuestCheckpoint: "Quest Completed")
 * 
 *      (QuestCheckpoint: "Cross the Bridge") --> (QuestBranch: "Bridge is broken". Option1: "Fix the Bridge". Option2: "Cross the River") --> (QuestCheckpoint: "Quest Completed")
 *
 * - It is used to represent any quest checkpoint in the game world, such as quest checkpoints, quest save points, etc.
 * - It supports interactive features, such as saving, loading, and resetting. Also supports two-way messaging with other quest checkpoints, environment, character, etc.
 */
#pragma once
#include <vector>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct Quest {
    std::string name;
    std::string description;
    std::vector<std::string> objectives;
    std::vector<std::string> rewards;
}

enum class QuestAction {
    accept,
    complete,
    fail,
    abandon
};

struct QuestCheckpointInfo : public NodeInfo {
    QuestCheckpointInfo() {
        nodeType = "Gameplay/QuestCheckpoint";
        
        inputs = {
            "questData",  // Quest data
            "questState"  // Quest state
        };
        
        outputs = {
            "questData",  // Quest data
            "questStatus",  // Quest status
            "questMetrics"  // Quest performance metrics
        };
    }
};

class QuestCheckpoint : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit QuestCheckpoint(const QuestCheckpointInfo& info = QuestCheckpointInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    Quest quest = {};
    Quest.name = "Quest";
    Quest.description = "Quest description";
    Quest.objectives = {"Objective 1", "Objective 2", "Objective 3"};
    Quest.rewards = {"Reward 1", "Reward 2", "Reward 3"};
    QuestAction questAction = QuestAction::accept;
    DataTable questData = DataTable();

    // === Processing ===
    void processNode() override {
 
    }
    void addObjective(const std::string& objectiveName);
    void removeObjective(const std::string& objectiveName);
    void addReward(const std::string& rewardName);
    void removeReward(const std::string& rewardName);
    void processQuestAction(QuestAction action);
    void processQuest();
    void saveQuest();
    void loadQuest();
    void resetQuest();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~QuestCheckpoint() = default;     // Default destructor
};

} // namespace hd
