/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Quest system setup with objectives and rewards
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "QuestManager.h"
#include "QuestCheckpoint.h"
#include "QuestBranch.h"
#include "LocationTrigger.h"
#include "CombatTrigger.h"

using namespace hd;

// Example 1: Quest system setup with objectives and rewards
auto& questSystem = Scene::current()
    .add<QuestManager>("quest_system")
        .connect<QuestCheckpoint>("quest_start")
            .quest("village_threat")
            .action("accept")
            .connect<QuestBranch>("quest_progress")
                .quest("village threat")
                .addObjective("investigate_ruins")
                    .required(true)
                    .connect<LocationTrigger>("ruins_area")

// Example 2: Quest system with branching objectives and rewards
auto& branchingQuest = Scene::current()
    .add<QuestManager>("quest_system")
        .connect<QuestCheckpoint>("quest_start")
            .quest("find the hermit")
            .connect<QuestBranch>("quest_progress")
                .quest("find the hermit")
                .addObjective("investigate_ruins")
                    .required(true)
                    .connect<LocationTrigger>("ruins_area")
                .addObjective("defeat_bandits")
                    .required(true)
                    .count(5)
                    .connect<CombatTrigger>("bandit_defeat")
                        .enemyType("bandit")
                .addReward("xp", 1000)


