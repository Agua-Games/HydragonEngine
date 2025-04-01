/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AuraEffect.h
 * @brief Header file for the AuraEffect class.
 * 
 * ARCHITECTURAL NOTES:
 * - AuraEffect is a class that represents an aura effect in Hydragon.
 * - It is used to represent any aura effect in the game world, such as spells, powers, magic shields, force fields, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other spells, environment, character, etc.
 * - It makes use of WavePhysics, MagicField, for spell effects which support energy transfer and propagation - thermal, kinetic, etc.
 */
#pragma once
#include "Effect.h"
#include "WavePhysics.h"
#include "PhysicsFields.h"
#include "DataTable.h"

namespace hd {
    
struct AuraEffectInfo : public EffectInfo {
    AuraEffectInfo() {
        nodeType = "Gameplay/AuraEffect";
        
        inputs = {
            "type",            // Type of aura effect (spell, power, etc.)
            "intensity",       // Intensity of the aura effect
            "duration",        // Duration of the aura effect
            "targeting",       // Targeting type of the aura effect (single, area, etc.)
            "effectData",      // Effect data (stats, etc.)
            "environment",     // Environment data (stats, etc.)
            "characterData",   // Character data (stats, etc.)
            "effectState"      // Effect state (stats, etc.)
        };
        
        outputs = {
            "effectStatus",    // Effect status (stats, etc.)
            "effectMetrics"    // Effect performance metrics (stats, etc.)
        };
    }
};

class AuraEffect : public Effect {
public:
    // === Allocation, Initialization, Loading ===
    explicit AuraEffect(const AuraEffectInfo& info = AuraEffectInfo())
        : Effect(info) {}    // Default constructor
    initialize() override {}
    load() override {}

    // Set default values
    std::string type = "spell";
    float intensity = 0.0f;
    float radius = 0.0f;
    float duration = 0.0f;
    std::string targeting = "single";

    // === Processing ===
    void processNode() override { }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AuraEffect() = default;     // Default destructor
};

} // namespace hd
