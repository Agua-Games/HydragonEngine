/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FlightMechanics.h
 * @brief Header file for the FlightMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - FlightMechanics is a class that represents flight mechanics in Hydragon.
 * - It is used to represent any flight mechanics in the game world, such as flying, hovering, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other flight mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Math.h"
#include "Ability.h"

namespace hd {

struct FlightMechanicsInfo : public NodeInfo {
    FlightMechanicsInfo() {
        nodeType = "FlightMechanics";
        
        inputs = {
            "flightData",        // Flight data
            "environment",       // Environment data
            "characterData",     // Character data
            "flightState"        // Flight state
        };
        
        outputs = {
            "flightStatus",      // Flight status
            "flightMetrics"      // Flight performance metrics
        };
    }
};

class FlightMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit FlightMechanics(const FlightMechanicsInfo& info = FlightMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    float alitude = 0.0f;

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FlightMechanics() = default;     // Default destructor
};

} // namespace hd
