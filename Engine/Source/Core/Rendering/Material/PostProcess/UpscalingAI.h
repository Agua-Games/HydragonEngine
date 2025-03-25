/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UpscalingAI.h
 * @brief Header file for the UpscalingAI class.
 * 
 * ARCHITECTURAL NOTES:
 * - UpscalingAI is a class that represents an upscaling AI effect in the engine's node graph.
 * - It is used to apply an upscaling AI effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct UpscalingAIInfo : public PostProcessInfo {
    UpscalingAIInfo() {
        NodeType = "Rendering/PostProcess/UpscalingAI";
        inputs = {
            "RenderedImage",   // Rendered image to apply upscaling AI to
            "UpscalingAIParams"  // Upscaling AI parameters
        };
        outputs = {
            "UpscalingAIedImage",  // Upscaling AI-applied image
            "UpscalingAIMetrics"  // Upscaling AI performance metrics
        };
    }
};

class UpscalingAI : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit UpscalingAI(const UpscalingAIInfo& info = UpscalingAIInfo())
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
    ~UpscalingAI() = default;     // Default destructor
};

} // namespace hd
