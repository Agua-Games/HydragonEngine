/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Horse.h
 * @brief Header file for the Horse class.
 * 
 * ARCHITECTURAL NOTES:
 * - Horse is a class that represents a horse in Hydragon.
 * - It is used to represent any horse in the game world, such as stallions, mares, geldings, etc.
 * - It supports interactive features, such as galloping, trotting, and neighing. Also supports two-way messaging with other horses, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct HorseInfo : public VehicleInfo {
    HorseInfo() {
        nodeType = "Gameplay/Horse";
        
        inputs = {
            "horseType",        // Type of horse (stallion, mare, gelding, etc.)
            "horseData",        // Horse data
            "environment",      // Environment data
            "characterData",    // Character data
            "horseState"        // Horse state
        };
        
        outputs = {
            "horseStatus",      // Horse status
            "horseMetrics"      // Horse performance metrics
        };
    }
};

class Horse : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit Horse(const HorseInfo& info = HorseInfo())
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
    ~Horse() = default;     // Default destructor
};

} // namespace hd