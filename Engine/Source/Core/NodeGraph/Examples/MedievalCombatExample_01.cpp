/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Medieval combat system setup with knights, archers, and magic
 */
#include "GameplayHeaders.h"
#include "Node.h"

using namespace hd;

// Create a medieval combat system with knights, archers, and magic
void setupMedievalCombatSystem() {
    // Create the main gameplay manager
    auto& gameplayManager = Engine::get().create<GameplayManager>("medievalCombat")
        // Configure basic gameplay settings
        .property("maxPlayers", 16)
        .property("respawnTime", 5.0f)
        .property("timeLimit", 600.0f)  // 10 minutes
        .property("overtime", true)
        .property("killCam", true);
    
    // Set up the gameplay mode as Team Deathmatch
    gameplayManager.gameplayMode = GameplayData::GameplayMode::TeamDeathmatch;
    
    // Create a gameplay area (battlefield)
    auto& battlefield = gameplayManager.connect<GameplayArea>("castleGrounds")
        .property("type", GameplayAreaType::Battlefield);
    
    // Add respawn points to the battlefield
    battlefield.connect<PlayerSpawner>("teamASpawn")
        .property("position", Vector3{-100.0f, 0.0f, 0.0f})
        .property("team", 0);
    
    battlefield.connect<PlayerSpawner>("teamBSpawn")
        .property("position", Vector3{100.0f, 0.0f, 0.0f})
        .property("team", 1);
    
    // Create a combat manager to handle combat mechanics
    auto& combatManager = gameplayManager.connect<CombatManager>("medievalCombat")
        .property("friendlyFire", false)
        .property("damageMultiplier", 1.0f);
    
    // Add different combat mechanics
    auto& meleeCombat = combatManager.connect<CombatMechanics>("meleeCombat")
        .property("range", 2.0f)
        .property("staggerChance", 0.3f)
        .property("criticalHitChance", 0.1f);
    
    auto& rangedCombat = combatManager.connect<CombatMechanics>("rangedCombat")
        .property("maxRange", 50.0f)
        .property("accuracyFalloff", 0.5f)
        .property("projectileSpeed", 20.0f);
    
    auto& magicCombat = combatManager.connect<CombatMechanics>("magicCombat")
        .property("manaCost", 25.0f)
        .property("castTime", 1.5f)
        .property("cooldown", 5.0f);
    
    // Create character classes
    auto& knightClass = combatManager.connect<CharacterClass>("knight")
        .property("health", 150.0f)
        .property("armor", 50.0f)
        .property("speed", 5.0f);
    
    // Add abilities to the knight
    knightClass.connect<CombatAbility>("swordSlash")
        .property("damage", 25.0f)
        .property("cooldown", 1.0f)
        .property("range", 2.0f)
        .connect<StatusEffect>("bleeding")
            .property("duration", 3.0f)
            .property("tickDamage", 5.0f);
    
    knightClass.connect<CombatAbility>("shieldBash")
        .property("damage", 15.0f)
        .property("cooldown", 5.0f)
        .property("stunDuration", 1.5f);
    
    // Create archer class
    auto& archerClass = combatManager.connect<CharacterClass>("archer")
        .property("health", 100.0f)
        .property("armor", 20.0f)
        .property("speed", 7.0f);
    
    // Add abilities to the archer
    archerClass.connect<CombatAbility>("powerShot")
        .property("damage", 35.0f)
        .property("cooldown", 3.0f)
        .property("range", 40.0f)
        .property("chargeTime", 1.0f);
    
    archerClass.connect<CombatAbility>("multiShot")
        .property("damage", 15.0f)
        .property("cooldown", 8.0f)
        .property("projectileCount", 3)
        .property("spreadAngle", 15.0f);
    
    // Create mage class
    auto& mageClass = combatManager.connect<CharacterClass>("mage")
        .property("health", 80.0f)
        .property("armor", 10.0f)
        .property("mana", 100.0f)
        .property("speed", 6.0f);
    
    // Add abilities to the mage
    mageClass.connect<CombatAbility>("fireball")
        .property("damage", 40.0f)
        .property("manaCost", 25.0f)
        .property("cooldown", 4.0f)
        .property("areaOfEffect", 3.0f)
        .connect<StatusEffect>("burning")
            .property("duration", 4.0f)
            .property("tickDamage", 8.0f);
    
    mageClass.connect<CombatAbility>("frostNova")
        .property("damage", 20.0f)
        .property("manaCost", 40.0f)
        .property("cooldown", 10.0f)
        .property("radius", 8.0f)
        .connect<StatusEffect>("frozen")
            .property("duration", 2.0f)
            .property("movementPenalty", 0.5f);
    
    // Set up score manager
    auto& scoreManager = gameplayManager.scoreManager
        .property("killPoints", 10)
        .property("assistPoints", 5)
        .property("deathPenalty", -5)
        .property("objectivePoints", 20);
    
    // Add a quest system for additional objectives
    auto& questManager = gameplayManager.connect<QuestManager>("battleQuests")
        .property("maxActiveQuests", 3)
        .property("questDensity", 5);
    
    // Add a quest for capturing strategic points
    questManager.connect<QuestCheckpoint>("capturePoints")
        .property("title", "Capture Strategic Points")
        .property("description", "Capture and hold the castle towers")
        .property("rewardPoints", 50)
        .connect<QuestCheckpoint>("northTower")
            .property("position", Vector3{0.0f, 10.0f, -50.0f})
            .property("captureTime", 10.0f)
        .connect<QuestCheckpoint>("southTower")
            .property("position", Vector3{0.0f, 10.0f, 50.0f})
            .property("captureTime", 10.0f);
}