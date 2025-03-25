/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FilmCameraPack.h
 * @brief Header file for the FilmCameraPack class.
 * 
 * ARCHITECTURAL NOTES:
 * - FilmCameraPack is a class that represents a film camera pack effect in the engine's node graph.
 * - It is used to apply a film camera pack effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct FilmCameraPackInfo : public PostProcessInfo {
    FilmCameraPackInfo() {
        NodeType = "Rendering/PostProcess/FilmCameraPack";
        inputs = {
            "RenderedImage",   // Rendered image to apply film camera pack to
            "FilmCameraPackParams"  // Film camera pack parameters
        };
        outputs = {
            "FilmCameraPackedImage",  // Film camera pack-applied image
            "FilmCameraPackMetrics"  // Film camera pack performance metrics
        };
    }
};

class FilmCameraPack : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit FilmCameraPack(const FilmCameraPackInfo& info = FilmCameraPackInfo())
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
    ~FilmCameraPack() = default;     // Default destructor
};

} // namespace hd
