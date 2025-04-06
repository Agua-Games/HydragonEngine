/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatMechanics.h
 * @brief Header file for the CombatMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatMechanics is a class that represents combat mechanics in Hydragon.
 * - It is used to represent any combat mechanics in the game world, such as damage, health, etc.
 * - It supports interactive features, such as dealing damage, taking damage, and healing. Also supports two-way messaging with other combat mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Ability.h"
#include "PhysicsFields.h"

namespace hd {

enum class CombatMechanicsType {
    Melee,
    Ranged,
    Magic,
    Other
};

struct ReinforcementData {
    std::string name;
    float cooldown;
    float damage;
};

struct CombatMechanicsInfo : public NodeInfo {
    CombatMechanicsInfo() {
        nodeType = "Gameplay/CombatMechanics";
        
        inputs = {
            "combatData",        // Combat data
            "environment",       // Environment data
            "characterData",     // Character data
            "combatState"        // Combat state
        };
        
        outputs = {
            "combatStatus",      // Combat status
            "combatMetrics"      // Combat performance metrics
        };
    }
};

class CombatMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit CombatMechanics(const CombatMechanicsInfo& info = CombatMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    std::vector<ReinforcementData> reinforcements;
    ReinforcementData.name = "none";
    ReinforcementData.cooldown = 0.0f;
    ReinforcementData.damage = 0.0f;
    DataTable combatMechanicsData;

    // TODO: Move melee, raged and magic variables to a struct
    // Melee
    float meleeRange = 0.0f;                 // Melee range of the character (e.g., sword swing range)
    float meleeRadius = 0.0f;                // Melee radius of the character (e.g., sword swing radius)
    float meleeForce = 0.0f;                  // Melee force of the character (e.g., sword swing force)
    float meleeMomentum = 0.0f;               // Melee momentum of the character (e.g., sword swing momentum)
    float meleeDuration = 0.0f;               // Melee duration of the character (e.g., sword swing duration)
    float meleeRangeFalloff = 0.0f;          // Melee range falloff of the character (e.g., sword swing range falloff)
    float meleeStaggerChance = 0.0f;         // Melee stagger chance of the character (e.g., sword swing stagger chance)
    float meleeCriticalHitChance = 0.0f;     // Melee critical hit chance of the character (e.g., sword swing critical hit chance)
    float meleeDamage = 0.0f;                // Melee damage of the character (e.g., sword swing damage)
    float meleeAccuracy = 0.0f;              // Melee accuracy of the character (e.g., sword swing accuracy)
    float meleeAccuracyFalloff = 0.0f;       // Melee accuracy falloff of the character (e.g., sword swing accuracy falloff)
    float meleeSpeed = 0.0f;                 // Melee speed of the character (e.g., sword swing speed)
    float meleeCooldown = 0.0f;              // Melee cooldown of the character (e.g., sword swing cooldown)
    float meleeLockTime = 0.0f;              // Melee lock time of the character (e.g., sword swing lock time)

    // Ranged
    float rangedRange = 0.0f;
    float rangedRadius = 0.0f;
    float rangedForce = 0.0f;
    float rangedMomentum = 0.0f;
    float rangedDuration = 0.0f;
    float rangedRangeFalloff = 0.0f;
    float rangedStaggerChance = 0.0f;
    float rangedCriticalHitChance = 0.0f;
    float rangedDamage = 0.0f;
    float rangedAccuracy = 0.0f;
    float rangedAccuracyFalloff = 0.0f;
    float rangedSpeed = 0.0f;
    float rangedCooldown = 0.0f;
    float rangedLockTime = 0.0f;
    float rangedProjectileSpeed = 0.0f;

    // Magic
    float magicRange = 0.0f;
    float magicRadius = 0.0f;
    float magicForce = 0.0f;
    float magicMomentum = 0.0f;
    float magicDuration = 0.0f;
    float magicRangeFalloff = 0.0f;
    float magicStaggerChance = 0.0f;
    float magicCriticalHitChance = 0.0f;
    float magicDamage = 0.0f;
    float magicAccuracy = 0.0f;
    float magicAccuracyFalloff = 0.0f;
    float magicSpeed = 0.0f;
    float magicCooldown = 0.0f;
    float magicLockTime = 0.0f;
    float magicProjectileSpeed = 0.0f;
    float manaCost = 0.0f;
    float castTime = 0.0f;

    // === Processing ===
    void processNode() override { }
    void addReinforcement(const std::string& reinforcementName);
    void removeReinforcement(const std::string& reinforcementName);
    void processReinforcement();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CombatMechanics() = default;     // Default destructor
};

} // namespace hd
