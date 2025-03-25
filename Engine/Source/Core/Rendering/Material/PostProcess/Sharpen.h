/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Sharpen.h
 * @brief Header file for the Sharpen class.
 * 
 * ARCHITECTURAL NOTES:
 * - Sharpen is a class that represents a sharpen effect in the engine's node graph.
 * - It is used to apply a sharpen effect to rendered images.
 * - Supports for both perspective and orthographic cameras.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct SharpenInfo : public PostProcessInfo {
    SharpenInfo() {
        NodeType = "Rendering/PostProcess/Sharpen";
        inputs = {
            "RenderedImage",   // Rendered image to apply sharpen to
            "SharpenParams"  // Sharpen parameters
        };
        outputs = {
            "SharpenedImage",  // Sharpen-applied image
            "SharpenMetrics"  // Sharpen performance metrics
        };
    }
};

class Sharpen : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit Sharpen(const SharpenInfo& info = SharpenInfo())
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
    ~Sharpen() = default;     // Default destructor
};

} // namespace hd
