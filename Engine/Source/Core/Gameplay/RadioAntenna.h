/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RadioAntenna.h
 * @brief Header file for the RadioAntenna class.
 * 
 * ARCHITECTURAL NOTES:
 * - RadioAntenna is a class that represents a radio antenna in Hydragon.
 * - It is used to represent any radio antenna in the game world, such as radio towers, radio dishes, etc.
 * - It supports interactive features, such as broadcasting, receiving, and analyzing. Also supports two-way messaging with other radio antennas, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct RadioAntennaInfo : public NodeInfo {
    RadioAntennaInfo() {
        nodeType = "Gameplay/RadioAntenna";
        
        inputs = {
            "radioAntennaType",  // Type of radio antenna (radio tower, radio dish, etc.)
            "radioAntennaData",  // Radio antenna data
            "environment",       // Environment data
            "characterData",     // Character data
            "radioAntennaState"  // Radio antenna state
        };
        
        outputs = {
            "radioAntennaStatus",  // Radio antenna status
            "radioAntennaMetrics"  // Radio antenna performance metrics
        };
    }
};

class RadioAntenna : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RadioAntenna(const RadioAntennaInfo& info = RadioAntennaInfo())
        : Node(info) {}
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
    ~RadioAntenna() = default;     // Default destructor
};

} // namespace hd
