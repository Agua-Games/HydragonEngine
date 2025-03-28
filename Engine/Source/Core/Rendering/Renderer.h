/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Renderer.h
 * @brief Header file for the Renderer class.
 *
 * ARCHITECTURAL NOTES:
 * - Renderer is a class that represents a renderer in the engine's node graph.
 * - It is used to define and manage rendering passes.
 * - It is also a base class for all renderers. This will allow us a flexible approach for rendering, like we usually see in DCC apps, where we can easily switch 
 * between different renderers. Also better for prototyping new renderers, in a modular way. And possibly having a lower end renderer for mobile, etc.
 * - Current implementations:
 *      - PrismaRenderer(c) (default) - Primary renderer optimized for visual quality
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Node.h"

namespace hd {

struct RendererInfo : public NodeInfo {
    RendererInfo() {
        nodeType = "Rendering/Renderer";
        inputs = {
            "renderTargets",   // Render targets to render to
            "renderSettings"  // Render settings
        };
        outputs = {
            "renderedImage",  // Rendered image
            "renderMetrics"  // Render performance metrics
        };
    }
};

class Renderer : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Renderer(const RendererInfo& info = RendererInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    virtual void render() = 0;
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Renderer() = default;     // Default destructor
};

} // namespace hd