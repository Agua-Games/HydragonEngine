/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BroadcastDevice.h
 * @brief Header file for the BroadcastDevice class.
 * 
 * ARCHITECTURAL NOTES:
 * - BroadcastDevice is a class that represents a broadcast device in Hydragon.
 * - It is used to represent any broadcast device in the game world, such as megaphones, walkie-talkies, alarms, radios, TVs, etc.
 * - It supports interactive features, such as broadcasting, receiving, and analyzing. Also supports two-way messaging with other broadcast devices, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Device.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "AudioFile.h"

namespace hd {

struct BroadcastDeviceInfo : public DeviceInfo {
    BroadcastDeviceInfo() {
        nodeType = "Gameplay/BroadcastDevice";
        
        inputs = {
            "broadcastDeviceType",  // Type of broadcast device (megaphone, walkie-talkie, alarm, radio, TV, etc.)
            "broadcastDeviceData",  // Broadcast device data
            "environment",          // Environment data
            "characterData",        // Character data
            "broadcastDeviceState"  // Broadcast device state
        };
        
        outputs = {
            "broadcastDeviceStatus",  // Broadcast device status
            "broadcastDeviceMetrics"  // Broadcast device performance metrics
        };
    }
};

class BroadcastDevice : public Device {
public:
    // === Allocation, Initialization, Loading === 
    explicit BroadcastDevice(const BroadcastDeviceInfo& info = BroadcastDeviceInfo())
        : Device(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processBroadcastDevice();
    void processNode() override {
        processBroadcastDevice(); 
    }
    void broadcast();
    void receive();
    void analyze();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BroadcastDevice() = default;     // Default destructor
};

} // namespace hd
