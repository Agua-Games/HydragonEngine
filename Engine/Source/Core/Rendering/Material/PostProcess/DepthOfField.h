/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DepthOfField.h
 * @brief Header file for the DepthOfField class.
 * 
 * ARCHITECTURAL NOTES:
 * - DepthOfField is a class that represents a depth of field effect in the engine's node graph.
 * - It is used to apply a depth of field effect to rendered images.
 * - Supports for both perspective and orthographic cameras.
 * - Support for Bokeh with custom shapes, blades count, built-in film grain, modeling of existing cameras.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct DepthOfFieldInfo : public PostProcessInfo {
    DepthOfFieldInfo() {
        NodeType = "Rendering/PostProcess/DepthOfField";
        inputs = {
            "RenderedImage",   // Rendered image to apply depth of field to
            "DepthOfFieldParams"  // Depth of field parameters
        };
        outputs = {
            "DepthOfFieldedImage",  // Depth of field-applied image
            "DepthOfFieldMetrics"  // Depth of field performance metrics
        };
    }
};

class DepthOfField : public PostProcess {
public:
    // === Allocation, Initialization, Loading ===
    explicit DepthOfField(const DepthOfFieldInfo& info = DepthOfFieldInfo())
        : PostProcess(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    float focalDistance = 0.0f;
    float focalRange = 0.0f;
    float maxBlur = 0.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DepthOfField() = default;     // Default destructor
};

} // namespace hd
