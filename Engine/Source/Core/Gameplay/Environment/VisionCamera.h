/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VisionCamera.h
 * @brief Header file for the VisionCamera class.
 * 
 * ARCHITECTURAL NOTES:
 * - VisionCamera is a class that represents a vision camera in Hydragon.
 * - It is used to represent any vision camera in the game world, such as cameras, etc.
 * - It supports interactive features, such as sensing, monitoring, and analyzing. Also supports two-way messaging with other environment sensors, environment, character, etc.
 */
#pragma once
#include "EnvironmentSensor.h"
#include "Device.h"

namespace hd {

class VisionCamera : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading ===
    explicit VisionCamera(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : EnvironmentSensor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable visionCameraData;
    DataTable visionCameraState;
    Device device;                      // Device is used to communicate with other devices
    bool visionDetection = true;
    float visionDetectionSensitivity = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~VisionCamera() = default;     // Default destructor
};

} // namespace hd
