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
#include "Wave.h"

namespace hd {

struct ColorGradingInfo : public PostProcessInfo {
    ColorGradingInfo() {
        nodeType = "Rendering/PostProcess/ColorGrading";
        inputs = {
            "renderedImage",   // Rendered image to apply color grading to
            "colorGradingParams"  // Color grading parameters
        };
        outputs = {
            "colorGradedImage",  // Color grading-applied image
            "colorGradingMetrics"  // Color grading performance metrics
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

    // Set default values
    RenderTarget* source = nullptr;
    RenderTarget* lut = nullptr;
    float exposure = 0.0f;
    float contrast = 0.0f;
    float saturation = 0.0f;
    float temperature = 0.0f;
    float tint = 0.0f;
    float vibrance = 0.0f;
    glm::vec3 colorBalance = glm::vec3(0.0f);

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ColorGrading() = default;     // Default destructor
};

} // namespace hd
