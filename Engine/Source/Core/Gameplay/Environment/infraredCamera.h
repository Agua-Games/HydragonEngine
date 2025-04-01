/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InfraredCamera.h
 * @brief Header file for the InfraredCamera class.
 * 
 * ARCHITECTURAL NOTES:
 * - InfraredCamera is a class that represents an infrared camera in Hydragon.
 * - It is used to represent any infrared camera in the game world, such as cameras, etc.
 * - It supports interactive features, such as sensing, monitoring, and analyzing. Also supports two-way messaging with other environment sensors, environment, character, etc.
 */
#pragma once
#include "EnvironmentSensor.h"
#include "Device.h"

namespace hd {

class InfraredCamera : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading === 
    explicit InfraredCamera(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : EnvironmentSensor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable infraredCameraData;
    DataTable infraredCameraState;
    Device device;                      // Device is used to communicate with other devices
    bool infraredDetection = true;
    float infraredDetectionSensitivity = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~InfraredCamera() = default;     // Default destructor
};

} // namespace hd
