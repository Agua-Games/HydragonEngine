/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EnvironmentManager.h
 * @brief Used to manage the environment.
 * 
 * ARCHITECTURAL NOTES:
 * - EnvironmentManager is a class that manages the environment in the game.
 * - It supports interactive features, such as weather, time of day, etc. Also supports two-way messaging with other environment sensors, environment, character, etc.
 * - Not to be confused with EnvironmentSensor, which is a class that represents a specific environment sensor in Hydragon, such as Camera, Microphone, etc.
 */

#pragma once

#include "Node.h"
#include "DataTable.h"
#include "EnvironmentSensor.h"

namespace hd {

struct EnvironmentManagerInfo : public NodeInfo {
    EnvironmentManagerInfo() {
        nodeType = "Gameplay/EnvironmentManager";
        
        inputs = {
            "environmentData",  // Environment data
            "environmentState"  // Environment state
        };
        
        outputs = {
            "environmentStatus",  // Environment status
            "environmentMetrics"  // Environment performance metrics
        };
    }
};

class EnvironmentManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit EnvironmentManager(const EnvironmentManagerInfo& info = EnvironmentManagerInfo())
        : Node(info) {}    // Default constructor
    void initialize() override {}
    void load() override {}

    // === Processing ===
    // Node
    void processNode() override {}
    void processEnvironmentManager();

    // Environment control
    void updateEnvironmentState();
    void processEnvironmentalEffects();
    void broadcastEnvironmentChanges();

    // Environment events
    void addHazard();
    void removeHazard();
    void addEvent();
    void removeEvent();

    // Environment state

    // Sensor management
    void registerSensor(EnvironmentSensor* sensor);
    void unregisterSensor(EnvironmentSensor* sensor);
    void processSensorData();

    // Query environment data
    DataTable getEnvironmentData();
    void queryWeatherManager();
    void queryTimeManager();
    void queryCharacterManager();
    void queryProceduralManager();
    void queryHazardManager();
    void queryGameplayManager();
    void queryEnvironmentSensor();

    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EnvironmentManager() = default;     // Default destructor
};
}