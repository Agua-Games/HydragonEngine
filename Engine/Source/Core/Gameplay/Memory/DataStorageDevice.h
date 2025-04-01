/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DataStorageDevice.h
 * @brief Header file for the DataStorageDevice class.
 * 
 * ARCHITECTURAL NOTES:
 * - DataStorageDevice is a class that represents a data storage device in Hydragon.
 * - It is used to represent any data storage device in the game world, such as hard drives, flash drives, etc.
 * - It supports interactive features, such as storing, retrieving, and analyzing data. Also supports two-way messaging with other data storage devices, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Device.h"

namespace hd {

struct DataStorageDeviceInfo : public DeviceInfo {
    DataStorageDeviceInfo() {
        nodeType = "Gameplay/DataStorageDevice";
        
        inputs = {
            "dataStorageDeviceType",  // Type of data storage device (hard drive, flash drive, etc.)
            "dataStorageDeviceData",  // Data storage device data
            "environment",            // Environment data
            "characterData",          // Character data
            "dataStorageDeviceState"  // Data storage device state
        };
        
        outputs = {
            "dataStorageDeviceStatus",  // Data storage device status
            "dataStorageDeviceMetrics"  // Data storage device performance metrics
        };
    }
};

class DataStorageDevice : public Device {
public:
    // === Allocation, Initialization, Loading === 
    explicit DataStorageDevice(const DataStorageDeviceInfo& info = DataStorageDeviceInfo())
        : Device(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processDataStorageDevice();
    void processNode() override {
        processDataStorageDevice(); 
    }
    void store();
    void retrieve();
    void analyze();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DataStorageDevice() = default;     // Default destructor
};

} // namespace hd
