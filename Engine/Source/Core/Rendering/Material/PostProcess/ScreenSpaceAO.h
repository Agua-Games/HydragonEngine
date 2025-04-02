/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ScreenSpaceAO.h
 * @brief Header file for the ScreenSpaceAO class.
 * 
 * ARCHITECTURAL NOTES:
 * - ScreenSpaceAO is a class that represents a screen space ambient occlusion effect in the engine's node graph.
 * - It is used to apply a screen space ambient occlusion effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct ScreenSpaceAOInfo : public PostProcessInfo {
    ScreenSpaceAOInfo() {
        nodeType = "Rendering/PostProcess/ScreenSpaceAO";
        inputs = {
            "renderedImage",   // Rendered image to apply screen space ambient occlusion to
            "screenSpaceAOParams"  // Screen space ambient occlusion parameters
        };
        outputs = {
            "screenSpaceAOedImage",  // Screen space ambient occlusion-applied image
            "screenSpaceAOMetrics"  // Screen space ambient occlusion performance metrics
        };
    }
};

class ScreenSpaceAO : public PostProcess {
public:
    // === Structure Definitions ===
    enum class SSAOQuality {
        Low,
        Medium,
        High
    };

    // === Allocation, Initialization, Loading === 
    explicit ScreenSpaceAO(const ScreenSpaceAOInfo& info = ScreenSpaceAOInfo())
        : PostProcess(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    float radius = 0.0f;
    float bias = 0.0f;
    float intensity = 0.0f;
    SSAOQuality quality = SSAOQuality::Low;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ScreenSpaceAO() = default;     // Default destructor
};

} // namespace hd
