/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VolumetricCloud.h
 * @brief Header file for the VolumetricCloud class.
 * 
 * ARCHITECTURAL NOTES:
 * - VolumetricCloud is a class that represents a volumetric cloud effect in the engine's node graph.
 * - It is used to apply a volumetric cloud effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"  // Include PostProcess.h for base class PostProcess

namespace hd {

struct VolumetricCloudInfo : public PostProcessInfo {
    VolumetricCloudInfo() {
        NodeType = "Rendering/PostProcess/VolumetricCloud";
        inputs = {
            "RenderedImage",   // Rendered image to apply volumetric cloud to
            "VolumetricCloudParams"  // Volumetric cloud parameters
        };
        outputs = {
            "VolumetricCloudImage",  // Volumetric cloud-applied image
            "VolumetricCloudMetrics"  // Volumetric cloud performance metrics
        };
    }
};

class VolumetricCloud : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit VolumetricCloud(const VolumetricCloudInfo& info = VolumetricCloudInfo())
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
    ~VolumetricCloud() = default;     // Default destructor
};

} // namespace hd
