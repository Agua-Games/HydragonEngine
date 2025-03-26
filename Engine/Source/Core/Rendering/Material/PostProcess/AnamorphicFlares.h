/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AnamorphicFlares.h
 * @brief Header file for the AnamorphicFlares class.
 * 
 * ARCHITECTURAL NOTES:
 * - AnamorphicFlares is a class that represents an anamorphic flares effect in the engine's node graph.
 * - It is used to apply an anamorphic flares effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct AnamorphicFlaresInfo : public PostProcessInfo {
    AnamorphicFlaresInfo() {
        NodeType = "Rendering/PostProcess/AnamorphicFlares";
        inputs = {
            "RenderedImage",   // Rendered image to apply anamorphic flares to
            "AnamorphicFlaresParams"  // Anamorphic flares parameters
        };
        outputs = {
            "AnamorphicFlaresedImage",  // Anamorphic flares-applied image
            "AnamorphicFlaresMetrics"  // Anamorphic flares performance metrics
        };
    }
};

class AnamorphicFlares : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit AnamorphicFlares(const AnamorphicFlaresInfo& info = AnamorphicFlaresInfo())
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
    ~AnamorphicFlares() = default;     // Default destructor
};

} // namespace hd
