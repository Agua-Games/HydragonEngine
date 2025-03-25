/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file StopMotionRender.h
 * @brief Header file for the StopMotionRender class.
 * 
 * ARCHITECTURAL NOTES:
 * - StopMotionRender is a class that represents a stop motion render effect in the engine's node graph.
 * - It is used to apply a stop motion render effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct StopMotionRenderInfo : public PostProcessInfo {
    StopMotionRenderInfo() {
        NodeType = "Rendering/PostProcess/StopMotionRender";
        inputs = {
            "RenderedImage",   // Rendered image to apply stop motion render to
            "StopMotionRenderParams"  // Stop motion render parameters
        };
        outputs = {
            "StopMotionRenderedImage",  // Stop motion render-applied image
            "StopMotionRenderMetrics"  // Stop motion render performance metrics
        };
    }
};

class StopMotionRender : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit StopMotionRender(const StopMotionRenderInfo& info = StopMotionRenderInfo())
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
    ~StopMotionRender() = default;     // Default destructor
};

} // namespace hd
