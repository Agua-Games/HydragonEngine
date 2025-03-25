/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file ColorGrading.h
 * @brief Header file for the ColorGrading class.
 * 
 * ARCHITECTURAL NOTES:
 * - ColorGrading is a class that represents a color grading effect in the engine's node graph.
 * - It is used to apply a color grading effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct ColorGradingInfo : public PostProcessInfo {
    ColorGradingInfo() {
        NodeType = "Rendering/PostProcess/ColorGrading";
        inputs = {
            "RenderedImage",   // Rendered image to apply color grading to
            "ColorGradingParams"  // Color grading parameters
        };
        outputs = {
            "ColorGradedImage",  // Color grading-applied image
            "ColorGradingMetrics"  // Color grading performance metrics
        };
    }
};

class ColorGrading : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit ColorGrading(const ColorGradingInfo& info = ColorGradingInfo())
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
    ~ColorGrading() = default;     // Default destructor
};

} // namespace hd
