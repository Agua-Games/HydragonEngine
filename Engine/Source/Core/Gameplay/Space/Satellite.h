/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Satellite.h
 * @brief Header file for the Satellite class.
 * 
 * ARCHITECTURAL NOTES:
 * - Satellite is a class that represents a satellite in Hydragon.
 * - It is used to represent any satellite in the game world, such as communication satellites, weather satellites, etc.
 * - It supports interactive features, such as orbiting, rotating, and communicating. Also supports two-way messaging with other satellites, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SatelliteInfo : public NodeInfo {
    SatelliteInfo() {
        nodeType = "Gameplay/Satellite";
        
        inputs = {
            "satelliteType",  // Type of satellite (communication, weather, etc.)
            "satelliteData",  // Satellite data
            "environment",    // Environment data
            "characterData",  // Character data
            "satelliteState"  // Satellite state
        };
        
        outputs = {
            "satelliteStatus",  // Satellite status
            "satelliteMetrics"  // Satellite performance metrics
        };
    }
};

class Satellite : public Node {
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
    explicit Satellite(const SatelliteInfo& info = SatelliteInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void orbit();
    void rotate();
    void changeAttitude();
    void changeOrbit();
    void adjustPanelOrientation();
    void simulateCentripetalGravity();
    void dock();
    void undock();
    void trade();
    void communicate();
    void repair();
    void upgrade();
    void equip();
    void defend();
    void attack();
    void evade();
    void setAlarm(AlarmLevel level);
    void deactivate();
    void selfDestruct();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Satellite() = default;     // Default destructor
};

} // namespace hd
