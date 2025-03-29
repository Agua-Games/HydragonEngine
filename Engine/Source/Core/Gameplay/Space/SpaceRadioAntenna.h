/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceRadioAntenna.h
 * @brief Header file for the SpaceRadioAntenna class.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceRadioAntenna is a class that represents a space radio antenna in Hydragon.
 * - It is used to represent any space radio antenna in the game world, such as space radio towers, space radio dishes, etc.
 * - It supports interactive features, such as broadcasting, receiving, and analyzing. Also supports two-way messaging with other space radio antennas, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Satellite.h"
#include "RadioAntenna.h"
#include "DataTable.h"

namespace hd {

struct SpaceRadioAntennaInfo : public SatelliteInfo, public RadioAntennaInfo {
    SpaceRadioAntennaInfo() {
        nodeType = "Gameplay/SpaceRadioAntenna";
        
        inputs = {
            "spaceRadioAntennaType",  // Type of space radio antenna (space radio tower, space radio dish, etc.)
            "spaceRadioAntennaData",  // Space radio antenna data
            "environment",            // Environment data
            "characterData",          // Character data
            "spaceRadioAntennaState"  // Space radio antenna state
        };
        
        outputs = {
            "spaceRadioAntennaStatus",  // Space radio antenna status
            "spaceRadioAntennaMetrics"  // Space radio antenna performance metrics
        };
    }
};

class SpaceRadioAntenna : public Satellite, public RadioAntenna {
public:
    // === Allocation, Initialization, Loading ===
    explicit SpaceRadioAntenna(const SpaceRadioAntennaInfo& info = SpaceRadioAntennaInfo())
        : Satellite(info), RadioAntenna(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void broadcast();
    void receive();
    void analyze();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SpaceRadioAntenna() = default;     // Default destructor
} // namespace hd
