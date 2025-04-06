/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SplineScatter.h
 * @brief Header file for the SplineScatter class.
 * 
 * ARCHITECTURAL NOTES:
 * - SplineScatter is a class that represents a spline scatter in Hydragon.
 * - It is used to scatter objects along a spline, from very regular placement patterns (like roads, pathways, railroads, hanging lamps, pipelines, piping, etc.), to 
 * very irregular placement patterns (like trees, rocks, etc.).
 * - It supports different scattering modes, such as uniform, random, and weighted.
 * - It supports different object types, such as particles, meshes, and sprites.
 * - It supports different spline types, such as linear, cubic, and bezier.
 * - It supports different scattering parameters, such as density, spacing, and offset.
 * - It uses the Vulkan API for spline scatter processing.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "SplineCurve.h"
#include "DataTable.h"

namespace hd {

struct SplineScatterInfo : public SplineCurveInfo {
    SplineScatterInfo() {
        nodeType = "Procedural/SplineScatter";
        
        inputs = {
            "SplineType",       // Linear, cubic, bezier, etc.
            "SplinePoints",     // Control points of the spline
            "ScatterMode",      // Uniform, random, weighted, etc.
            "ObjectTypes",      // Particle, mesh, sprite, etc.
            "Density",          // Number of objects per unit length
            "Spacing",          // Minimum distance between objects
            "Offset",           // Random offset for scattering objects
            "Seed"              // Random seed for scattering objects
        };
        
        outputs = {
            "Objects",          // Scattered objects
            "Metrics"           // Performance and quality metrics
        };
    }
};

class SplineScatter : public SplineCurve {
public:
    // === Allocation, Initialization, Loading ===
    explicit SplineScatter(const SplineScatterInfo& info = SplineScatterInfo())
        : SplineCurve(info) {}    // Default constructor for SplineScatter class, which is derived from SplineCurve class.   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SplineScatter() = default;     // Default destructor
};

} // namespace hd
