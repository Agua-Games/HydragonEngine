/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Swarm.h
 * @brief Header file for the Swarm class.
 * 
 * ARCHITECTURAL NOTES:
 * - Swarm is a class that represents a swarm in Hydragon.
 * - It is used to represent any swarm in the game world, such as bird flocks, fish schools, etc.
 * - It supports interactive features, such as flocking, schooling, and swarming. Also supports two-way messaging with other swarms, environment, character, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct SwarmInfo : public NodeInfo {
    SwarmInfo() {
        nodeType = "Gameplay/Swarm";
        
        inputs = {
            "swarmType",       // Type of swarm (bird flock, fish school, etc.)
            "swarmData",       // Swarm data
            "environment",     // Environment data
            "characterData",   // Character data
            "swarmState"       // Swarm state
        };
        
        outputs = {
            "swarmStatus",     // Swarm status
            "swarmMetrics"     // Swarm performance metrics
        };
    }
};

class Swarm : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Swarm(const SwarmInfo& info = SwarmInfo())
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
    ~Swarm() = default;     // Default destructor
};

} // namespace hd