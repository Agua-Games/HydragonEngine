/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Terrain meshlet system for efficient mesh rendering
 */

#pragma once
#include "Core/Graphics/Meshlet/MeshletSystem.h"
#include "Core/Memory/Virtualization/VirtualMemoryManager.h"
#include "Core/Graphics/RenderAPI/IRenderDevice.h"
#include "Core/Math/BoundingBox.h"
#include "TerrainTypes.h"

namespace Hydragon {
namespace Terrain {

class TerrainMeshletSystem {
public:
    static constexpr uint32_t INVALID_MESHLET = ~0u;
    
    struct MeshletConfig {
        uint32_t maxVertices = 64;
        uint32_t maxPrimitives = 126;
        uint32_t maxMeshlets = 32768;
        bool enableCulling = true;
        float lodThreshold = 0.75f;
    };

    TerrainMeshletSystem(IRenderDevice* device, const MeshletConfig& config);
    ~TerrainMeshletSystem();  // Added destructor
    
    // Prevent copying
    TerrainMeshletSystem(const TerrainMeshletSystem&) = delete;
    TerrainMeshletSystem& operator=(const TerrainMeshletSystem&) = delete;

    // Core meshlet operations
    Result<void> buildMeshlets(const TerrainChunk& chunk);
    void updateLODs(const Camera& camera);
    void drawMeshlets(CommandBuffer* cmd);
    
    // Streaming integration
    void streamMeshlets(const StreamingRegion& region);
    void unloadMeshlets(const StreamingRegion& region);
    
    // Debug visualization
    void drawMeshletBounds(CommandBuffer* cmd);
    void drawLODTransitions(CommandBuffer* cmd);

private:
    struct TerrainMeshlet {
        uint32_t vertexOffset;
        uint32_t primitiveOffset;
        uint32_t vertexCount;
        uint32_t primitiveCount;
        float lodDistance;
        BoundingBox bounds;
        bool isVisible = true;
    };

    IRenderDevice* m_Device;
    MeshletConfig m_Config;
    std::vector<TerrainMeshlet> m_Meshlets;
    
    // GPU resources
    BufferHandle m_MeshletBuffer = nullptr;
    BufferHandle m_VertexBuffer = nullptr;
    BufferHandle m_PrimitiveBuffer = nullptr;

    // Tracking offsets
    uint32_t m_CurrentVertexOffset = 0;
    uint32_t m_CurrentPrimitiveOffset = 0;
    
    // Helper functions
    Result<void> buildMeshletBuffers();
    void updateMeshletVisibility(const Camera& camera);
    Result<TerrainMeshlet> generateMeshlet(const TerrainPatch& patch);
    BoundingBox calculateBounds(const std::vector<TerrainVertex>& vertices);
    float calculateLODDistance(const BoundingBox& bounds) const;
    void cleanupResources();

    // Error checking
    bool validateConfig() const;
    bool validateBufferCapacity(size_t vertexCount, size_t indexCount) const;
};

}} // namespace Hydragon::Terrain 