/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Terrain meshlet system implementation
 */

#include "TerrainMeshletSystem.h"
#include "Core/Graphics/RenderAPI/CommandBuffer.h"
#include "Core/Profiling/Profiler.h"
#include "Core/Common/Result.h"

namespace Hydragon {
namespace Terrain {

TerrainMeshletSystem::TerrainMeshletSystem(IRenderDevice* device, const MeshletConfig& config)
    : m_Device(device)
    , m_Config(config) {
    
    HD_PROFILE_SCOPE("TerrainMeshletSystem::Initialize");
    
    HD_CHECK(device != nullptr, "Invalid render device");
    HD_CHECK(validateConfig(), "Invalid meshlet configuration");
    
    // Pre-allocate meshlet storage
    try {
        m_Meshlets.reserve(config.maxMeshlets);
    } catch (const std::bad_alloc&) {
        HD_LOG_ERROR("Failed to allocate meshlet storage");
        throw;
    }
    
    // Create GPU buffers
    auto result = buildMeshletBuffers();
    if (!result) {
        cleanupResources();
        HD_LOG_ERROR("Failed to create meshlet buffers: {}", result.error());
        throw std::runtime_error(result.error());
    }
}

TerrainMeshletSystem::~TerrainMeshletSystem() {
    cleanupResources();
}

void TerrainMeshletSystem::cleanupResources() {
    if (m_Device) {
        if (m_MeshletBuffer) m_Device->destroyBuffer(m_MeshletBuffer);
        if (m_VertexBuffer) m_Device->destroyBuffer(m_VertexBuffer);
        if (m_PrimitiveBuffer) m_Device->destroyBuffer(m_PrimitiveBuffer);
    }
    
    m_MeshletBuffer = nullptr;
    m_VertexBuffer = nullptr;
    m_PrimitiveBuffer = nullptr;
}

Result<void> TerrainMeshletSystem::buildMeshletBuffers() {
    // Create meshlet buffer
    BufferDesc meshletDesc;
    meshletDesc.size = m_Config.maxMeshlets * sizeof(TerrainMeshlet);
    meshletDesc.usage = BufferUsage::Storage | BufferUsage::Transfer;
    
    m_MeshletBuffer = m_Device->createBuffer(meshletDesc);
    if (!m_MeshletBuffer) {
        return Error("Failed to create meshlet buffer");
    }

    // Create vertex buffer
    BufferDesc vertexDesc;
    vertexDesc.size = m_Config.maxMeshlets * m_Config.maxVertices * sizeof(TerrainVertex);
    vertexDesc.usage = BufferUsage::Vertex | BufferUsage::Storage;
    
    m_VertexBuffer = m_Device->createBuffer(vertexDesc);
    if (!m_VertexBuffer) {
        return Error("Failed to create vertex buffer");
    }

    // Create primitive buffer
    BufferDesc primitiveDesc;
    primitiveDesc.size = m_Config.maxMeshlets * m_Config.maxPrimitives * sizeof(uint32_t) * 3;
    primitiveDesc.usage = BufferUsage::Storage | BufferUsage::Index;
    
    m_PrimitiveBuffer = m_Device->createBuffer(primitiveDesc);
    if (!m_PrimitiveBuffer) {
        return Error("Failed to create primitive buffer");
    }

    return Success;
}

Result<void> TerrainMeshletSystem::buildMeshlets(const TerrainChunk& chunk) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::BuildMeshlets");

    // Reset state
    m_Meshlets.clear();
    m_CurrentVertexOffset = 0;
    m_CurrentPrimitiveOffset = 0;

    // Generate meshlets for each patch in the chunk
    for (const auto& patch : chunk.getPatches()) {
        if (m_Meshlets.size() >= m_Config.maxMeshlets) {
            HD_LOG_WARNING("Maximum meshlet count reached");
            break;
        }

        auto result = generateMeshlet(patch);
        if (!result) {
            return Error(fmt::format("Failed to generate meshlet: {}", result.error()));
        }

        m_Meshlets.push_back(result.value());
    }

    // Upload to GPU
    if (!m_Device->uploadBufferData(m_MeshletBuffer, m_Meshlets.data(), 
                                  m_Meshlets.size() * sizeof(TerrainMeshlet))) {
        return Error("Failed to upload meshlet data to GPU");
    }

    return Success;
}

Result<TerrainMeshlet> TerrainMeshletSystem::generateMeshlet(const TerrainPatch& patch) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::GenerateMeshlet");

    TerrainMeshlet meshlet = {};
    
    // Get patch vertices
    const auto& vertices = patch.getVertices();
    const auto& indices = patch.getIndices();
    
