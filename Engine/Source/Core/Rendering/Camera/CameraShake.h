/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CameraShake.h
 * @brief Header file for the CameraShake class.
 * 
 * ARCHITECTURAL NOTES:
 * - CameraShake is a class that represents a camera shake effect in the engine's node graph.
 * - It is used to apply a camera shake effect to rendered images.
 * 
 */
#pragma once
#include "Node.h"
#include "ProceduralAnim.h"

namespace hd {

struct CameraShakeInfo : public NodeInfo {
    CameraShakeInfo() {
        NodeType = "Rendering/CameraShake";
        inputs = {
            "RenderedImage",   // Rendered image to apply camera shake to
            "CameraShakeParams"  // Camera shake parameters
        };
        outputs = {
            "CameraShakenImage",  // Camera shake-applied image
            "CameraShakeMetrics"  // Camera shake performance metrics
        };
    }
};

class CameraShake : public ProceduralAnim {
public:
    // === Allocation, Initialization, Loading === 
    explicit CameraShake(const CameraShakeInfo& info = CameraShakeInfo())
        : ProceduralAnim(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void setCameraShake(float intensity, float frequency);
    void setCameraShakeParams(float intensity, float frequency);
    void processCameraShake();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CameraShake() = default;     // Default destructor
};

} // namespace hd
