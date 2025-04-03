/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceStation.h
 * @brief Header file for the SpaceStation class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceStation is a class that represents a space station in Hydragon.
 * - It is used to represent any space station in the game world, such as space stations, spaceports, etc.
 * - It supports interactive features, such as docking, undocking, and trading. Also supports two-way messaging with other space stations, environment, character, etc.
 * 
 * @todo Decide if using composition of Satellite and Station, or simple inheritance plus an instance member (of Satellite).
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Station.h"
#include "Satellite.h"
#include "DataTable.h"

namespace hd {

struct SpaceStationInfo : public StationInfo, public SatelliteInfo {
    SpaceStationInfo() {
        nodeType = "Gameplay/SpaceStation";
        
        inputs = {
            "spaceStationType",  // Type of space station (space station, spaceport, etc.)
            "spaceStationData",  // Space station data
            "environment",       // Environment data
            "characterData",     // Character data
            "spaceStationState"  // Space station state
        };
        
        outputs = {
            "spaceStationStatus",  // Space station status
            "spaceStationMetrics"  // Space station performance metrics
        };
    }
};

class SpaceStation : public Satellite, public Station {
public:
    // === Structure Definitions ===
    enum class AlarmLevel {
        None,
        Low,
        Medium,
        High,
        Critical
    };

    // === Allocation, Initialization, Loading ===
    explicit SpaceStation(const SpaceStationInfo& info = SpaceStationInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void changeAttitude() override;
    void rotate() override;
    void orbit() override;
    void changeOrbit() override;
    void adjustPanelOrientation() override;
    void simulateCentripetalGravity();
    void dock();
    void undock();
    void trade();
    void communicate() override;
    void repair() override;
    void upgrade() override;
    void equip() override;
    void defend() override;
    void attack() override;
    void evade() override;
    void setAlarm(AlarmLevel level);
    void deactivate() override;
    void selfDestruct() override;
    void processSpaceStation();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpaceStation() = default;     // Default destructor
};

} // namespace hd
