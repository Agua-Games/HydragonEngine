/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Device.h
 * @brief Header file for the Device class.
 * 
 * ARCHITECTURAL NOTES:
 * - Device is a class that represents a device in Hydragon.
 * - It is used to represent any device in the game world, such as radio equipment, phones, tablets, computers, etc.
 * - It supports interactive features, such as using the device, interacting with the environment, etc. Also supports two-way messaging with other devices, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "AudioFile.h"

namespace hd {

struct DeviceInfo : public NodeInfo {
    DeviceInfo() {
        nodeType = "Gameplay/Device";
        
        inputs = {
            "deviceType",        // Type of device (phone, tablet, computer, etc.)
            "deviceData",        // Device data
            "environment",       // Environment data
            "characterData",     // Character data
            "deviceState"        // Device state
        };
        
        outputs = {
            "deviceStatus",      // Device status
            "deviceMetrics"      // Device performance metrics
        };
    }
};

class Device : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Device(const DeviceInfo& info = DeviceInfo())
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
    ~Device() = default;     // Default destructor
};

} // namespace hd
