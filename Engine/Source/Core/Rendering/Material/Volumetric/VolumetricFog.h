/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file VolumetricFog.h
 * @brief Header file for the VolumetricFog class.
 * 
 * ARCHITECTURAL NOTES:
 * - VolumetricFog is a class that represents a volumetric fog effect in the engine's node graph.
 * - It is used to apply a volumetric fog effect to rendered images.
 * 
 */

#pragma once

#include "PostProcess.h"

namespace hd {

struct VolumetricFogInfo : public PostProcessInfo {
    VolumetricFogInfo() {
        NodeType = "Rendering/PostProcess/VolumetricFog";
        inputs = {
            "RenderedImage",   // Rendered image to apply volumetric fog to
            "VolumetricFogParams"  // Volumetric fog parameters
        };
        outputs = {
            "VolumetricFogImage",  // Volumetric fog-applied image
            "VolumetricFogMetrics"  // Volumetric fog performance metrics
        };
    }
};

class VolumetricFog : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit VolumetricFog(const VolumetricFogInfo& info = VolumetricFogInfo())
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
    ~VolumetricFog() = default;     // Default destructor
};

} // namespace hd
