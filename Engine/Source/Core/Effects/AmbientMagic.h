/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AmbientMagic.h
 * @brief Header file for the AmbientMagic class.
 * 
 * ARCHITECTURAL NOTES:
 * - AmbientMagic is a class that represents an ambient magic effect in the engine's node graph.
 * - It is used to represent any ambient magic effect in the game world, such as spells, powers, etc.
 * - It supports interactive features, such as casting, targeting, and cooldowns. Also supports two-way messaging with other spells, environment, character, etc.
 * - It makes use of WavePhysics, MagicField, for spell effects which support energy transfer and propagation - thermal, kinetic, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "Effect.h"

struct AmbientMagicInfo : public EffectInfo {
    AmbientMagicInfo() {
        nodeType = "Effects/AmbientMagic";
        
        inputs = {
            "intensity",       // Intensity of the ambient magic effect
            "duration",        // Duration of the ambient magic effect
            "targeting",       // Targeting type of the ambient magic effect (single, area, etc.)
            "ambientMagicData",// Ambient magic data (stats, etc.)
            "environment",     // Environment data (stats, etc.)
            "characterData",   // Character data (stats, etc.)
            "ambientMagicState"// Ambient magic state (stats, etc.)
        };
        
        outputs = {
            "ambientMagicStatus",// Ambient magic status (stats, etc.)
            "ambientMagicMetrics"// Ambient magic performance metrics (stats, etc.)
        };
    }
};

class AmbientMagic : public Effect {
public:
    // === Allocation, Initialization, Loading ===
    explicit AmbientMagic(const AmbientMagicInfo& info = AmbientMagicInfo())
        : Effect(info) {}
    initialize() override {}
    load() override {}
    
    // Set default values
    float intensity = 0.0f;              // Intensity of the ambient magic effect. If 0, it is not active.
    float duration = 0.0f;               // Duration of the ambient magic effect. If 0, it lasts until the effect is removed.
    std::string targeting = "single";    // Targeting type of the ambient magic effect (single, area, etc.)
    DataTable ambientMagicData;          // Ambient magic data (stats, etc.)

    // === Processing ===
    void processNode() override { }
    void update() override {}
    void processAmbientMagic();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AmbientMagic() = default;     // Default destructor
};

