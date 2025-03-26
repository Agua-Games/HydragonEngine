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
        NodeType = "Rendering/PostProcess/ScreenSpaceAO";
        inputs = {
            "RenderedImage",   // Rendered image to apply screen space ambient occlusion to
            "ScreenSpaceAOParams"  // Screen space ambient occlusion parameters
        };
        outputs = {
            "ScreenSpaceAOedImage",  // Screen space ambient occlusion-applied image
            "ScreenSpaceAOMetrics"  // Screen space ambient occlusion performance metrics
        };
    }
};

class ScreenSpaceAO : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit ScreenSpaceAO(const ScreenSpaceAOInfo& info = ScreenSpaceAOInfo())
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
    ~ScreenSpaceAO() = default;     // Default destructor
};

} // namespace hd
