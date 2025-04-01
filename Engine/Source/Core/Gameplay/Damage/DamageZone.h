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
#include "Wave.h"

namespace hd {

enum class DamageZoneType {
    Fire,
    Lava,
    Acid,
    Electricity,
    Cold,
    Poison,
    Radiation,
    Custom
};

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
            "radius",
            "extents",
            "force",             // Optional force applied to affected entities
            "multiplier",        // Damage multiplier
            "damageChance",      // Chance of dealing damage
            "damageZoneType",
            "damageData", 
            "environment",
            "characterData",
            "damageState"
        };
        
        outputs = {
            "damageStatus",
            "damageMetrics"
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
    vec3 force = 0.0f;
    float multiplier = 1.0f;
    float damageChance = 0.0f;
    DamageZoneType type = DamageZoneType::Fire;

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