/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file River.h
 * @brief Header file for the River class.
 * 
 * ARCHITECTURAL NOTES:
 * - River is a class that represents a river in the engine's node graph.
 * - It is used to represent any river in the game world, such as streams, creeks, etc.
 * - It supports splines for placement, with terrain integration, procedural erosion, etc.
 * - It supports procedural features, such as water flow, water movement, water reflection, water refraction, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other rivers, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "WaterSurface.h"

namespace hd {

struct RiverInfo : public NodeInfo {
    RiverInfo() {
        NodeType = "Water/River";
        
        inputs = {
            "RiverType",        // Type of river (stream, creek, etc.)
            "RiverParams",      // River parameters
            "RiverState",       // River state
            "RiverFlow",        // River flow parameters
            "RiverInteraction", // River interaction parameters
            "RiverRendering",   // River rendering parameters
            "RiverPhysics",     // River physics parameters
            "RiverLOD",         // River LOD parameters, etc.
            "RiverLODState"     // River LOD state
        };        
        
        outputs = {
            "RiverLODState",    // River LOD state
            "RiverLODMetrics"   // River LOD performance metrics
        };
    }
};

class River : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit River(const RiverInfo& info = RiverInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void processRiver();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~River() = default;     // Default destructor
};

} // namespace hd
