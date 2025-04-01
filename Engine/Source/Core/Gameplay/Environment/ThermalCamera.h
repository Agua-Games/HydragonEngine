/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ThermalCamera.h
 * @brief Header file for the ThermalCamera class.
 * 
 * ARCHITECTURAL NOTES:
 * - ThermalCamera is a class that represents a thermal camera in Hydragon.
 * - It is used to represent any thermal camera in the game world, such as thermal cameras, etc.
 * - It supports interactive features, such as sensing, monitoring, and analyzing. Also supports two-way messaging with other environment sensors, environment, character, etc.
 */
#pragma once
#include "EnvironmentSensor.h"
#include "Device.h"

namespace hd {

class ThermalCamera : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading === 
    explicit ThermalCamera(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : EnvironmentSensor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable thermalCameraData;
    DataTable thermalCameraState;
    Device device;                      // Device is used to communicate with other devices
    bool thermalDetection = true;
    float thermalDetectionSensitivity = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ThermalCamera() = default;     // Default destructor
};

} // namespace hd
