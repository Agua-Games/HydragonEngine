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

#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"
#include "Core/Gameplay/Character.h"
#include "Core/Gameplay/Inventory.h"
#include "Core/Gameplay/Quest.h"
#include "Core/Gameplay/Dialogue.h"

using namespace hd;

// RPG Character Progression System
auto characterSystem = Scene::current()
    .add<Character>("player")
        .connect<StatsSystem>("base_stats")
            .addStat("strength", 10)
            .addStat("agility", 8)
            .addStat("intelligence", 12)
            .connect<StatModifier>("equipment_bonus")
                .addModifier("strength", 2.0f, ModifierType::Multiplicative)
            .connect<StatModifier>("buff_bonus")
                .addModifier("agility", 1.5f, ModifierType::Additive)
        .connect<SkillTree>("combat_skills")
            .addSkill("power_strike")
                .requiredLevel(5)
                .requiredStat("strength", 15)
                .connect<Ability>("power_strike_ability")
                    .damage(50.0f)
                    .cooldown(8.0f)
            .addSkill("quick_dodge")
                .requiredLevel(8)
                .requiredStat("agility", 20)
        .connect<ExperienceSystem>("xp_system")
            .addXPSource("combat", 1.0f)
            .addXPSource("quests", 1.5f)
            .connect<LevelSystem>("level_progression")
                .setLevelCurve({
                    {1, 1000},
                    {2, 2500},
                    {3, 4500}
                });

// Interactive Dialogue System
auto dialogueSystem = Scene::current()
    .add<DialogueManager>("npc_dialogue")
        .connect<DialogueTree>("merchant_conversation")
            .addNode("greeting")
                .text("Welcome to my shop!")
                .connect<DialogueChoice>("shop_choice")
                    .addOption("Show me your wares", "open_shop")
                    .addOption("Tell me about your travels", "travel_story")
                    .addOption("Goodbye", "exit")
            .addNode("open_shop")
                .connect<InventorySystem>("merchant_inventory")
                    .addItem("health_potion", 5)
                    .addItem("magic_scroll", 3)
            .addNode("travel_story")
                .connect<QuestTrigger>("hidden_quest")
                    .questId("lost_artifact")
                    .condition("first_time");

// Quest and Mission Manager
auto questSystem = Scene::current()
    .add<QuestManager>("quest_system")
        .connect<QuestChain>("main_quest")
            .addQuest("village_threat")
                .addObjective("investigate_ruins")
                    .required(true)
                    .connect<LocationTrigger>("ruins_area")
                        .radius(10.0f)
                        .position({100.0f, 0.0f, 100.0f})
                .addObjective("defeat_bandits")
                    .required(true)
                    .count(5)
                    .connect<CombatTrigger>("bandit_defeat")
                        .enemyType("bandit")
                .addReward("xp", 1000)
                .addReward("gold", 500)
            .connect<QuestState>("quest_progress")
                .trackObjective("investigate_ruins")
                .trackObjective("defeat_bandits");

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