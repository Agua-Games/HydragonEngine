/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BulgeDeformer.h
 * @brief Header file for the BulgeDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - BulgeDeformer is a class that represents a bulge or bump effect in Hydragon.
 * - It is used to create a bulge or bump effect on a shape or object.
 * - It supports various bulge types, such as circular, elliptical, and helical.
 * - It also supports different bulge directions, such as positive and negative.
 * - It can be used to create various effects, such as bumps, ridges, and waves.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct BulgeDeformerInfo : public DeformerInfo {
    BulgeDeformerInfo() {
        nodeType = "Procedural/BulgeDeformer";
        
        inputs = {
            "shape",                  // Shape to be bulged or bumped
            "bulgeType",              // Circular, elliptical, helical, etc.
            "bulgeAxis",              // Axis of bulging
            "bulgeDirection",         // Positive or negative direction
            "bulgeAmount",            // Amount of bulging
            "bulgeCenter"             // Center of bulging
        };
        
        outputs = {
            "bulgedShape",            // Bulged or bumped shape
            "bulgeMetrics"            // Performance and quality metrics
        };
    }
};

class BulgeDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit BulgeDeformer(const BulgeDeformerInfo& info = BulgeDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    BulgeDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processBulge();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~BulgeDeformer() = default;     // Default destructor
};

} // namespace hd

