/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file RaytracedShadows.h
 * @brief Header file for the RaytracedShadows class.
 * 
 * ARCHITECTURAL NOTES:
 * - RaytracedShadows is a class that represents a raytraced shadows effect in the engine's node graph.
 * - It is used to apply a raytraced shadows effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"  // Include PostProcess.h for base class PostProcess

namespace hd {

struct RaytracedShadowsInfo : public PostProcessInfo {
    RaytracedShadowsInfo() {
        NodeType = "Rendering/PostProcess/RaytracedShadows";
        inputs = {
            "RenderedImage",   // Rendered image to apply raytraced shadows to
            "RaytracedShadowsParams"  // Raytraced shadows parameters
        };
        outputs = {
            "RaytracedShadowsImage",  // Raytraced shadows-applied image
            "RaytracedShadowsMetrics"  // Raytraced shadows performance metrics
        };
    }
};

class RaytracedShadows : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit RaytracedShadows(const RaytracedShadowsInfo& info = RaytracedShadowsInfo())
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
    ~RaytracedShadows() = default;     // Default destructor
};

} // namespace hd
