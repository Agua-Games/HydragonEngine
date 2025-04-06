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
// Example 1: Quest system setup with objectives and rewards
// Quest: Find the lost children (simple quest)
auto& rescueQuest = questManager.connect<Quest>("rescueChildren")
    .property("title", "Find the Lost Children")
    .property("description", "Two children have gone missing from the village")
    
    // Checkpoint 1: Talk to villager
    .connect<QuestCheckpoint>("talkToVillager")
        .property("position", Vector3{10.0f, 0.0f, 15.0f})
        .property("description", "Talk to the farmer about the missing children")
        .property("isInteractionObjective", true)
    
    // Checkpoint 2: Find first child in fields
    .connect<QuestCheckpoint>("findChildInFields")
        .property("position", Vector3{50.0f, 0.0f, 30.0f})
        .property("description", "Find the first child in the wheat fields")
        .property("isLocationObjective", true)
    
    // Checkpoint 3: Find second child in cave
    .connect<QuestCheckpoint>("findChildInCave")
        .property("position", Vector3{-20.0f, -5.0f, 60.0f})
        .property("description", "Find the second child in the cave")
        .property("isLocationObjective", true)
    
    // Checkpoint 4: Defeat monster
    .connect<QuestCheckpoint>("defeatCaveMonster")
        .property("position", Vector3{-25.0f, -10.0f, 70.0f})
        .property("description", "Defeat the cave monster")
        .property("isCombatObjective", true)
    
    // Checkpoint 5: Return to village
    .connect<QuestCheckpoint>("returnToVillage")
        .property("position", Vector3{0.0f, 0.0f, 0.0f})
        .property("description", "Return the children to the village")
        .property("isLocationObjective", true);