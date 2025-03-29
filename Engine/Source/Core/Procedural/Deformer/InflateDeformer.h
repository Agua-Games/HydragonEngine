/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InflateDeformer.h
 * @brief Header file for the InflateDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - InflateDeformer is a class that represents an inflation effect in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct InflateDeformerInfo : public DeformerInfo {
    InflateDeformerInfo() {
        nodeType = "Procedural/InflateDeformer";
        
        inputs = {
            "inputData",                // Input data to be inflateDeformerd
            "inflateDeformerType",      // Type of inflation (e.g., noise, warp, etc.)
            "inflateDeformerParams"     // Additional parameters for the inflation
        };
        
        outputs = {
            "inflateDeformerdData"      // InflateDeformerd output data
        };
    }
};

class InflateDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit InflateDeformer(const InflateDeformerInfo& info = InflateDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    InflateDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processInflateDeformer();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~InflateDeformer() = default;     // Default destructor
};

} // namespace hd
