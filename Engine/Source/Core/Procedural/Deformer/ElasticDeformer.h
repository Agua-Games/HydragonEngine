/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ElasticDeformer.h
 * @brief Header file for the ElasticDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - ElasticDeformer is a class that represents an elastic deformer in Hydragon.
 * - It is used to create elastic effects on a shape or object.
 * - It supports various elastic types, such as linear, circular, and helical.
 * - It also supports different elastic directions, such as positive and negative.
 * - It can be used to create various effects, such as stretching, compressing, and bouncing.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct ElasticDeformerInfo : public DeformerInfo {
    ElasticDeformerInfo() {
        nodeType = "Procedural/ElasticDeformer";
        
        inputs = {
            "shape",          // Shape to be deformed elastically
            "elasticType",    // Linear, circular, helical, etc.
            "elasticAxis",    // Axis of the elastic deformation
            "elasticDirection",// Positive or negative direction
            "elasticAmount",  // Amount of elastic deformation
            "elasticCenter"   // Center of the elastic deformation
        };
        
        outputs = {
            "elasticShape",   // Elasticly deformed shape
            "elasticMetrics"  // Performance and quality metrics
        };
    }
};

class ElasticDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit ElasticDeformer(const ElasticDeformerInfo& info = ElasticDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    ElasticDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processElastic();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ElasticDeformer() = default;     // Default destructor
};

} // namespace hd

