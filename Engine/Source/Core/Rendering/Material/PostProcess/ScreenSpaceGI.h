/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ScreenSpaceGI.h
 * @brief Header file for the ScreenSpaceGI class.
 * 
 * ARCHITECTURAL NOTES:
 * - ScreenSpaceGI is a class that represents a screen space global illumination effect in the engine's node graph.
 * - It is used to apply a screen space global illumination effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct ScreenSpaceGIInfo : public PostProcessInfo {
    ScreenSpaceGIInfo() {
        NodeType = "Rendering/PostProcess/ScreenSpaceGI";
        inputs = {
            "RenderedImage",   // Rendered image to apply screen space global illumination to
            "ScreenSpaceGIParams"  // Screen space global illumination parameters
        };
        outputs = {
            "ScreenSpaceGIedImage",  // Screen space global illumination-applied image
            "ScreenSpaceGIMetrics"  // Screen space global illumination performance metrics
        };
    }
};

class ScreenSpaceGI : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit ScreenSpaceGI(const ScreenSpaceGIInfo& info = ScreenSpaceGIInfo())
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
    ~ScreenSpaceGI() = default;     // Default destructor
};

} // namespace hd
