/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TerraformingDevice.h
 * @brief Header file for the TerraformingDevice class.
 * 
 * ARCHITECTURAL NOTES:
 * - TerraformingDevice is a class that represents a terraforming device in Hydragon.
 * - It is used to represent any terraforming device in the game world, such as terraforming machines, terraforming tools, etc.
 * - It supports interactive features, such as terraforming, mining, and analyzing. Also supports two-way messaging with other terraforming devices, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "SpaceStation.h"
#include "Device.h"
#include "DataTable.h"

namespace hd {

struct TerraformingDeviceInfo : public SpaceStationInfo, public DeviceInfo {
    TerraformingDeviceInfo() {
        nodeType = "Gameplay/TerraformingDevice";
        
        inputs = {
            "terraformingDeviceType",  // Type of terraforming device (terraforming machine, terraforming tool, etc.)
            "terraformingDeviceData",  // Terraforming device data
            "environment",             // Environment data
            "characterData",           // Character data
            "terraformingDeviceState"  // Terraforming device state
        };
        
        outputs = {
            "terraformingDeviceStatus",    // Terraforming device status
            "terraformingDeviceMetrics"    // Terraforming device performance metrics
        };
    }
};

class TerraformingDevice : public SpaceStation, public Device {
public:
    // === Allocation, Initialization, Loading ===
    explicit TerraformingDevice(const TerraformingDeviceInfo& info = TerraformingDeviceInfo())
        : SpaceStation(info), Device(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void terraform();
    void mine();
    void analyze();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TerraformingDevice() = default;     // Default destructor  
};

} // namespace hd
