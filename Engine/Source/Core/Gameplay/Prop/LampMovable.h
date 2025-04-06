/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LampMovable.h
 * @brief Header file for the LampMovable class.
 * 
 * ARCHITECTURAL NOTES:
 * - LampMovable is a class that represents a movable lamp in Hydragon.
 * - It is used to represent any portable, movable lamp in the game world, such as torches, lanterns, etc.
 * - It supports interactive features, such as lighting, dimming, and flickering. Also supports two-way messaging with other LampMovables, environment, character, etc.
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

struct LampMovableInfo : public PropInfo {
    LampMovableInfo() {
        nodeType = "Gameplay/LampMovable";
        
        inputs = {
            "LampMovableType",        // Type of LampMovable (torch, lantern, etc.)
            "LampMovableData",        // LampMovable data
            "environment",            // Environment data
            "characterData",          // Character data
            "LampMovableState"        // LampMovable state
        };
        
        outputs = {
            "LampMovableStatus",      // LampMovable status
            "LampMovableMetrics"      // LampMovable performance metrics
        };
    }
};

class LampMovable : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit LampMovable(const LampMovableInfo& info = LampMovableInfo())
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
    ~LampMovable() = default;     // Default destructor
};

} // namespace hd
