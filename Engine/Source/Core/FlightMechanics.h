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
    float altitude = 0.0f;          // Altitude in meters
    float maxAltitude = 0.0f;       // Maximum altitude in meters
    float wingSpan = 0.0f;          // Wing span in meters
    float speed = 0.0f;             // Speed in meters per second
    float altitudeChange = 0.0f;    // Altitude change in meters per second
    float yaw = 0.0f;               // Yaw in degrees
    float pitch = 0.0f;             // Pitch in degrees
    float roll = 0.0f;              // Roll in degrees
    float bank = 0.0f;              // Bank in degrees
    float lift = 0.0f;              // Lift in Newtons
    float drag = 0.0f;              // Drag in Newtons
    float thrust = 0.0f;            // Thrust in Newtons
    float weight = 0.0f;            // Weight in Newtons
    float fuel = 0.0f;              // Fuel in liters
    float fuelConsumption = 0.0f;   // Fuel consumption in liters per second
    float hoverStability = 0.0f;    // Hover stability in percentage

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FlightMechanics() = default;     // Default destructor
};

} // namespace hd
