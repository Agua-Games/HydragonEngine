/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Battery.h
 * @brief Header file for the Battery class.
 * 
 * ARCHITECTURAL NOTES:
 * - Battery is a class that represents a battery in Hydragon.
 * - It is used to represent any battery in the game world, such as AA, AAA, C, D, etc.
 * - It supports interactive features, such as charging, discharging, and monitoring. Also supports two-way messaging with other batteries, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Prop.h"
#include "DataTable.h"

namespace hd {

struct BatteryInfo : public PropInfo {
    BatteryInfo() {
        nodeType = "Gameplay/Battery";
        
        inputs = {
            "batteryType",  // Type of battery (AA, AAA, C, D, etc.)
            "batteryData",  // Battery data
            "environment", // Environment data
            "characterData", // Character data
            "batteryState" // Battery state
        };
        
        outputs = {
            "batteryStatus", // Battery status
            "batteryMetrics" // Battery performance metrics
        };
    }
};

class Battery : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Battery(const BatteryInfo& info = BatteryInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void charge();
    void discharge();
    void monitor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Battery() = default;     // Default destructor
};

} // namespace hd
