/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceProbe.h
 * @brief Header file for the SpaceProbe class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceProbe is a class that represents a space probe in Hydragon.
 * - It is used to represent any space probe in the game world, such as space probes, buoys, sensors, cameras, etc.
 * - It supports interactive features, such as scanning, monitoring, and analyzing. Also supports two-way messaging with other space probes, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Satellite.h"
#include "DataTable.h"

namespace hd {

struct SpaceProbeInfo : public SatelliteInfo {
    SpaceProbeInfo() {
        nodeType = "Gameplay/SpaceProbe";
        
        inputs = {
            "spaceProbeType",  // Type of space probe (space probe, buoy, sensor, camera, etc.)
            "spaceProbeData",  // Space probe data
            "environment",     // Environment data
            "characterData",   // Character data
            "spaceProbeState"  // Space probe state
        };
        
        outputs = {
            "spaceProbeStatus",  // Space probe status
            "spaceProbeMetrics"  // Space probe performance metrics
        };
    }
};

class SpaceProbe : public Satellite {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpaceProbe(const SpaceProbeInfo& info = SpaceProbeInfo())
        : Satellite(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void scan();
    void monitor();
    void analyze();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpaceProbe() = default;     // Default destructor
};

} // namespace hd
