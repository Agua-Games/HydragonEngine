/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Lamp.h
 * @brief Header file for the Lamp class.
 * 
 * ARCHITECTURAL NOTES:
 * - Lamp is a class that represents a lamp in Hydragon.
 * - It is used to represent any lamp in the game world, such as street lamps, torches, lanterns, etc.
 * - It supports interactive features, such as lighting, dimming, and flickering. Also supports two-way messaging with other lamps, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Light.h"
#include "LightMesh.h"
#include "Prop.h"
#include "DataTable.h"

namespace hd {

struct LampInfo : public PropInfo {
    LampInfo() {
        nodeType = "Gameplay/Lamp";
        
        inputs = {
            "lampType",        // Type of lamp (street lamp, torch, lantern, etc.)
            "lampData",        // Lamp data
            "environment",     // Environment data
            "characterData",   // Character data
            "lampState"        // Lamp state
        };
        
        outputs = {
            "lampStatus",      // Lamp status
            "lampMetrics"      // Lamp performance metrics
        };
    }
};

class Lamp : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Lamp(const LampInfo& info = LampInfo())
        : Prop(info) {}    // Prop constructor is called instead of Node constructor
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Lamp() = default;     // Default destructor
};

} // namespace hd
