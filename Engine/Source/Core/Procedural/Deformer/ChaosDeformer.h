/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ChaosDeformer.h
 * @brief Header file for the ChaosDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - ChaosDeformer is a class that represents a chaos simulation deformer in Hydragon.
 * - It is used to simulate the behavior of chaos or turbulence on a shape or object.
 * - It supports various chaos simulation types, such as linear, circular, and helical.
 * - It also supports different chaos simulation directions, such as positive and negative.
 * - It can be used to create various effects, such as swirling, turbulence, and chaos.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct ChaosDeformerInfo : public DeformerInfo {
    ChaosDeformerInfo() {
        nodeType = "Procedural/ChaosDeformer";
        
        inputs = {
            "shape",          // Shape to be deformed by chaos simulation
            "chaosType",      // Linear, circular, helical, etc.
            "chaosAxis",      // Axis of chaos simulation
            "chaosDirection", // Positive or negative direction
            "chaosAmount",    // Amount of chaos simulation
            "chaosCenter"     // Center of chaos simulation
        };
        
        outputs = {
            "chaosShape",     // Chaos-simulated shape
            "chaosMetrics"    // Performance and quality metrics
        };
    }
};

class ChaosDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit ChaosDeformer(const ChaosDeformerInfo& info = ChaosDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    ChaosDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processChaos();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ChaosDeformer() = default;     // Default destructor
};

} // namespace hd
