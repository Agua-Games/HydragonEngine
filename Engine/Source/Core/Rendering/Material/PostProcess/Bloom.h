/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Bloom.h
 * @brief Header file for the Bloom class.
 * 
 * ARCHITECTURAL NOTES:
 * - Bloom is a class that represents a bloom effect in the engine's node graph.
 * - It is used to apply a bloom effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct BloomInfo : public PostProcessInfo {
    BloomInfo() {
        NodeType = "Rendering/PostProcess/Bloom";
        inputs = {
            "RenderedImage",   // Rendered image to apply bloom to
            "BloomParams"  // Bloom parameters
        };
        outputs = {
            "BloomedImage",  // Bloom-applied image
            "BloomMetrics"  // Bloom performance metrics
        };
    }
};

class Bloom : public PostProcess {
public:
    // === Allocation, Initialization, Loading ===
    explicit Bloom(const BloomInfo& info = BloomInfo())
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
    ~Bloom() = default;     // Default destructor
};

} // namespace hd
