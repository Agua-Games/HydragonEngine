/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DroneMechanics.h
 * @brief Header file for the DroneMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - DroneMechanics is a class that represents drone mechanics in Hydragon.
 * - It is used to represent any drone mechanics in the game world, such as drones, flying machines, etc.
 * - It supports interactive features, such as flying, hovering, and landing. Also supports two-way messaging with other drone mechanics, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "Drone.h"

namespace hd {

struct DroneMechanicsInfo : public NodeInfo {
    DroneMechanicsInfo() {
        nodeType = "Gameplay/DroneMechanics";
        
        inputs = {
            "droneMechanicsData",// Drone mechanics data
            "environment",       // Environment data
            "characterData",     // Character data
            "droneMechanicsState"// Drone mechanics state
        };
        
        outputs = {
            "droneMechanicsStatus",// Drone mechanics status
            "droneMechanicsMetrics"// Drone mechanics performance metrics
        };
    }
};

class DroneMechanics : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit DroneMechanics(const DroneMechanicsInfo& info = DroneMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DroneMechanics() = default;     // Default destructor
};

} // namespace hd