    // Validate sizes
    if (vertices.size() > m_Config.maxVertices) {
        return Error("Vertex count exceeds maximum");
    }
    
    if (indices.size() > m_Config.maxPrimitives * 3) {
        return Error("Primitive count exceeds maximum");
    }

    // Validate buffer capacity
    if (!validateBufferCapacity(vertices.size(), indices.size())) {
        return Error("Buffer capacity exceeded");
    }

    // Setup meshlet data
    meshlet.vertexOffset = m_CurrentVertexOffset;
    meshlet.primitiveOffset = m_CurrentPrimitiveOffset;
    meshlet.vertexCount = vertices.size();
    meshlet.primitiveCount = indices.size() / 3;
    
    try {
        meshlet.bounds = calculateBounds(vertices);
        meshlet.lodDistance = calculateLODDistance(meshlet.bounds);
    } catch (const std::exception& e) {
        return Error(fmt::format("Failed to calculate meshlet bounds: {}", e.what()));
    }

    // Upload vertex and index data
    if (!m_Device->uploadBufferData(m_VertexBuffer, 
                                  vertices.data(), 
                                  vertices.size() * sizeof(TerrainVertex),
                                  meshlet.vertexOffset * sizeof(TerrainVertex))) {
        return Error("Failed to upload vertex data");
    }
                             
    if (!m_Device->uploadBufferData(m_PrimitiveBuffer,
                                  indices.data(),
                                  indices.size() * sizeof(uint32_t),
                                  meshlet.primitiveOffset * sizeof(uint32_t) * 3)) {
        return Error("Failed to upload index data");
    }

    // Update offsets
    m_CurrentVertexOffset += vertices.size();
    m_CurrentPrimitiveOffset += indices.size() / 3;

    return meshlet;
}

void TerrainMeshletSystem::updateLODs(const Camera& camera) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::UpdateLODs");

    // Update visibility and LOD selection for each meshlet
    updateMeshletVisibility(camera);

    // Sort meshlets front to back
    std::sort(m_Meshlets.begin(), m_Meshlets.end(),
        [&camera](const TerrainMeshlet& a, const TerrainMeshlet& b) {
            return camera.getDistance(a.bounds.center) < 
                   camera.getDistance(b.bounds.center);
        });
}

void TerrainMeshletSystem::drawMeshlets(CommandBuffer* cmd) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::DrawMeshlets");

    // Bind meshlet resources
    cmd->bindBuffer(0, m_MeshletBuffer);
    cmd->bindBuffer(1, m_VertexBuffer);
    cmd->bindBuffer(2, m_PrimitiveBuffer);

    // Draw meshlets using mesh shader if available
    if (m_Device->supportsMeshShaders()) {
        cmd->bindShader(m_Device->getShader("TerrainMeshlet_MS"));
        cmd->drawMeshlets(m_Meshlets.size());
    } else {
        // Fallback to traditional vertex/index drawing
        cmd->bindShader(m_Device->getShader("TerrainMeshlet_VS"));
        for (const auto& meshlet : m_Meshlets) {
            cmd->drawIndexed(meshlet.primitiveCount * 3,
                           meshlet.primitiveOffset * 3,
                           meshlet.vertexOffset);
        }
    }
}

bool TerrainMeshletSystem::validateConfig() const {
    if (m_Config.maxVertices == 0 || m_Config.maxVertices > 256) {
        HD_LOG_ERROR("Invalid maxVertices: {}", m_Config.maxVertices);
        return false;
    }

    if (m_Config.maxPrimitives == 0 || m_Config.maxPrimitives > 256) {
        HD_LOG_ERROR("Invalid maxPrimitives: {}", m_Config.maxPrimitives);
        return false;
    }

    if (m_Config.maxMeshlets == 0) {
        HD_LOG_ERROR("Invalid maxMeshlets: {}", m_Config.maxMeshlets);
        return false;
    }

    if (m_Config.lodThreshold <= 0.0f || m_Config.lodThreshold > 1.0f) {
        HD_LOG_ERROR("Invalid lodThreshold: {}", m_Config.lodThreshold);
        return false;
    }

    // Validate memory requirements
    const size_t totalVertexMemory = m_Config.maxMeshlets * m_Config.maxVertices * sizeof(TerrainVertex);
    const size_t totalIndexMemory = m_Config.maxMeshlets * m_Config.maxPrimitives * 3 * sizeof(uint32_t);
    const size_t totalMeshletMemory = m_Config.maxMeshlets * sizeof(TerrainMeshlet);
    
    const size_t totalMemoryRequired = totalVertexMemory + totalIndexMemory + totalMeshletMemory;
    
    if (totalMemoryRequired > m_Device->getMaxAllocationSize()) {
        HD_LOG_ERROR("Total memory requirement exceeds device limits: {} bytes", totalMemoryRequired);
        return false;
    }

    // Validate alignment requirements
    if (!Math::isPowerOfTwo(m_Config.maxVertices)) {
        HD_LOG_ERROR("maxVertices must be power of 2");
        return false;
    }

    return true;
}

