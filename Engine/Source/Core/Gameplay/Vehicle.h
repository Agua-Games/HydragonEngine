/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Vehicle.h
 * @brief Header file for the Vehicle class.
 * 
 * ARCHITECTURAL NOTES:
 * - Vehicle is a class that represents a vehicle in Hydragon.
 * - It is used to represent any vehicle in the game world, such as cars, motorcycles, boats, planes, etc.
 * - It supports interactive features, such as driving, steering, and braking. Also supports two-way messaging with other vehicles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct VehicleInfo : public NodeInfo {
    VehicleInfo() {
        NodeType = "Gameplay/Vehicle";
        
        inputs = {
            "Model",        // Model of the vehicle
            "Position",     // Position of the vehicle
            "Rotation",     // Rotation of the vehicle
            "Scale",        // Scale of the vehicle
            "Material",     // Material of the vehicle
            "Animation",    // Animation of the vehicle
            "Physics",      // Physics of the vehicle
            "Collision",    // Collision of the vehicle
            "Script"        // Script of the vehicle
        };
        
        outputs = {
            "Model",        // Model of the vehicle
            "Position",     // Position of the vehicle
            "Rotation",     // Rotation of the vehicle
            "Scale",        // Scale of the vehicle
            "Material",     // Material of the vehicle
            "Animation",    // Animation of the vehicle
            "Physics",      // Physics of the vehicle
            "Collision",    // Collision of the vehicle
            "Script"        // Script of the vehicle
        };
    }
};

class Vehicle : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Vehicle(const NodeInfo& info = NodeInfo()) : Node(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // === Processing ===
    void update() override {}
    void processVehicle();
    void processNodeGraph() override {
        processVehicle(); 
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Vehicle() = default;     // Default destructor
};

} // namespace hd
