/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file RaytracedAO.h
 * @brief Header file for the RaytracedAO class.
 * 
 * ARCHITECTURAL NOTES:
 * - RaytracedAO is a class that represents a raytraced ambient occlusion effect in the engine's node graph.
 * - It is used to apply a raytraced ambient occlusion effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"  // Include PostProcess.h for base class PostProcess

namespace hd {

struct RaytracedAOInfo : public PostProcessInfo {
    RaytracedAOInfo() {
        NodeType = "Rendering/PostProcess/RaytracedAO";
        inputs = {
            "RenderedImage",   // Rendered image to apply raytraced ambient occlusion to
            "RaytracedAOParams"  // Raytraced ambient occlusion parameters
        };
        outputs = {
            "RaytracedAOImage",  // Raytraced ambient occlusion-applied image
            "RaytracedAOMetrics"  // Raytraced ambient occlusion performance metrics
        };
    }
};

class RaytracedAO : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit RaytracedAO(const RaytracedAOInfo& info = RaytracedAOInfo())
        : PostProcess(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RaytracedAO() = default;     // Default destructor
};

} // namespace hd