bool TerrainMeshletSystem::validateBufferCapacity(size_t vertexCount, size_t indexCount) const {
    if (m_CurrentVertexOffset + vertexCount > m_Config.maxVertices * m_Config.maxMeshlets) {
        HD_LOG_ERROR("Vertex buffer capacity exceeded");
        return false;
    }

    if (m_CurrentPrimitiveOffset + (indexCount / 3) > m_Config.maxPrimitives * m_Config.maxMeshlets) {
        HD_LOG_ERROR("Primitive buffer capacity exceeded");
        return false;
    }

    return true;
}

void TerrainMeshletSystem::updateMeshletVisibility(const Camera& camera) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::UpdateVisibility");

    const auto& frustum = camera.getFrustum();
    const float maxDistance = camera.getFarPlane();

    for (auto& meshlet : m_Meshlets) {
        // Frustum culling
        meshlet.isVisible = m_Config.enableCulling ? 
            frustum.intersects(meshlet.bounds) : true;

        // Distance culling
        if (meshlet.isVisible) {
            float distance = camera.getDistance(meshlet.bounds.center);
            meshlet.isVisible = distance <= maxDistance;
        }
    }
}

BoundingBox TerrainMeshletSystem::calculateBounds(const std::vector<TerrainVertex>& vertices) {
    if (vertices.empty()) {
        return BoundingBox();
    }

    Vector3 min = vertices[0].position;
    Vector3 max = min;

    for (const auto& vertex : vertices) {
        min = Vector3::min(min, vertex.position);
        max = Vector3::max(max, vertex.position);
    }

    return BoundingBox(min, max);
}

float TerrainMeshletSystem::calculateLODDistance(const BoundingBox& bounds) const {
    // Calculate distance based on bounds size and LOD threshold
    float size = bounds.getSize().length();
    return size * m_Config.lodThreshold;
}

void TerrainMeshletSystem::streamMeshlets(const StreamingRegion& region) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::StreamMeshlets");

    if (!m_Device->supportsResidentBuffers()) {
        HD_LOG_WARNING("Device doesn't support resident buffers, streaming disabled");
        return;
    }

    // Track streaming stats
    StreamingStats stats;
    stats.startTime = Time::now();

    try {
        // Find meshlets that intersect with the streaming region
        std::vector<TerrainMeshlet*> meshlets;
        for (auto& meshlet : m_Meshlets) {
            if (region.bounds.intersects(meshlet.bounds)) {
                meshlets.push_back(&meshlet);
            }
        }

        // Sort by priority (distance from region center)
        std::sort(meshlets.begin(), meshlets.end(),
            [&region](const TerrainMeshlet* a, const TerrainMeshlet* b) {
                float distA = (a->bounds.center - region.center).lengthSquared();
                float distB = (b->bounds.center - region.center).lengthSquared();
                return distA < distB;
            });

        // Stream in high priority meshlets first
        for (auto* meshlet : meshlets) {
            if (!m_Device->isBufferResident(m_VertexBuffer, 
                                          meshlet->vertexOffset * sizeof(TerrainVertex),
                                          meshlet->vertexCount * sizeof(TerrainVertex))) {
                m_Device->makeBufferResident(m_VertexBuffer,
                                           meshlet->vertexOffset * sizeof(TerrainVertex),
                                           meshlet->vertexCount * sizeof(TerrainVertex));
            }
        }

        stats.endTime = Time::now();
        stats.meshletCount = meshlets.size();
        stats.totalMemory = calculateStreamingMemory(meshlets);
        
        m_StreamingHistory.push_back(stats);
        
        // Trim history
        while (m_StreamingHistory.size() > MAX_STREAMING_HISTORY) {
            m_StreamingHistory.pop_front();
        }
    } catch (const std::exception& e) {
        HD_LOG_ERROR("Streaming error: {}", e.what());
        m_LastStreamingError = e.what();
    }
}

