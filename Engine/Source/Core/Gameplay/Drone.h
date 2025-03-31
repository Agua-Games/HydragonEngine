/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Drone.h
 * @brief Header file for the Drone class.
 * 
 * ARCHITECTURAL NOTES:
 * - Drone is a class that represents a drone in Hydragon.
 * - It is used to represent any drone in the game world, such as quadcopters, helicopters, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other drones, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Transform.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "PhysicsTypes.h"
#include "AudioFile.h"
#include "DataTable.h"

namespace hd {

enum class DronePropulsionType {
    Quadcopter,
    Helicopter,
    Other
};

enum class DroneType {
    reconUAV,
    attackUAV,
    Other
};

struct DroneInfo : public NodeInfo {
    DroneInfo() {
        nodeType = "Gameplay/Drone";
        
        inputs = {
            "droneType",        // Type of drone (quadcopter, helicopter, etc.)
            "droneData",        // Drone data
            "environment",      // Environment data
            "characterData",    // Character data
            "droneState"        // Drone state
        };
        
        outputs = {
            "droneStatus",      // Drone status
            "droneMetrics"      // Drone performance metrics
        };
    }
};

class Drone : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Drone(const DroneInfo& info = DroneInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DronePropulsionType propulsionType = DronePropulsionType::Quadcopter;
    DroneType type = DroneType::Other;
    Transform transform;
    float altitude = 0.0f;
    float maxAltitude = 0.0f;
    float speed = 0.0f;
    PhysicsData physicsData;
    PhysicsData.velocity = 0.0f;
    PhysicsData.acceleration = 0.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Drone() = default;     // Default destructor
};

} // namespace hd
