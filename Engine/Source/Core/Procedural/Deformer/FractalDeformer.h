/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FractalDeformer.h
 * @brief Header file for the FractalDeformer class.
 * 
 * ARCHITECTURAL NOTES:
 * - FractalDeformer is a class that represents a fractal deformer in Hydragon.
 * - It is used to create fractal effects on a shape or object.
 * - It supports various fractal types, such as linear, circular, and helical.
 * - It also supports different fractal directions, such as positive and negative.
 * - It can be used to create various effects, such as branching, fractal patterns, and self-similarity.
 */
#pragma once
#include "Engine.h"
#include "Deformer.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct FractalDeformerInfo : public DeformerInfo {
    FractalDeformerInfo() {
        nodeType = "Procedural/FractalDeformer";
        
        inputs = {
            "shape",          // Shape to be deformed fractally
            "fractalType",    // Linear, circular, helical, etc.
            "fractalAxis",    // Axis of fractal deformation
            "fractalDirection",// Positive or negative direction
            "fractalAmount",  // Amount of fractal deformation
            "fractalCenter"   // Center of fractal deformation
        };
        
        outputs = {
            "fractalShape",   // Fractally deformed shape
            "fractalMetrics"  // Performance and quality metrics
        };
    }
};

class FractalDeformer : public Deformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit FractalDeformer(const FractalDeformerInfo& info = FractalDeformerInfo())
        : Deformer(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    FractalDeformerInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processFractal();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~FractalDeformer() = default;     // Default destructor
};

} // namespace hd
