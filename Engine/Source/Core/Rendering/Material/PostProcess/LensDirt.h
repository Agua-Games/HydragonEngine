/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LensDirt.h
 * @brief Header file for the LensDirt class.
 * 
 * ARCHITECTURAL NOTES:
 * - LensDirt is a class that represents a lens dirt effect in the engine's node graph.
 * - It is used to apply a lens dirt effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct LensDirtInfo : public PostProcessInfo {
    LensDirtInfo() {
        NodeType = "Rendering/PostProcess/LensDirt";
        inputs = {
            "RenderedImage",   // Rendered image to apply lens dirt to
            "LensDirtParams"  // Lens dirt parameters
        };
        outputs = {
            "LensDirtedImage",  // Lens dirt-applied image
            "LensDirtMetrics"  // Lens dirt performance metrics
        };
    }
};

class LensDirt : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit LensDirt(const LensDirtInfo& info = LensDirtInfo())
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
    ~LensDirt() = default;     // Default destructor
};

} // namespace hd
