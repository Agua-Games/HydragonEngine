/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SolarArray.h
 * @brief Header file for the SolarArray class.
 * 
 * ARCHITECTURAL NOTES:
 * - SolarArray is a class that represents a solar array in Hydragon.
 * - It is used to represent any solar array in the game world, such as solar panels, solar farms, etc.
 * - It supports interactive features, such as generating, storing, and distributing energy. Also supports two-way messaging with other solar arrays, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct SolarArrayInfo : public NodeInfo {
    SolarArrayInfo() {
        nodeType = "Gameplay/SolarArray";
        
        inputs = {
            "solarArrayType",  // Type of solar array (solar panel, solar farm, etc.)
            "solarArrayData",  // Solar array data
            "environment",     // Environment data
            "characterData",   // Character data
            "solarArrayState"  // Solar array state
        };
        
        outputs = {
            "solarArrayStatus",  // Solar array status
            "solarArrayMetrics"  // Solar array performance metrics
        };
    }
};

class SolarArray : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SolarArray(const SolarArrayInfo& info = SolarArrayInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void generate();
    void store();
    void distribute();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SolarArray() = default;     // Default destructor
};

} // namespace hd
