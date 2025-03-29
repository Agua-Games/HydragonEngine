/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DamageZone.h
 * @brief Header file for the DamageZone class.
 * 
 * ARCHITECTURAL NOTES:
 * - DamageZone is a class that represents a damage zone in Hydragon.
 * - It is used to represent any damage zone in the game world, such as fires, lava pools, burning zones, traps, pits, etc.
 * - It supports interactive features, such as detecting and responding to damage. Also supports two-way messaging with other damage zones, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct DamageZoneInfo : public NodeInfo {
    struct DamageType {
        enum class Type {
            Fire,
            Lava,
            Acid,
            Electricity,
            Cold,
            Poison,
            Radiation,
            Custom
        };
    }

    struct DamageData {
        float damageAmount;
        float damageDuration;
        float damageFrequency;
        float damageSeverity;
        float damageResistance;
        float damageEffectiveness;
        float damageEffectDuration;
        float damageEffectFrequency;
        float damageEffectSeverity;
        float damageEffectResistance;
        float damageEffectEffectiveness;
    };

    DamageZoneInfo() {
        nodeType = "Gameplay/DamageZone";
        
        inputs = {
            "damageType",      // Type of damage
            "damageData",      // Damage data
            "environment",     // Environment data
            "characterData",   // Character data
            "damageState"      // Damage state
        };
        
        outputs = {
            "damageStatus",    // Damage status
            "damageMetrics"    // Damage performance metrics
        };
    }
};

/**
 * @brief DamageZone class.
 * Used to apply damage to characters, vehicles, etc, in the game world.
 */
class DamageZone : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit DamageZone(const DamageZoneInfo& info = DamageZoneInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float radius = 0.0f;
    vec3 extents = vec3(0.0f);
    float force = 0.0f;
    float multiplier = 1.0f;
    float detonationChance = 0.0f;
    float fireChance = 0.0f;

    // === Processing ===
    void calculateDamageFromField();        // Calculate damage from physics field. e.g. fire, lava, etc.
    void detectDamage();
    void dealDamage();
    void processNode() override {
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DamageZone() = default;     // Default destructor
};

} // namespace hd