/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ChromaticAberration.h
 * @brief Header file for the ChromaticAberration class.
 * 
 * ARCHITECTURAL NOTES:
 * - ChromaticAberration is a class that represents a chromatic aberration effect in the engine's node graph.
 * - It is used to apply a chromatic aberration effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct ChromaticAberrationInfo : public PostProcessInfo {
    ChromaticAberrationInfo() {
        NodeType = "Rendering/PostProcess/ChromaticAberration";
        inputs = {
            "RenderedImage",   // Rendered image to apply chromatic aberration to
            "ChromaticAberrationParams"  // Chromatic aberration parameters
        };
        outputs = {
            "ChromaticAberrationedImage",  // Chromatic aberration-applied image
            "ChromaticAberrationMetrics"  // Chromatic aberration performance metrics
        };
    }
};

class ChromaticAberration : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit ChromaticAberration(const ChromaticAberrationInfo& info = ChromaticAberrationInfo())
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
    ~ChromaticAberration() = default;     // Default destructor
};

} // namespace hd
