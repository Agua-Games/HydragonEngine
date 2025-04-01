/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ScannerDevice.h
 * @brief Header file for the ScannerDevice class.
 * 
 * ARCHITECTURAL NOTES:
 * - ScannerDevice is a class that represents a scanner device in Hydragon. Its main difference from EnvironmentSensor is that it actively scans the environment, by
 * sending a givem type of wave, and waiting for the response.
 * - It is used to represent any scanner device in the game world, such as radars, sonars, etc.
 * - It supports interactive features, such as scanning, monitoring, and analyzing. Also supports two-way messaging with other scanner devices, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "EnvironmentSensor.h"

namespace hd {

struct ScannerDeviceInfo : public EnvironmentSensorInfo {
    ScannerDeviceInfo() {
        nodeType = "Gameplay/ScannerDevice";
        
        inputs = {
            "scannerDeviceType",  // Type of scanner device (radar, sonar, etc.)
            "scannerDeviceData",  // Scanner device data
            "environment",        // Environment data
            "characterData",      // Character data
            "scannerDeviceState"  // Scanner device state
        };
        
        outputs = {
            "scannerDeviceStatus",  // Scanner device status
            "scannerDeviceMetrics"  // Scanner device performance metrics
        };
    }
};

class ScannerDevice : public EnvironmentSensor {
public:
    // === Allocation, Initialization, Loading === 
    explicit ScannerDevice(const ScannerDeviceInfo& info = ScannerDeviceInfo())
        : Device(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    ScannerDeviceInfo scannerDeviceInfo = {};  // Initialize scanner device info struct with default values
    float range = 0.0f;
    float jamResistance = 0.0f;
    DataTable scannerDeviceData;

    // === Processing ===
    void processScannerDevice();
    void processNode() override {
        processScannerDevice(); 
    }
    void scan();
    void monitor();
    void analyze();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ScannerDevice() = default;     // Default destructor
};

} // namespace hd
