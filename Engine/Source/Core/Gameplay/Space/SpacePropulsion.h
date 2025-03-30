/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpacePropulsion.h
 * @brief Header file for the SpacePropulsion class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpacePropulsion is a class that represents an interstellar propulsion system in Hydragon.
 * - It is used to represent any interstellar propulsion system in the game world, such as warp drives, hyperdrives, etc.
 * - It supports interactive features, such as starting, stopping, and controlling. Also supports two-way messaging with other interstellar propulsion systems, environment, character, etc.
 */

#pragma once
#include "Engine.h"
#include "Node.h"
#include "Propulsion.h"
#include "DataTable.h"
#include "Prop.h"
#include "FuelTypes.h"

namespace hd {

struct SpacePropulsionInfo : public PropulsionInfo {
    SpacePropulsionInfo() {
        nodeType = "Gameplay/SpacePropulsion";
        
        inputs = {
            "spacePropulsionType",  // Type of interstellar propulsion (warp drive, hyperdrive, etc.)
            "spacePropulsionData",  // Interstellar propulsion data
            "environment",                 // Environment data
            "characterData",               // Character data
            "spacePropulsionState"  // Interstellar propulsion state
        };
        
        outputs = {
            "spacePropulsionStatus",  // Interstellar propulsion status
            "spacePropulsionMetrics"  // Interstellar propulsion performance metrics
        };
    }
};

class SpacePropulsion : public Propulsion {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpacePropulsion(const SpacePropulsionInfo& info = SpacePropulsionInfo())
        : Propulsion(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    SpacePropulsionType type = SpacePropulsionType::solidPropellant;
    FuelType fuelType = FuelType::custom;
    float warpFactor = 0.0f;
    float maxWarpFactor = 0.0f;

    // === Processing ===
    void processNode() override {    
    }    
    void start();
    void stop();
    void control();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpacePropulsion() = default;     // Default destructor  
};

} // namespace hd
