/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ToonMaterial.h
 * @brief Header file for the ToonMaterial class.
 * 
 * ARCHITECTURAL NOTES:
 * - ToonMaterial is a class that represents a toon material in the engine's node graph.
 * - It is used to apply a toon shading effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Material.h"

namespace hd {

struct ToonMaterialInfo : public MaterialInfo {
    ToonMaterialInfo() {
        NodeType = "Rendering/Material/Toon";
        inputs = {
            "RenderedImage",   // Rendered image to apply toon shading to
            "ToonShadingParams"  // Toon shading parameters
        };
        outputs = {
            "ToonShadedImage",  // Toon shading-applied image
            "ToonShadingMetrics"  // Toon shading performance metrics
        };
    }
};

class ToonMaterial : public Material {
public:
    // === Allocation, Initialization, Loading === 
    explicit ToonMaterial(const ToonMaterialInfo& info = ToonMaterialInfo())
        : Material(info) {}   
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
    ~ToonMaterial() = default;     // Default destructor
};

} // namespace hd
