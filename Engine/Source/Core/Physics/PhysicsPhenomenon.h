/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PhysicsPhenomenon.h
 * @brief Header file for the PhysicsPhenomenon class.
 * 
 * ARCHITECTURAL NOTES:
 * - PhysicsPhenomenon is a class that represents a physics phenomenon (same as a FX, for all practical purposes) in Hydragon.
 * - It is used to represent any physics phenomenon in the game world, such as collisions, explosions, etc.
 * - It is physics-enabled, via WavePhysics.
 */

#pragma once
#include "Node.h"

namespace hd {

struct PhysicsPhenomenonInfo : public NodeInfo {
    PhysicsPhenomenonInfo() {
        NodeType = "Physics/PhysicsPhenomenon";
        inputs = {
            "PhysicsSettings",  // Physics settings
            "PhysicsData",      // Physics data
            "PhysicsState"      // Physics state
        };
        outputs = {
            "PhysicsStatus",  // Physics status
            "PhysicsMetrics"  // Physics performance metrics
        };
    }
};

class PhysicsPhenomenon : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit PhysicsPhenomenon(const PhysicsPhenomenonInfo& info = PhysicsPhenomenonInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~PhysicsPhenomenon() = default;     // Default destructor
};

} // namespace hd