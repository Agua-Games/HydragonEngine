/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VisionOrgan.h
 * @brief Header file for the VisionOrgan class.
 * 
 * ARCHITECTURAL NOTES:
 * - VisionOrgan is a class that represents a vision organ in Hydragon. As some other classes derived from EnvironmentSensor, it is mostly an archetype/preset class - only 
 * some members are configured to model a specific type of environment sensor, to spare users the trouble of configuring it every time.
 * - It is used to represent any vision organ in the game world, such as eyes, etc.
 */
#pragma once
#include "EnvironmentSensor.h"

namespace hd {

class VisionOrgan : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading === 
    explicit VisionOrgan(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : EnvironmentSensor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable visionOrganData;
    DataTable visionOrganState;
    bool visionDetection = true;
    float visionDetectionSensitivity = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~VisionOrgan() = default;     // Default destructor
};

} // namespace hd
