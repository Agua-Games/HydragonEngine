/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EyeAdaptationExposure.h
 * @brief Header file for the EyeAdaptationExposure class.
 * 
 * ARCHITECTURAL NOTES:
 * - EyeAdaptationExposure is a class that represents an eye adaptation and exposure effect in the engine's node graph.
 * - It is used to apply an eye adaptation and exposure effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct EyeAdaptationExposureInfo : public PostProcessInfo {
    EyeAdaptationExposureInfo() {
        NodeType = "Rendering/PostProcess/EyeAdaptationExposure";
        inputs = {
            "RenderedImage",   // Rendered image to apply eye adaptation and exposure to
            "EyeAdaptationExposureParams"  // Eye adaptation and exposure parameters
        };
        outputs = {
            "EyeAdaptationExposedImage",  // Eye adaptation and exposure-applied image
            "EyeAdaptationExposureMetrics"  // Eye adaptation and exposure performance metrics
        };
    }
};

class EyeAdaptationExposure : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit EyeAdaptationExposure(const EyeAdaptationExposureInfo& info = EyeAdaptationExposureInfo())
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
    ~EyeAdaptationExposure() = default;     // Default destructor
};

} // namespace hd
