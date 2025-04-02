/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Ski.h
 * @brief Header file for the Ski class.
 * 
 * ARCHITECTURAL NOTES:
 * - Ski is a class that represents a ski in Hydragon.
 * - It is used to represent any ski in the game world, such as snow skis, water skis, etc.
 * - It supports interactive features, such as skiing, snowboarding, and water skiing. Also supports two-way messaging with other skis, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Vehicle.h"
#include "DataTable.h"
#include "Lighting.h"

namespace hd {

struct SkiInfo : public VehicleInfo {
    SkiInfo() {
        nodeType = "Gameplay/Ski";
        
        inputs = {
            "skiType",  // Type of ski (ski, snowboard, water ski, etc.)
            "skiData",  // Ski data
            "environment", // Environment data
            "characterData", // Character data
            "skiState" // Ski state
        };
        
        outputs = {
            "skiStatus", // Ski status
            "skiMetrics" // Ski performance metrics
        };
    }
};

class Ski : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Ski(const SkiInfo& info = SkiInfo())
        : Vehicle(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Ski() = default;     // Default destructor
};

} // namespace hd
