/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PostProcess.h
 * @brief Header file for the PostProcess class.
 * 
 * ARCHITECTURAL NOTES:
 * - PostProcess is a class that represents a post-processing effect in the engine's node graph.
 * - It is used to apply post-processing effects to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsTypes.h"

namespace hd {

struct PostProcessInfo : public NodeInfo {
    PostProcessInfo() {
        NodeType = "Rendering/PostProcess";
        inputs = {
            "RenderedImage",             // Rendered image to apply post-processing to
            "Intensity",                 // Post-processing intensity
            "PostProcessParams"          // Post-processing parameters
        };
        outputs = {
            "ProcessedImage",            // Post-processed image
            "PostProcessMetrics"         // Post-processing performance metrics
        };
    }
};

class PostProcess : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit PostProcess(const PostProcessInfo& info = PostProcessInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    std::unordered_map<std::string, std::any> PostProcessParams;
    std::unordered_map<std::string, std::any> PostProcessMetrics;

    float Intensity = 1.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~PostProcess() = default;     // Default destructor
};

} // namespace hd
