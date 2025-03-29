/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Beacon.h
 * @brief Header file for the Beacon class.
 * 
 * ARCHITECTURAL NOTES:
 * - Beacon is a class that represents a beacon in Hydragon.
 * - It is used to represent any beacon in the game world, such as lighthouses, buoys, etc.
 * - It supports interactive features, such as signaling, guiding, and warning. Also supports two-way messaging with other beacons, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct BeaconInfo : public NodeInfo {
    BeaconInfo() {
        nodeType = "Gameplay/Beacon";
        
        inputs = {
            "beaconType",      // Type of beacon (lighthouse, buoy, etc.)
            "beaconData",      // Beacon data
            "environment",     // Environment data
            "characterData",   // Character data
            "beaconState"      // Beacon state
        };
        
        outputs = {
            "beaconStatus",    // Beacon status
            "beaconMetrics"    // Beacon performance metrics
        };
    }
};

class Beacon : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Beacon(const BeaconInfo& info = BeaconInfo())
        : Node(info) {}
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
    ~Beacon() = default;     // Default destructor
};

} // namespace hd
