/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ToonShadingScreen.h
 * @brief Header file for the ToonShadingScreen class.
 * 
 * ARCHITECTURAL NOTES:
 * - ToonShadingScreen is a class that represents a toon shading effect in the engine's node graph.
 * - It is used to apply a toon shading effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct ToonShadingScreenInfo : public PostProcessInfo {
    ToonShadingScreenInfo() {
        NodeType = "Rendering/PostProcess/ToonShadingScreen";
        inputs = {
            "RenderedImage",   // Rendered image to apply toon shading to
            "ToonShadingScreenParams"  // Toon shading parameters
        };
        outputs = {
            "ToonShadingScreenImage",  // Toon shading-applied image
            "ToonShadingScreenMetrics"  // Toon shading performance metrics
        };
    }
};

class ToonShadingScreen : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit ToonShadingScreen(const ToonShadingScreenInfo& info = ToonShadingScreenInfo())
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
    ~ToonShadingScreen() = default;     // Default destructor
};

} // namespace hd
