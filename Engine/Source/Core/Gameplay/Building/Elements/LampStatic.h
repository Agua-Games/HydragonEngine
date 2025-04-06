/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LampStatic.h
 * @brief Header file for the LampStatic class.
 * 
 * ARCHITECTURAL NOTES:
 * - LampStatic is a class that represents a static lamp in Hydragon.
 * - It is used to represent any static lamp in the game world, such as street lamps, etc.
 * - It supports interactive features, such as lighting, dimming, and flickering. Also supports two-way messaging with other lamps, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Light.h"
#include "LightMesh.h"
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct LampStaticInfo : public BuildingElementInfo {
    LampStaticInfo() {
        nodeType = "Gameplay/LampStatic";
        
        inputs = {
            "lampType",        // Type of lamp (street lamp, etc.)
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

class LampStatic : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit LampStatic(const LampStaticInfo& info = LampStaticInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~LampStatic() = default;     // Default destructor
};

} // namespace hd
