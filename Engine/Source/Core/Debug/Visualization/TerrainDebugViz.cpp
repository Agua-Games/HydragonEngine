#include "TerrainDebugViz.h"
#include "Core/Graphics/RenderAPI/DebugRenderer.h"
#include "Core/Memory/Profiling/MemoryProfiler.h"

namespace Hydragon {

class TerrainDebugViz::Impl {
    struct VisualizationState {
        std::vector<DebugMeshlet> meshletVisuals;
        std::vector<DebugStreamingStatus> streamingVisuals;
        MemoryStats memoryStats;
    };

    VisualizationState m_state;
    DebugRenderer m_renderer;
};

TerrainDebugViz::TerrainDebugViz()
    : m_impl(std::make_unique<Impl>()) {
    
    InitializeDebugMaterials();
}

void TerrainDebugViz::RenderMeshletBounds(
    const std::unordered_map<ChunkId, std::vector<MeshletHandle>>& meshlets) {
    
    for (const auto& [chunkId, handles] : meshlets) {
        for (const auto& handle : handles) {
            auto bounds = GetMeshletBounds(handle);
            m_impl->m_renderer.DrawAABB(bounds, GetMeshletColor(handle));
        }
    }
}

void TerrainDebugViz::RenderVertexDensityHeatmap(
    const std::unordered_map<ChunkId, std::vector<MeshletHandle>>& meshlets) {
    
    for (const auto& [chunkId, handles] : meshlets) {
        for (const auto& handle : handles) {
            float density = CalculateVertexDensity(handle);
            Color heatmapColor = MapDensityToColor(density);
            
            auto mesh = GetMeshletGeometry(handle);
            m_impl->m_renderer.DrawMesh(mesh, heatmapColor, 0.5f);
        }
    }
}

void TerrainDebugViz::UpdateMemoryStats(const MemoryStats& stats) {
    m_impl->m_state.memoryStats = stats;

    // Draw memory usage graph
    DrawMemoryGraph({
        .position = Vector2(10, 10),
        .size = Vector2(200, 100),
        .stats = stats
    });

    // Draw text overlay
    std::stringstream ss;
    ss << "Terrain Memory Usage:\n"
       << "Heightfield: " << FormatSize(stats.heightfieldMemory) << "\n"
       << "Textures: " << FormatSize(stats.textureMemory) << "\n"
       << "Active Chunks: " << stats.activeChunks << "\n"
       << "Pending: " << stats.pendingRequests;

    m_impl->m_renderer.DrawText(
        Vector2(220, 10),
        ss.str(),
        Color::White
    );
}

Color TerrainDebugViz::MapDensityToColor(float density) {
    // Create a heat gradient from blue (low) to red (high)
    const float minDensity = 0.0f;
    const float maxDensity = 1.0f;
    
    float t = (density - minDensity) / (maxDensity - minDensity);
    t = std::clamp(t, 0.0f, 1.0f);

    return Color::Lerp(Color::Blue, Color::Red, t);
}

} // namespace Hydragon 