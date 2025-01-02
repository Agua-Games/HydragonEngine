/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Rendering system type definitions
 */

#pragma once
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector4.h"
#include "Core/Math/Matrix4.h"
#include <cstdint>

namespace Hydragon::Graphics {

using MaterialHandle = uint32_t;
using TextureHandle = uint32_t;
using BufferHandle = uint32_t;
using RenderTargetHandle = uint32_t;

enum class RenderCommandType {
    Draw,
    Dispatch,
    CopyBuffer,
    CopyTexture,
    ClearTarget
};

enum class TextureFormat {
    R8G8B8A8_UNORM,
    R16G16B16A16_FLOAT,
    R32G32B32A32_FLOAT,
    D24S8,
    D32_FLOAT
};

struct Viewport {
    float x, y;
    float width, height;
    float minDepth;
    float maxDepth;
};

struct MaterialDesc {
    const char* shaderPath;
    bool enableBlending;
    bool depthWrite;
    bool depthTest;
    uint32_t renderQueue;
};

struct TextureDesc {
    uint32_t width;
    uint32_t height;
    TextureFormat format;
    bool generateMips;
    bool dynamic;
};

struct RenderCommand {
    RenderCommandType type;
    MaterialHandle material;
    BufferHandle vertexBuffer;
    BufferHandle indexBuffer;
    uint32_t vertexCount;
    uint32_t instanceCount;
    Matrix4 transform;
};

struct RenderStats {
    uint32_t drawCalls;
    uint32_t triangles;
    uint32_t vertices;
    uint32_t instances;
    size_t gpuMemoryUsed;
    float frameTimeMs;
};

} // namespace Hydragon::Graphics 