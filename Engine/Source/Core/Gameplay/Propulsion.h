/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Propulsion.h
 * @brief Header file for the Propulsion class.
 * 
 * ARCHITECTURAL NOTES:
 * - Propulsion is a class that represents a propulsion system in Hydragon.
 * - It is used to represent any propulsion system in the game world, such as engines, thrusters, etc.
 * - It supports interactive features, such as starting, stopping, and controlling. Also supports two-way messaging with other propulsion systems, environment, character, etc.
 */

#pragma once
#include "Engine.h"
#include "Node.h"
#include "Prop.h"

namespace hd {

struct PropulsionInfo : public PropInfo {
    PropulsionInfo() {
        nodeType = "Gameplay/Propulsion";
        
        inputs = {
            "propulsionType",  // Type of propulsion (engine, thruster, etc.)
            "propulsionData",  // Propulsion data
            "environment",     // Environment data
            "characterData",   // Character data
            "propulsionState"  // Propulsion state
        };
        
        outputs = {
            "propulsionStatus",  // Propulsion status
            "propulsionMetrics"  // Propulsion performance metrics
        };
    }
};

class Propulsion : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Propulsion(const PropulsionInfo& info = PropulsionInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}

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
    ~Propulsion() = default;     // Default destructor  
};

} // namespace hd
