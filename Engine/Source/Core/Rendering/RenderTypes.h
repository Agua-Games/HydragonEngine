/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Render system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Rendering {

using ResourceHandle = uint64_t;
using PipelineHandle = uint64_t;
using BufferHandle = uint64_t;
using TextureHandle = uint64_t;

enum class ResourceType {
    Buffer,
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
    RenderTarget,
    DepthStencil,
    AccelerationStructure
};

enum class ShaderStage {
    Vertex,
    Hull,
    Domain,
    Geometry,
    Pixel,
    Compute,
    Mesh,
    Amplification,
    RayGen,
    RayMiss,
    RayClosestHit,
    RayAnyHit
};

struct ResourceDesc {
    ResourceType type;
    uint32_t width = 1;
    uint32_t height = 1;
    uint32_t depth = 1;
    uint32_t mipLevels = 1;
    uint32_t arraySize = 1;
    Format format = Format::Unknown;
    ResourceFlags flags = ResourceFlags::None;
};

struct PipelineDesc {
    std::vector<ShaderDesc> shaders;
    VertexLayoutDesc vertexLayout;
    RasterStateDesc rasterState;
    DepthStencilDesc depthStencil;
    BlendStateDesc blendState;
    PrimitiveTopology topology;
};

struct RenderStats {
    uint32_t drawCalls;
    uint32_t triangles;
    uint32_t vertices;
    uint32_t shaderBindings;
    uint32_t resourceUpdates;
    size_t gpuMemoryUsed;
    float frameTimeMs;
};

} // namespace Hydragon::Rendering 