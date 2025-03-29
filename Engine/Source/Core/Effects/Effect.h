/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Effect.h
 * @brief Header file for the Effect class.
 * 
 * ARCHITECTURAL NOTES:
 * - Effect is a class that represents an effect in Hydragon.
 * - It is used to represent any effect in the game world, such as visual, audio, etc.
 * - It supports interactive features, such as targeting, duration, etc.
 * - It makes use of WavePhysics, MagicField and other non-realistic fields, for effects which support energy transfer and propagation - thermal, kinetic, etc.
 * - It supports two-way messaging with other effects, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Prop.h"
#include "EnvironmentManager.h"

struct EffectInfo : public NodeInfo {
    EffectInfo() {
        nodeType = "Gameplay/Effect";
        
        inputs = {
            "type",            // Type of effect (visual, audio, etc.)
            "intensity",       // Intensity of the effect
            "duration",        // Duration of the effect
            "targeting",       // Targeting type of the effect (single, area, etc.)
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

class Effect : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Effect(const EffectInfo& info = EffectInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}
    
    // Set default values
    // (...)

    // === Processing ===
    void processNode() override { }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Effect() = default;     // Default destructor
};
