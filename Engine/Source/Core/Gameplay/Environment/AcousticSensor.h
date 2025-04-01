/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AcousticSensor.h
 * @brief Header file for the AcousticSensor class.
 * 
 * ARCHITECTURAL NOTES:
 * - AcousticSensor is a class that represents an acoustic sensor in Hydragon. As some other classes derived from EnvironmentSensor, it is mostly an archetype/preset class - only 
 * some members are configured to model a specific type of environment sensor, to spare users the trouble of configuring it every time.
 * - It is used to represent any acoustic sensor in the game world, such as microphones, etc.
 */
#pragma once
#include "EnvironmentSensor.h"
#include "Device.h"

namespace hd {

class AcousticSensor : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading === 
    explicit AcousticSensor(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : EnvironmentSensor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable acousticSensorData;
    DataTable acousticSensorState;
    Device device;                      // Device is used to communicate with other devices
    bool acousticDetection = true;
    float acousticDetectionSensitivity = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AcousticSensor() = default;     // Default destructor
};

} // namespace hd
