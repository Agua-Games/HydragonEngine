/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Pickup.h
 * @brief Header file for the Pickup class.
 * 
 * ARCHITECTURAL NOTES:
 * - Pickup is a class that represents a pickup in Hydragon.
 * - It is used to represent any pickup in the game world, such as health packs, ammo crates, etc.
 * - It supports interactive features, such as picking up, using, and dropping. Also supports two-way messaging with other pickups, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct PickupInfo : public NodeInfo {
    PickupInfo() {
        nodeType = "Gameplay/Pickup";
        
        inputs = {
            "pickupType",      // Type of pickup (health pack, ammo crate, etc.)
            "pickupData",      // Pickup data
            "environment",     // Environment data
            "characterData",   // Character data
            "pickupState"      // Pickup state
        };
        
        outputs = {
            "pickupStatus",    // Pickup status
            "pickupMetrics"    // Pickup performance metrics
        };
    }
};

class Pickup : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Pickup(const PickupInfo& info = PickupInfo())
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
    ~Pickup() = default;     // Default destructor
};

} // namespace hd
