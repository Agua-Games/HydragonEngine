/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Quest.h
 * @brief Header file for the Quest class.
 * 
 * ARCHITECTURAL NOTES:
 * - Quest is a class that represents a quest in Hydragon.
 * - It is used to represent any quest in the game world, such as quests, missions, etc.
 * - It supports interactive features, such as accepting, completing, and failing. Also supports two-way messaging with other quests, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct QuestInfo : public NodeInfo {
    QuestInfo() {
        nodeType = "Gameplay/Quest";
        
        inputs = {
            "questType",        // Type of quest (mission, quest, etc.)
            "questData",        // Quest data
            "environment",      // Environment data
            "characterData",    // Character data
            "questState"        // Quest state
        };
        
        outputs = {
            "questStatus",      // Quest status
            "questMetrics"      // Quest performance metrics
        };
    }
};

class Quest : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Quest(const QuestInfo& info = QuestInfo())
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
    ~Quest() = default;     // Default destructor
};

} // namespace hd
