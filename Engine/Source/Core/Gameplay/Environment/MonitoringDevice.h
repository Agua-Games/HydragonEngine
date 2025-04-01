/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MonitoringDevice.h
 * @brief Header file for the MonitoringDevice class.
 * 
 * ARCHITECTURAL NOTES:
 * - MonitoringDevice is a class that represents a monitoring device in Hydragon.
 * - It is used to represent any monitoring device in the game world, such as cameras, microphones, etc.
 * - It supports interactive features, such as monitoring, recording, and analyzing. Also supports two-way messaging with other monitoring devices, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Device.h"

namespace hd {

struct MonitoringDeviceInfo : public DeviceInfo {
    MonitoringDeviceInfo() {
        nodeType = "Gameplay/MonitoringDevice";
        
        inputs = {
            "monitoringDeviceType",  // Type of monitoring device (camera, microphone, etc.)
            "monitoringDeviceData",  // Monitoring device data
            "environment",           // Environment data
            "characterData",         // Character data
            "monitoringDeviceState"  // Monitoring device state
        };
        
        outputs = {
            "monitoringDeviceStatus",  // Monitoring device status
            "monitoringDeviceMetrics"  // Monitoring device performance metrics
        };
    }
};

class MonitoringDevice : public Device {
public:
    // === Allocation, Initialization, Loading ===
    explicit MonitoringDevice(const MonitoringDeviceInfo& info = MonitoringDeviceInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~MonitoringDevice() = default;     // Default destructor
};

} // namespace hd
