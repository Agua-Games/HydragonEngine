/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VolumetricMedia.h
 * @brief Header file for the VolumetricMedia class.
 * 
 * ARCHITECTURAL NOTES:
 * - VolumetricMedia is a class that represents a volumetric media effect in the engine's node graph.
 * - It is used to apply a volumetric media effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct VolumetricMediaInfo : public PostProcessInfo {
    VolumetricMediaInfo() {
        NodeType = "Rendering/PostProcess/VolumetricMedia";
        inputs = {
            "RenderedImage",   // Rendered image to apply volumetric media to
            "VolumetricMediaParams"  // Volumetric media parameters
        };
        outputs = {
            "VolumetricMediaedImage",  // Volumetric media-applied image
            "VolumetricMediaMetrics"  // Volumetric media performance metrics
        };
    }
};

class VolumetricMedia : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit VolumetricMedia(const VolumetricMediaInfo& info = VolumetricMediaInfo())
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
    ~VolumetricMedia() = default;     // Default destructor
};

} // namespace hd
