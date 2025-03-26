/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file CausticsReflRefraction.h
 * @brief Header file for the CausticsReflRefraction class.
 * 
 * ARCHITECTURAL NOTES:
 * - CausticsReflRefraction is a class that represents a caustics, reflection, and refraction effect in the engine's node graph.
 * - It is used to apply a caustics, reflection, and refraction effect to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "PostProcess.h"

namespace hd {

struct CausticsReflRefractionInfo : public PostProcessInfo {
    CausticsReflRefractionInfo() {
        NodeType = "Rendering/PostProcess/CausticsReflRefraction";
        inputs = {
            "RenderedImage",   // Rendered image to apply caustics, reflection, and refraction to
            "CausticsReflRefractionParams"  // Caustics, reflection, and refraction parameters
        };
        outputs = {
            "CausticsReflRefractionedImage",  // Caustics, reflection, and refraction-applied image
            "CausticsReflRefractionMetrics"  // Caustics, reflection, and refraction performance metrics
        };
    }
};

class CausticsReflRefraction : public PostProcess {
public:
    // === Allocation, Initialization, Loading === 
    explicit CausticsReflRefraction(const CausticsReflRefractionInfo& info = CausticsReflRefractionInfo())
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
    ~CausticsReflRefraction() = default;     // Default destructor
};

} // namespace hd
