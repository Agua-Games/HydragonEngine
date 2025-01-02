/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Graphics pipeline type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Graphics {

using PipelineHandle = uint32_t;
using ShaderHandle = uint32_t;

enum class ShaderStage {
    Vertex,
    Fragment,
    Compute,
    Geometry,
    TessControl,
    TessEvaluation
};

struct ShaderDesc {
    ShaderStage stage;
    std::string source;
    std::string entryPoint;
    std::vector<std::string> defines;
};

struct PipelineDesc {
    std::vector<ShaderHandle> shaders;
    VertexLayout vertexLayout;
    RasterState rasterState;
    DepthStencilState depthState;
    BlendState blendState;
};

struct PipelineState {
    bool wireframe;
    bool cullBackface;
    bool depthTest;
    bool alphaBlend;
};

struct PipelineStats {
    uint32_t activePipelines;
    uint32_t shaderCompilations;
    uint32_t pipelineBindings;
    uint32_t stateChanges;
    size_t cacheSize;
    float compileTimeMs;
};

} // namespace Hydragon::Graphics 