/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file KineticSensor.h
 * @brief Header file for the KineticSensor class.
 * 
 * ARCHITECTURAL NOTES:
 * - KineticSensor is a class that represents a kinetic sensor in Hydragon.
 * - It is used to represent any kinetic sensor in the game world, such as accelerometers, gyroscopes, etc.
 * - It supports interactive features, such as sensing, monitoring, and analyzing. Also supports two-way messaging with other environment sensors, environment, character, etc.
 */
#pragma once
#include "EnvironmentSensor.h"
#include "Device.h"

namespace hd {

class KineticSensor : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading ===
    explicit KineticSensor(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : EnvironmentSensor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable kineticSensorData;
    DataTable kineticSensorState;
    Device device;                      // Device is used to communicate with other devices
    bool kineticDetection = true;
    float kineticDetectionSensitivity = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~KineticSensor() = default;     // Default destructor
};

} // namespace hd
