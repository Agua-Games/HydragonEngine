/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceBeacon.h
 * @brief Header file for the SpaceBeacon class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceBeacon is a class that represents a space beacon in Hydragon.
 * - It is used to represent any space beacon in the game world, such as space beacons, buoys, etc.
 * - It supports interactive features, such as signaling, guiding, and warning. Also supports two-way messaging with other space beacons, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Satellite.h"
#include "Beacon.h"
#include "DataTable.h"

namespace hd {

struct SpaceBeaconInfo : public SatelliteInfo, public BeaconInfo {
    SpaceBeaconInfo() {
        nodeType = "Gameplay/SpaceBeacon";
        
        inputs = {
            "spaceBeaconType",  // Type of space beacon (space beacon, buoy, sensor, camera, etc.)
            "spaceBeaconData",  // Space beacon data
            "environment",      // Environment data
            "characterData",    // Character data
            "spaceBeaconState"  // Space beacon state
        };
        
        outputs = {
            "spaceBeaconStatus",  // Space beacon status
            "spaceBeaconMetrics"  // Space beacon performance metrics
        };
    }
};

class SpaceBeacon : public Satellite, public Beacon {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpaceBeacon(const SpaceBeaconInfo& info = SpaceBeaconInfo())
        : Satellite(info), Beacon(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void signal();
    void guide();
    void warn();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpaceBeacon() = default;     // Default destructor
};

} // namespace hd
