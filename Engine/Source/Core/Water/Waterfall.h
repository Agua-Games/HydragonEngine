/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Waterfall.h
 * @brief Header file for the Waterfall class.
 * 
 * ARCHITECTURAL NOTES:
 * - Waterfall is a class that represents a waterfall in the engine's node graph.
 * - It is used to represent any waterfall in the game world, such as streams, creeks, etc.
 * - It supports splines for placement, with terrain integration, procedural erosion, etc.
 * - It supports procedural features, such as water flow, water movement, water reflection, water refraction, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other waterfalls, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "WaterSurface.h"

namespace hd {

struct WaterfallInfo : public NodeInfo {
    WaterfallInfo() {
        NodeType = "Water/Waterfall";
        
        inputs = {
            "Type",        // Type of waterfall (stream, creek, etc.)
            "Params",      //  parameters
            "State",       //  state
            "Flow",        //  flow parameters
            "Interaction", //  interaction parameters
            "Rendering",   //  rendering parameters
            "Physics",     //  physics parameters
            "LOD",         //  LOD parameters, etc.
            "LODState"     //  LOD state
        };        
        
        outputs = {
            "LODState",    //  LOD state
            "LODMetrics"   //  LOD performance metrics
        };
    }
};

class Waterfall : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Waterfall(const WaterfallInfo& info = WaterfallInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override { }
    void processWaterfall();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Waterfall() = default;     // Default destructor
};

} // namespace hd