void TerrainMeshletSystem::unloadMeshlets(const StreamingRegion& region) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::UnloadMeshlets");

    // Find meshlets outside the streaming region
    std::vector<TerrainMeshlet*> unloadCandidates;
    for (auto& meshlet : m_Meshlets) {
        if (!region.bounds.intersects(meshlet.bounds)) {
            unloadCandidates.push_back(&meshlet);
        }
    }

    // Sort by distance (furthest first)
    std::sort(unloadCandidates.begin(), unloadCandidates.end(),
        [&region](const TerrainMeshlet* a, const TerrainMeshlet* b) {
            float distA = (a->bounds.center - region.center).lengthSquared();
            float distB = (b->bounds.center - region.center).lengthSquared();
            return distA > distB;
        });

    // Unload furthest meshlets first
    for (auto* meshlet : unloadCandidates) {
        if (m_Device->isBufferResident(m_VertexBuffer,
                                     meshlet->vertexOffset * sizeof(TerrainVertex),
                                     meshlet->vertexCount * sizeof(TerrainVertex))) {
            m_Device->makeBufferNonResident(m_VertexBuffer,
                                          meshlet->vertexOffset * sizeof(TerrainVertex),
                                          meshlet->vertexCount * sizeof(TerrainVertex));
        }
    }
}

void TerrainMeshletSystem::drawMeshletBounds(CommandBuffer* cmd) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::DrawMeshletBounds");

    if (!cmd) {
        HD_LOG_ERROR("Invalid command buffer");
        return;
    }

    // Bind debug visualization shader
    auto* shader = m_Device->getShader("TerrainMeshlet_Debug");
    if (!shader) {
        HD_LOG_ERROR("Failed to get debug shader");
        return;
    }
    cmd->bindShader(shader);

    // Setup debug visualization constants
    DebugDrawConstants constants;
    constants.boundingBoxColor = m_Config.enableCulling ? 
        Vector4(0.0f, 1.0f, 0.0f, 0.3f) :  // Green for active culling
        Vector4(1.0f, 1.0f, 0.0f, 0.3f);   // Yellow for disabled culling

    cmd->pushConstants(constants);

    // Draw bounding box for each visible meshlet
    for (const auto& meshlet : m_Meshlets) {
        if (meshlet.isVisible) {
            cmd->drawBoundingBox(meshlet.bounds, constants.boundingBoxColor);
            
            // Draw LOD transition sphere
            if (m_Config.enableCulling) {
                cmd->drawSphere(meshlet.bounds.center, meshlet.lodDistance, 
                              Vector4(1.0f, 0.0f, 0.0f, 0.2f));
            }
        }
    }
}

void TerrainMeshletSystem::drawLODTransitions(CommandBuffer* cmd) {
    HD_PROFILE_SCOPE("TerrainMeshletSystem::DrawLODTransitions");

    if (!cmd) {
        HD_LOG_ERROR("Invalid command buffer");
        return;
    }

    // Bind LOD visualization shader
    auto* shader = m_Device->getShader("TerrainMeshlet_LOD");
    if (!shader) {
        HD_LOG_ERROR("Failed to get LOD shader");
        return;
    }
    cmd->bindShader(shader);

    // Setup LOD visualization constants
    LODVisualizationConstants constants;
    constants.lodColors[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);  // Highest LOD (red)
    constants.lodColors[1] = Vector4(1.0f, 1.0f, 0.0f, 1.0f);  // Medium LOD (yellow)
    constants.lodColors[2] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);  // Low LOD (green)
    constants.transitionRange = 5.0f;  // Blend range between LODs

    cmd->pushConstants(constants);

    // Draw meshlets with LOD coloring
    for (const auto& meshlet : m_Meshlets) {
        if (!meshlet.isVisible) continue;

        // Calculate LOD blend factor
        float lodFactor = calculateLODFactor(meshlet);
        Vector4 color = interpolateLODColor(lodFactor, constants);

        // Draw meshlet with LOD-based coloring
        cmd->drawMeshletWireframe(meshlet, color);

        // Draw transition boundaries
        if (abs(lodFactor - floor(lodFactor)) < 0.1f) {
            cmd->drawSphere(meshlet.bounds.center, meshlet.lodDistance,
                          Vector4(1.0f, 1.0f, 1.0f, 0.5f));
        }
    }
}

// Helper methods for LOD visualization
float TerrainMeshletSystem::calculateLODFactor(const TerrainMeshlet& meshlet) const {
    const float distance = m_CurrentCamera ? 
        m_CurrentCamera->getDistance(meshlet.bounds.center) : 0.0f;
    return distance / meshlet.lodDistance;
}

Vector4 TerrainMeshletSystem::interpolateLODColor(float factor, 
                                                const LODVisualizationConstants& constants) const {
    const int baseLOD = static_cast<int>(factor);
    const float blend = factor - baseLOD;
    
    if (baseLOD >= MAX_LOD_LEVELS - 1) {
        return constants.lodColors[MAX_LOD_LEVELS - 1];
    }
    
    return Vector4::lerp(constants.lodColors[baseLOD],
                        constants.lodColors[baseLOD + 1],
                        blend);
}

}} // namespace Hydragon::Terrain 