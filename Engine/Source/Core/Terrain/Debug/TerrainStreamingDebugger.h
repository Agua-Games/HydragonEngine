/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#pragma once
#include "Core/Graphics/RenderAPI/CommandBuffer.h"
#include "Core/Debug/Visualization/DebugRenderer.h"
#include "Core/Memory/GPU/GPUMemoryManager.h"

namespace Hydragon {
namespace Terrain {

class TerrainStreamingDebugger {
public:
    struct DebugConfig {
        bool showRegionBounds = true;
        bool showStreamingStatus = true;
        bool showMemoryUsage = true;
        bool showBandwidthGraph = true;
        Vector4 loadedColor = {0.0f, 1.0f, 0.0f, 0.3f};
        Vector4 streamingColor = {1.0f, 1.0f, 0.0f, 0.3f};
        Vector4 unloadedColor = {1.0f, 0.0f, 0.0f, 0.3f};
    };

    TerrainStreamingDebugger(IRenderDevice* device);

    void drawDebugInfo(CommandBuffer* cmd, const StreamingStats& stats);
    void drawMemoryGraph(CommandBuffer* cmd, const GPUMemoryManager& memoryManager);
    void drawStreamingOverlay(CommandBuffer* cmd, const std::vector<RegionStreamingInfo>& regions);

private:
    struct DebugDrawData {
        std::vector<Vector3> vertices;
        std::vector<uint32_t> indices;
        BufferHandle vertexBuffer;
        BufferHandle indexBuffer;
    };

    IRenderDevice* m_Device;
    DebugConfig m_Config;
    DebugDrawData m_DrawData;
    DebugRenderer m_DebugRenderer;

    void drawRegionBounds(CommandBuffer* cmd, const RegionStreamingInfo& region);
    void drawMemoryUsageBar(CommandBuffer* cmd, const StreamingStats& stats);
    void drawBandwidthGraph(CommandBuffer* cmd, const StreamingStats& stats);
    void updateDebugMesh(const std::vector<RegionStreamingInfo>& regions);
};

}} // namespace Hydragon::Terrain 