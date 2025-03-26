/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Station.h
 * @brief Header file for the Station class.
 * 
 * ARCHITECTURAL NOTES:
 * - Station is a class that represents a station in Hydragon.
 * - It is used to represent any station in the game world, such as train stations, bus stations, airports, etc.
 * - It supports interactive features, such as boarding, disembarking, and ticketing. Also supports two-way messaging with other stations, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct StationInfo : public BuildingInfo {
    StationInfo() {
        nodeType = "Gameplay/Station";
        
        inputs = {
            "model",        // Model of the station
            "position",     // Position of the station
            "rotation",     // Rotation of the station
            "scale",        // Scale of the station
            "material",     // Material of the station
            "animation",    // Animation of the station
            "physics",      // Physics of the station
            "collision",    // Collision of the station
            "script"        // Script of the station
        };
        
        outputs = {
            "model",        // Model of the station
            "position",     // Position of the station
            "rotation",     // Rotation of the station
            "scale",        // Scale of the station
            "material",     // Material of the station
            "animation",    // Animation of the station
            "physics",      // Physics of the station
            "collision",    // Collision of the station
            "script"        // Script of the station
        };
    }
};

class Station : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit Station(const StationInfo& info = StationInfo())
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
    ~Station() = default;     // Default destructor
};

} // namespace hd
