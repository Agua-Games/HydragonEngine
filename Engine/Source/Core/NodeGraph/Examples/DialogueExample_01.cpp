/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Dialogue system setup with branching conversations
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DialogueManager.h"
#include "DialogueCheckpoint.h"
#include "DialogueBranch.h"
#include "Inventory.h"
#include "QuestManager.h"
#include "QuestCheckpoint.h"
#include "QuestBranch.h"

using namespace hd;

// Example 1: Dialogue system setup with branching conversations
auto& dialogueSystem = Scene::current()
    .add<DialogueManager>("npc_dialogue")
        .connect<DialogueCheckpoint>("merchant_conversation")
            .connect<DialogueBranch>("greeting")
                .text("Welcome to my shop!")
                .addOption("Show me your wares", "open_shop")
                .addOption("Tell me about your travels", "travel_story")
                    .connect<DialogueBranch>("Amazing travel story", "travel_story")
                        .text("I've seen many wonders...")
                        .addOption("Thank you", "exit")
                        .addOption("That's interesting", "exit")
                .addOption("Goodbye", "exit")
            .connect<DialogueBranch>("open_shop")
                .text("Here are my items...")
                .addOption("Buy something", "buy_item")
                    .connect<Inventory>("merchant_inventory")
                        .addCategory("weapons")
                        .addCategory("armor")
                .addOption("Exit", "exit")

// Example 2: Dialogue system with quest triggers
auto& questDialogue = Scene::current()
    .add<DialogueManager>("quest_dialogue")
        .connect<DialogueCheckpoint>("quest_giver")
            .connect<DialogueBranch>("quest_offer")
                .text("I have a quest for you...")
                .addOption("Accept", "quest_accepted")
                    .connect<QuestCheckpoint>("quest_start")
                        .quest("village_threat")
                        .action("accept")
                .addOption("Decline", "exit");