/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RenderPass.h
 * @brief Header file for the RenderPass class.
 * 
 * ARCHITECTURAL NOTES:
 * - RenderPass is a class that represents a render pass in the engine's node graph.
 * - It is used to define and manage rendering passes.
 * 
 */
#pragma once 
#include <string>
#include <unordered_map>
#include "Node.h"

namespace hd {

struct RenderPassInfo : public NodeInfo {
    RenderPassInfo() {
        NodeType = "Rendering/RenderPass";
        inputs = {
            "RenderTargets",   // Render targets to render to
            "RenderSettings"  // Render settings
        };
        outputs = {
            "RenderedImage",  // Rendered image
            "RenderMetrics"  // Render performance metrics
        };
    }
};

class RenderPass : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit RenderPass(const RenderPassInfo& info = RenderPassInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RenderPass() = default;     // Default destructor
};

} // namespace hd
