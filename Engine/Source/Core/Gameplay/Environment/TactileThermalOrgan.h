/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TactileThermalOrgan.h
 * @brief Header file for the TactileThermalOrgan class.
 * 
 * ARCHITECTURAL NOTES:
 * - TactileThermalOrgan is a class that represents a tactile thermal organ in Hydragon. As some other classes derived from EnvironmentSensor, it is mostly an archetype/preset class - only 
 * some members are configured to model a specific type of environment sensor, to spare users the trouble of configuring it every time.
 * - It is used to represent any tactile thermal organ in the game world, such as skin, etc.
 */
#pragma once
#include "EnvironmentSensor.h"

namespace hd {

class TactileThermalOrgan : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading === 
    explicit TactileThermalOrgan(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : EnvironmentSensor(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable tactileThermalOrganData;
    DataTable tactileThermalOrganState;
    bool tactileThermalDetection = true;
    float tactileThermalDetectionSensitivity = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TactileThermalOrgan() = default;     // Default destructor
};

} // namespace hd
