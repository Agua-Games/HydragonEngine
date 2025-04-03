/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ShaderCompiler.h
 * @brief Header file for the ShaderCompiler class.
 * 
 * ARCHITECTURAL NOTES:
 * - ShaderCompiler is a class that represents a shader compiler in the engine's node graph.
 * - It is used to compile and manage shader resources.
 * - As we mostly use GLSL for shaders, we use Vulkan-SDK's tools for compiling shaders to SPIR-V. So this class is mostly a wrapper for those tools.
 * - It is also a base class for all shader compilers. This will allow us a flexible approach for compiling shaders, like we usually see in DCC apps, where we can easily switch 
 * between different shader compilers. Also better for prototyping new shader compilers, in a modular way. And it will allow us to switch between different shader languages, 
 * like GLSL, HLSL, etc.
 */
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Node.h"
#include "GraphicsTypes.h"

namespace hd {
    
struct ShaderCompilerInfo : public NodeInfo {
    ShaderCompilerInfo() {
        NodeType = "Graphics/ShaderCompiler";
        inputs = {
            "Shaders" // Array of shaders
        };
        outputs = {
            "CompiledShaders" // Compiled shaders
        };
    }
};
    
class ShaderCompiler : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ShaderCompiler(const ShaderCompilerInfo& info = ShaderCompilerInfo())
        : Node(info), ShaderCompilerInfo(info) {}
    initialize() override {}
    load() override {}

    // Set default values.
    // Whether to enable async compilation. If enabled, it will compile shaders in the background, which can be useful for performance. Otherwise, it will compile synchronously.
    bool enableAsyncCompilation = true;
    // Maximum number of concurrent shader compilation tasks. This is only relevant if async compilation is enabled. Otherwise, it will compile synchronously.
    int maxConcurrentTasks = 4;

    // === Processing ===
    void processNode() override {
        
    }
    void compileShaders(bol async, const std::vector<Shader>& shaders);   // Leverages async background compilation, if enabled. Otherwise, it will compile synchronously.
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ShaderCompiler() = default;     // Default destructor
};

} // namespace hd
