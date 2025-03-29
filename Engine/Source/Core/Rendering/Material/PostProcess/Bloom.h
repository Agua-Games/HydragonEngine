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
        nodeType = "Rendering/PostProcess/Bloom";
        inputs = {
            "renderedImage",   // Rendered image to apply bloom to
            "intensity",   // Bloom intensity
            "threshold",   // Bloom threshold
            "radius",   // Bloom radius
            "quality",   // Bloom quality
            "bloomParams"  // Bloom parameters
        };
        outputs = {
            "bloomedImage",  // Bloom-applied image
            "bloomMetrics"  // Bloom performance metrics
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

    float intensity = 1.0f;
    float threshold = 1.0f;
    float radius = 1.0f;
    float quality = 1.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Bloom() = default;     // Default destructor
};

} // namespace hd
