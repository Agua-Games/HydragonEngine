/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Skateboard.h
 * @brief Header file for the Skateboard class.
 * 
 * ARCHITECTURAL NOTES:
 * - Skateboard is a class that represents a skateboard in Hydragon.
 * - It is used to represent any skateboard in the game world, such as longboards, cruisers, etc.
 * - It supports interactive features, such as skating, grinding, and ollieing. Also supports two-way messaging with other skateboards, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct SkateboardInfo : public VehicleInfo {
    SkateboardInfo() {
        nodeType = "Gameplay/Skateboard";
        
        inputs = {
            "model",        // Model of the skateboard
            "position",     // Position of the skateboard
            "rotation",     // Rotation of the skateboard
            "scale",        // Scale of the skateboard
            "material",     // Material of the skateboard
            "animation",    // Animation of the skateboard
            "physics",      // Physics of the skateboard
            "collision",    // Collision of the skateboard
            "script"        // Script of the skateboard
        };
        
        outputs = {
            "skateboardStatus", // Skateboard status
            "skateboardMetrics" // Skateboard performance metrics
        };
    }
};

class Skateboard : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Skateboard(const SkateboardInfo& info = SkateboardInfo())
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
    ~Skateboard() = default;     // Default destructor
};

} // namespace hd
