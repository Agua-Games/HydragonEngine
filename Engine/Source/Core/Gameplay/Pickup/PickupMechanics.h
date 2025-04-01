/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PickupMechanics.h
 * @brief Header file for the PickupMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - PickupMechanics is a class that represents pickup mechanics in Hydragon.
 * - It is used to represent any pickup mechanics in the game world, such as picking up, using, and dropping.
 * - It supports interactive features, such as picking up, using, and dropping. Also supports two-way messaging with other pickup mechanics, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Pickup.h"
#include "DataTable.h"

namespace hd {

struct PickupMechanicsInfo : public NodeInfo {
    PickupMechanicsInfo() {
        nodeType = "Gameplay/PickupMechanics";
        
        inputs = {
            "pickupMechanicsData",// Pickup mechanics data
            "environment",        // Environment data
            "characterData",      // Character data
            "pickupMechanicsState"// Pickup mechanics state
        };
        
        outputs = {
            "pickupMechanicsStatus",// Pickup mechanics status
            "pickupMechanicsMetrics"// Pickup mechanics performance metrics
        };
    }
};

class PickupMechanics : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit PickupMechanics(const PickupMechanicsInfo& info = PickupMechanicsInfo())
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
    ~PickupMechanics() = default;     // Default destructor
};

} // namespace hd
