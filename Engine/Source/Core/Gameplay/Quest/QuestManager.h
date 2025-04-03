/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file QuestManager.h
 * @brief Header file for the QuestManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - QuestManager is a singleton class that manages the quests in the game.
 * - It is responsible for updating the quests and managing the quest system.
 * - It uses the Vulkan API for quest management.
 */
#pragma once

#include "Node.h"
#include "DataTable.h"

namespace hd {

struct QuestManagerInfo : public NodeInfo {
    QuestManagerInfo() {
        nodeType = "Gameplay/QuestManager";
        
        inputs = {
            "questData",        // Array of quests data
            "questState",    // Current state of the quest data
            "questSpeed",    // Speed of the quest data
            "questLoop"      // Whether the quest data should loop
        };
        
        outputs = {
            "activeQuests",  // Active quests
            "questMetrics"   // Performance and quality metrics
        };
    }
};

class QuestManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit QuestManager(const QuestManagerInfo& info = QuestManagerInfo())
        : Node(info) {}   // Default constructor for QuestManager class
    initialize() override {}
    load() override {}

    // Set default values
    float maxActiveQuests = 10;
    bool dynamicGeneration = false;
    int questDensity = 5;

    // === Processing ===
    void processNode() override {
    }
    void addQuest(const std::string& questName);
    void removeQuest(const std::string& questName);
    void processQuest();
    void loadQuestTemplate(const std::string& templatePath);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~QuestManager() = default;     // Default destructor
};

} // namespace hd

