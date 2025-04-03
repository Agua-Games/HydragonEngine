/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AntennaArray.h
 * @brief Header file for the AntennaArray class.
 * 
 * ARCHITECTURAL NOTES:
 * - AntennaArray is a class that represents an antenna array in Hydragon.
 * - It is used to represent any antenna array in the game world, such as radio antenna arrays, radiotelescope arrays, etc.
 * - It supports interactive features, such as broadcasting, receiving, and analyzing. Also supports two-way messaging with other antenna arrays, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Building.h"
#include "DataTable.h"
#include "RadioAntenna.h"
#include "Placement.h"

namespace hd {

enum class AntennaArrayType {
    Radio,
    Radiotelescope,
    Custom
};

struct AntennaArrayInfo : public BuildingInfo {
    AntennaArrayInfo() {
        nodeType = "Gameplay/AntennaArray";
        
        inputs = {
            "antennaArrayType",  // Type of antenna array (radio antenna array, radiotelescope array, etc.)
            "antennaArrayData",  // Antenna array data
            "environment",       // Environment data
            "characterData",     // Character data
            "antennaArrayState"  // Antenna array state
        };
        
        outputs = {
            "antennaArrayStatus",  // Antenna array status
            "antennaArrayMetrics"  // Antenna array performance metrics
        };
    }
};

class AntennaArray : public Building {
public:
    // === Allocation, Initialization, Loading ===
    explicit AntennaArray(const AntennaArrayInfo& info = AntennaArrayInfo())
        : Building(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    AntennaArrayType type = AntennaArrayType::Custom;
    std::vector<RadioAntenna> antennas;
    DataTable antennaData;
    Placement placement;

    // === Processing ===
    void processNode() override { }
    void broadcast();
    void receive();
    void analyze();
    void focus();
    void synchronize();             // Synchronize the antenna array with other antenna arrays
    void processAntenna();          // Process each antenna in the antenna array separately
    void processAntennaArray();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~AntennaArray() = default;     // Default destructor
};

} // namespace hd