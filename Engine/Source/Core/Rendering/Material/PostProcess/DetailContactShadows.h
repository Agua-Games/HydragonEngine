/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DetailContactShadows.h
 * @brief Header file for the DetailContactShadows class.
 * 
 * ARCHITECTURAL NOTES:
 * - DetailContactShadows is a class that represents a detail contact shadows effect in the engine's node graph.
 * - It is used to apply a detail contact shadows effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct DetailContactShadowsInfo : public PostProcessInfo {
    DetailContactShadowsInfo() {
        NodeType = "Rendering/PostProcess/DetailContactShadows";
        inputs = {
            "RenderedImage",   // Rendered image to apply detail contact shadows to
            "DetailContactShadowsParams"  // Detail contact shadows parameters
        };
        outputs = {
            "DetailContactShadowsImage",  // Detail contact shadows-applied image
            "DetailContactShadowsMetrics"  // Detail contact shadows performance metrics
        };
    }
};

class DetailContactShadows : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit DetailContactShadows(const DetailContactShadowsInfo& info = DetailContactShadowsInfo())
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
    ~DetailContactShadows() = default;     // Default destructor
};

} // namespace hd
