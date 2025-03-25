/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VoxelGI.h
 * @brief Header file for the VoxelGI class.
 * 
 * ARCHITECTURAL NOTES:
 * - VoxelGI is a class that represents a voxel global illumination effect in the engine's node graph.
 * - It is used to apply a voxel global illumination effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct VoxelGIInfo : public PostProcessInfo {
    VoxelGIInfo() {
        NodeType = "Rendering/PostProcess/VoxelGI";
        inputs = {
            "RenderedImage",   // Rendered image to apply voxel global illumination to
            "VoxelGIParams"  // Voxel global illumination parameters
        };
        outputs = {
            "VoxelGIedImage",  // Voxel global illumination-applied image
            "VoxelGIMetrics"  // Voxel global illumination performance metrics
        };
    }
};

class VoxelGI : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit VoxelGI(const VoxelGIInfo& info = VoxelGIInfo())
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
    ~VoxelGI() = default;     // Default destructor
};

} // namespace hd
