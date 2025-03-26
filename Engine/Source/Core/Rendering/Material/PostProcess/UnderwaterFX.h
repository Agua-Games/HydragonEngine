/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UnderwaterFX.h
 * @brief Header file for the UnderwaterFX class.
 * 
 * ARCHITECTURAL NOTES:
 * - UnderwaterFX is a class that represents an underwater effect in the engine's node graph.
 * - It is used to apply an underwater effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct UnderwaterFXInfo : public PostProcessInfo {
    UnderwaterFXInfo() {
        NodeType = "Rendering/PostProcess/UnderwaterFX";
        inputs = {
            "RenderedImage",   // Rendered image to apply underwater effect to
            "UnderwaterFXParams"  // Underwater effect parameters
        };
        outputs = {
            "UnderwaterFXedImage",  // Underwater effect-applied image
            "UnderwaterFXMetrics"  // Underwater effect performance metrics
        };
    }
};

class UnderwaterFX : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit UnderwaterFX(const UnderwaterFXInfo& info = UnderwaterFXInfo())
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
    ~UnderwaterFX() = default;     // Default destructor
};

} // namespace hd
