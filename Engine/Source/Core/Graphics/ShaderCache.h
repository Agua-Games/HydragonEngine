/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ShaderCache.h
 * @brief Header file for the ShaderCache class.
 * 
 * ARCHITECTURAL NOTES:
 * - ShaderCache is a class that represents a shader cache in the engine's node graph.
 * - It is used to cache and manage shader resources.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Node.h"
#include "GraphicsTypes.h"

namespace hd {

struct ShaderCacheInfo : public NodeInfo {
    ShaderCacheInfo() {
        NodeType = "Graphics/ShaderCache";
        inputs = {
            "Shaders" // Array of shaders
        };
        outputs = {
            "ActiveShaders" // Active shaders
        };
    }
};

class ShaderCache : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ShaderCache(const ShaderCacheInfo& info = ShaderCacheInfo())
        : Node(info) {}
    ~ShaderCache() override = default;

    // === Processing ===
    void processNode() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
};

} // namespace hd