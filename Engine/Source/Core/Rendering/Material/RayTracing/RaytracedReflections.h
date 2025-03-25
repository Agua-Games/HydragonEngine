/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file RaytracedReflections.h
 * @brief Header file for the RaytracedReflections class.
 * 
 * ARCHITECTURAL NOTES:
 * - RaytracedReflections is a class that represents a raytraced reflections effect in the engine's node graph.
 * - It is used to apply a raytraced reflections effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"  // Include PostProcess.h for base class PostProcess

namespace hd {

struct RaytracedReflectionsInfo : public PostProcessInfo {
    RaytracedReflectionsInfo() {
        NodeType = "Rendering/PostProcess/RaytracedReflections";
        inputs = {
            "RenderedImage",   // Rendered image to apply raytraced reflections to
            "RaytracedReflectionsParams"  // Raytraced reflections parameters
        };
        outputs = {
            "RaytracedReflectionsImage",  // Raytraced reflections-applied image
            "RaytracedReflectionsMetrics"  // Raytraced reflections performance metrics
        };
    }
};

class RaytracedReflections : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit RaytracedReflections(const RaytracedReflectionsInfo& info = RaytracedReflectionsInfo())
        : PostProcess(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RaytracedReflections() = default;     // Default destructor
};

} // namespace hd
