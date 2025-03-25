/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Vignette.h
 * @brief Header file for the Vignette class.
 * 
 * ARCHITECTURAL NOTES:
 * - Vignette is a class that represents a vignette effect in the engine's node graph.
 * - It is used to apply a vignette effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct VignetteInfo : public PostProcessInfo {
    VignetteInfo() {
        NodeType = "Rendering/PostProcess/Vignette";
        inputs = {
            "RenderedImage",   // Rendered image to apply vignette to
            "VignetteParams"  // Vignette parameters
        };
        outputs = {
            "VignettedImage",  // Vignette-applied image
            "VignetteMetrics"  // Vignette performance metrics
        };
    }
};

class Vignette : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit Vignette(const VignetteInfo& info = VignetteInfo())
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
    ~Vignette() = default;     // Default destructor
};

} // namespace hd
