/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EscapePod.h
 * @brief Header file for the EscapePod class.
 * 
 * ARCHITECTURAL NOTES:
 * - EscapePod is a class that represents an escape pod in Hydragon.
 * - It is used to represent any escape pod in the game world, such as escape pods, lifeboats, etc.
 * - It supports interactive features, such as launching, steering, and docking. Also supports two-way messaging with other escape pods, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Vehicle.h"

namespace hd {

struct EscapePodInfo : public VehicleInfo {
    EscapePodInfo() {
        nodeType = "Gameplay/EscapePod";
        
        inputs = {
            "escapePodType",  // Type of escape pod (escape pod, lifeboat, etc.)
            "escapePodData",  // Escape pod data
            "environment",    // Environment data
            "characterData",  // Character data
            "escapePodState"  // Escape pod state
        };
        
        outputs = {
            "escapePodStatus",  // Escape pod status
            "escapePodMetrics"  // Escape pod performance metrics
        };
    }
};

class EscapePod : public Vehicle {
public:
    // === Allocation, Initialization, Loading ===
    explicit EscapePod(const EscapePodInfo& info = EscapePodInfo())
        : Vehicle(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void launch();
    void steer();
    void dock();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EscapePod() = default;     // Default destructor
};

} // namespace hd
