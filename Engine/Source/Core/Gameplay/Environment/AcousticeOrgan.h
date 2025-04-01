/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AcousticeOrgan.h
 * @brief Header file for the AcousticeOrgan class.
 * 
 * ARCHITECTURAL NOTES:
 * - AcousticeOrgan is a class that represents an acoustic organ in Hydragon. As some other classes derived from EnvironmentSensor, it is mostly an archetype/preset class - only
 * some members are configured to model a specific type of environment sensor, to spare users the trouble of configuring it every time.
 * - It is used to represent any acoustic organ in the game world, such as ears, etc.
 */
#pragma once
#include "EnvironmentSensor.h"

namespace hd {

class AcousticeOrgan : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading === 
    explicit AcousticeOrgan(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : EnvironmentSensor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable acousticeOrganData;
    DataTable acousticeOrganState;
    bool acousticeDetection = true;
    float acousticeDetectionSensitivity = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AcousticeOrgan() = default;     // Default destructor
};

} // namespace hd
