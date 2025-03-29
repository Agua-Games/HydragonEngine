/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
    1. RPG Character Progression System:
        - Stat management with modifiers
        - Skill tree with requirements
        - Experience and leveling system
    2. Interactive Dialogue System:
        - Branching dialogue trees
        - Quest triggers in conversations
        - Merchant inventory integration
    3. Quest and Mission Manager:
        - Quest chains with multiple objectives
        - Location and combat-based triggers
        - Reward system
    4. Inventory Crafting System:
        - Recipe database
        - Crafting grid with validation
        - Crafting stations with effects
    5. Interactive Environment System:
        - Loot tables
        - Animation integration
        - Lock and key mechanics
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Character.h"
#include "Inventory.h"
#include "InventoryManager.h"
#include "CraftingManager.h"
#include "RecipeSystem.h"
#include "CraftingStation.h"
#include "InteractionManager.h"
#include "ContainerTable.h"
#include "QuestManager.h"
#include "DialogueManager.h"
#include "DialogueTree.h"
#include "StatsManager.h"
#include "StatModifier.h"
#include "SkillTree.h"
#include "Skill.h"
#include "ExperienceTree.h"
#include "DataCurve.h"

using namespace hd;

// RPG Character Progression System
auto characterSystem = Scene::current()
    .add<Character>("player")
        .connect<StatsManager>("base_stats")
            .addStat("strength", 10)
            .addStat("agility", 8)
            .addStat("intelligence", 12)
            .connect<StatModifier>("equipment_bonus")
                .addStat("strength", 2.0f)
            .connect<StatModifier>("buff_bonus")
                .addStat("agility", 1.5f)
        .connect<SkillTree>("combat_skills")
            .connect<Skill>("power_strike")
                .requiredLevel(5)
                .requiredStat("strength", 15)
                .connect<Ability>("power_strike_ability")
                    .damage(50.0f)
                    .cooldown(8.0f)
            .connect<Skill>("quick_dodge")
                .requiredLevel(8)
                .requiredStat("agility", 20)
        .connect<ExperienceTree>("xp_system")
            .addXPSource("combat", 1.0f)
            .addXPSource("quests", 1.5f)
            .connect<DataCurve>("level_progression")

// Interactive Dialogue System
auto dialogueSystem = Scene::current()
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

// Quest System
auto questSystem = Scene::current()
    .add<QuestManager>("quest_system")
        .connect<QuestCheckpoint>("main_quest")
            .quest("village threat")
            .action("accept")
            .connect<QuestBranch>("quest_progress")
                .quest("village threat")
                .addObjective("investigate_ruins")
                    .required(true)
                    .connect<LocationTrigger>("ruins_area")
                .addObjective("defeat_bandits")
                    .required(true)
                    .count(5)
                    .connect<CombatTrigger>("bandit_defeat")
                        .enemyType("bandit")
                .addReward("xp", 1000)
                .addReward("gold", 500)

// Inventory Crafting System
auto craftingSystem = Scene::current()
    .add<CraftingManager>("crafting")
        .connect<RecipeSystem>("recipe_database")
            .addRecipe("health_potion")
                .addIngredient("herb", 2)
                .addIngredient("water_flask", 1)
                .connect<CraftingResult>("potion_result")
                    .item("health_potion")
                    .quantity(1)
            .addRecipe("magic_sword")
                .addIngredient("iron_ingot", 3)
                .addIngredient("magic_crystal", 1)
                .connect<CraftingResult>("sword_result")
                    .item("magic_sword")
                    .quantity(1)
        .connect<InventoryGrid>("crafting_grid")
            .size(3, 3)
            .connect<ItemValidator>("recipe_validator")
                .validatePlacement(true)
                .validateCombination(true)
        .connect<CraftingStation>("forge")
            .type(CraftingType::Blacksmith)
            .level(2)
            .connect<ParticleSystem>("forge_fx")
                .addEmitter("sparks")
                .addEmitter("smoke");

// Interactive Environment System
auto environmentSystem = Scene::current()
    .add<InteractionManager>("world_interaction")
        .connect<InteractiveObject>("treasure_chest")
            .position({10.0f, 0.0f, 10.0f})
            .connect<LootTable>("chest_loot")
                .addItem("gold", {10, 50})
                .addItem("health_potion", {1, 3})
                .rarity(0.8f)
            .connect<AnimationSystem>("chest_anim")
                .addState("closed", "models/chest_closed.fbx")
                .addState("open", "models/chest_open.fbx")
            .connect<LockSystem>("chest_lock")
                .difficulty(2)
                .connect<ItemRequirement>("key_check")
                    .requireItem("rusty_key");