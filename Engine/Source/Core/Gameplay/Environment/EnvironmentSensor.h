/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EnvironmentSensor.h
 * @brief EnvironmentSensor represents an environment sensor in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - EnvironmentSensor is a class that represents an environment sensor in Hydragon.
 * - It is used to represent any environment sensor in the game world, from character sensory organs, senses, to environment sensors, such as cameras, microphones, etc.
 * - It supports interactive features, such as sensing, monitoring, and analyzing. Also supports two-way messaging with other environment sensors, environment, character, etc.
 * - It supports sampling different types of environment data, such as light, sound, temperature, humidity, etc.
 * - Not to be confused with EnvironmentManager, which is a class that manages the environment in the game.
 */

#pragma once

#include "Node.h"
#include "DataTable.h"

namespace hd {

struct EnvironmentSensorInfo : public NodeInfo {
    EnvironmentSensorInfo() {
        nodeType = "Gameplay/EnvironmentSensor";
        
        inputs = {
            "sensorType",                // Type of environment sensor (camera, microphone, etc.)
            "visionCone",                // Vision cone of the environment sensor
            "visionRange",               // Vision range of the environment sensor
            "lightSensitivity",          // Light sensitivity of the environment sensor
            "soundDetectionRadius",      // Sound detection radius of the environment sensor
            "soundDetectionBinaural",    // Sound detection binaural of the environment sensor
            "alertnessLevel",            // Alertness level of the environment sensor (0.0f to 1.0f)
            "suspicionThreshold",        // Suspicion threshold of the environment sensor
            "sensorData",                // Environment sensor data
            "environment",               // Environment data
            "characterData",             // Character data
            "sensorState"                // Environment sensor state
        };
        
        outputs = {
            "sensorStatus",      // Environment sensor status
            "sensorMetrics"      // Environment sensor performance metrics
        };
    }
};

class EnvironmentSensor : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit EnvironmentSensor(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : Node(info) {}    // Default constructor
    void initialize() override {}
    void load() override {}

    // === Processing ===
    void sampleEnvironmentData();
    void senseEnvironmentData();
    void monitorEnvironmentData();
    void analyzeEnvironmentData();
    void processEnvironmentSensor();
    void processNode() override {}
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EnvironmentSensor() = default;     // Default destructor
};
}