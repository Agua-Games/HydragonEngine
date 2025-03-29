/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Motorbike.h
 * @brief Header file for the Motorbike class.
 * 
 * ARCHITECTURAL NOTES:
 * - Motorbike is a class that represents a motorbike in Hydragon.
 * - It is used to represent any motorbike in the game world, such as motorcycles, mopeds, etc.
 * - It supports interactive features, such as driving, steering, and braking. Also supports two-way messaging with other motorbikes, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct MotorbikeInfo : public VehicleInfo {
    MotorbikeInfo() {
        nodeType = "Gameplay/Motorbike";
        
        inputs = {
            "model",        // Model of the motorbike
            "position",     // Position of the motorbike
            "rotation",     // Rotation of the motorbike
            "scale",        // Scale of the motorbike
            "material",     // Material of the motorbike
            "animation",    // Animation of the motorbike
            "physics",      // Physics of the motorbike
            "collision",    // Collision of the motorbike
            "script"        // Script of the motorbike
        };
        
        outputs = {
            "motorbikeStatus", // Motorbike status
            "motorbikeMetrics" // Motorbike performance metrics
        };
    }
};

class Motorbike : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Motorbike(const MotorbikeInfo& info = MotorbikeInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Motorbike() = default;     // Default destructor
};

} // namespace hd

