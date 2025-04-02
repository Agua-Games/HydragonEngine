/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Automobile.h
 * @brief Header file for the Automobile class.
 * 
 * ARCHITECTURAL NOTES:
 * - Automobile is a class that represents an automobile in Hydragon.
 * - It is used to represent any automobile in the game world, such as cars, trucks, vans, etc.
 * - It supports interactive features, such as driving, steering, and braking. Also supports two-way messaging with other automobiles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct AutomobileInfo : public VehicleInfo {
    AutomobileInfo() {
        nodeType = "Gameplay/Automobile";
        
        inputs = {
            "model",        // Model of the automobile
            "position",     // Position of the automobile
            "rotation",     // Rotation of the automobile
            "scale",        // Scale of the automobile
            "material",     // Material of the automobile
            "animation",    // Animation of the automobile
            "physics",      // Physics of the automobile
            "collision",    // Collision of the automobile
            "script"        // Script of the automobile
        };
        
        outputs = {
            "automobileStatus", // Automobile status
            "automobileMetrics" // Automobile performance metrics
        };
    }
};

class Automobile : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Automobile(const AutomobileInfo& info = AutomobileInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    AutomobileType type = AutomobileType::Other;
    float tireFriction = 0.0f;
    float tireRadius = 0.0f;
    float tireWidth = 0.0f;
    float tirePressure = 0.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Automobile() = default;     // Default destructor
};

} // namespace hd
