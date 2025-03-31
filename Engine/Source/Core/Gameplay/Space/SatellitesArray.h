/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SatellitesArray.h
 * @brief Header file for the SatellitesArray class.
 * 
 * ARCHITECTURAL NOTES:
 * - SatellitesArray is a class that represents an array of satellites in Hydragon.
 * - It is used to represent any array of satellites in the game world, such as communication satellite arrays, weather satellite arrays, etc.
 * - It supports interactive features, such as orbiting, rotating, and communicating. Also supports two-way messaging with other satellites, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Satellite.h"
#include "DataTable.h"
#include "Placement.h"

namespace hd {

struct SatellitesArrayInfo : public SatelliteInfo {
    SatellitesArrayInfo() {
        nodeType = "Gameplay/SatellitesArray";
        
        inputs = {
            "satellitesArrayType",  // Type of satellites array (communication, weather, etc.)
            "satellitesArrayData",  // Satellites array data
            "environment",          // Environment data
            "characterData",        // Character data
            "satellitesArrayState"  // Satellites array state
        };
        
        outputs = {
            "satellitesArrayStatus",  // Satellites array status
            "satellitesArrayMetrics"  // Satellites array performance metrics
        };
    }
};

class SatellitesArray : public Satellite {
public:
    // === Allocation, Initialization, Loading ===
    explicit SatellitesArray(const SatellitesArrayInfo& info = SatellitesArrayInfo())
        : Satellite(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    SatelliteArrayType type = SatelliteArrayType::Custom;
    std::vector<Satellite> satellites;
    DataTable satelliteData;
    Placement placement;

    // === Processing ===
    void processNode() override { }
    void orbit();
    void rotate();
    void communicate();
    void synchronize();             // Synchronize the satellites array with other satellites arrays
    void processSatellite();        // Process each satellite in the satellites array separately
    void processSatellitesArray();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SatellitesArray() = default;     // Default destructor
};

} // namespace hd