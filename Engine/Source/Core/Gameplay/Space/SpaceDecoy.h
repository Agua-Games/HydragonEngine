/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceDecoy.h
 * @brief Header file for the SpaceDecoy class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceDecoy is a class that represents a space decoy in Hydragon.
 * - It is used to represent any space decoy in the game world, such as space decoys, buoys, sensors, cameras, etc.
 * - It supports interactive features, such as deploying, detonating, and monitoring. Also supports two-way messaging with other space decoys, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Satellite.h"
#include "Decoy.h"
#include "DataTable.h"

namespace hd {

struct SpaceDecoyInfo : public SatelliteInfo, public DecoyInfo {
    SpaceDecoyInfo() {
        nodeType = "Gameplay/SpaceDecoy";
        
        inputs = {
            "spaceDecoyType",  // Type of space decoy (space decoy, buoy, sensor, camera, etc.)
            "spaceDecoyData",  // Space decoy data
            "environment",     // Environment data
            "characterData",   // Character data
            "spaceDecoyState"  // Space decoy state
        };
        
        outputs = {
            "spaceDecoyStatus",  // Space decoy status
            "spaceDecoyMetrics"  // Space decoy performance metrics
        };
    }
};

class SpaceDecoy : public Satellite, public Decoy {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpaceDecoy(const SpaceDecoyInfo& info = SpaceDecoyInfo())
        : Satellite(info), Decoy(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void deployDecoy();
    void detonateDecoy();
    void monitor();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpaceDecoy() = default;     // Default destructor
};

} // namespace hd

