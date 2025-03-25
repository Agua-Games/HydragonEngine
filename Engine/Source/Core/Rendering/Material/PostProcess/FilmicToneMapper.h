/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FilmicToneMapper.h
 * @brief Header file for the FilmicToneMapper class.
 * 
 * ARCHITECTURAL NOTES:
 * - FilmicToneMapper is a class that represents a filmic tone mapping effect in the engine's node graph.
 * - It is used to apply a filmic tone mapping effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct FilmicToneMapperInfo : public PostProcessInfo {
    FilmicToneMapperInfo() {
        NodeType = "Rendering/PostProcess/FilmicToneMapper";
        inputs = {
            "RenderedImage",   // Rendered image to apply filmic tone mapping to
            "FilmicToneMapperParams"  // Filmic tone mapping parameters
        };
        outputs = {
            "FilmicToneMappedImage",  // Filmic tone mapped image
            "FilmicToneMapperMetrics"  // Filmic tone mapping performance metrics
        };
    }
};

class FilmicToneMapper : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit FilmicToneMapper(const FilmicToneMapperInfo& info = FilmicToneMapperInfo())
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
    ~FilmicToneMapper() = default;     // Default destructor
};

} // namespace hd
