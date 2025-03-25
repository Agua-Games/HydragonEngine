/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MotionBlur.h
 * @brief Header file for the MotionBlur class.
 * 
 * ARCHITECTURAL NOTES:
 * - MotionBlur is a class that represents a motion blur effect in the engine's node graph.
 * - It is used to apply a motion blur effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct MotionBlurInfo : public PostProcessInfo {
    MotionBlurInfo() {
        NodeType = "Rendering/PostProcess/MotionBlur";
        inputs = {
            "RenderedImage",   // Rendered image to apply motion blur to
            "MotionBlurParams"  // Motion blur parameters
        };
        outputs = {
            "MotionBlurredImage",  // Motion blur-applied image
            "MotionBlurMetrics"  // Motion blur performance metrics
        };
    }
};

class MotionBlur : public PostProcess {
public:
    // === Allocation, Initialization, Loading ===
    explicit MotionBlur(const MotionBlurInfo& info = MotionBlurInfo())
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
    ~MotionBlur() = default;     // Default destructor
};

} // namespace hd
