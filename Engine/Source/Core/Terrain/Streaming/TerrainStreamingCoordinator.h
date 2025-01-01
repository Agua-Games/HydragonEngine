/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Coordinates streaming between terrain subsystems
 */

#pragma once
#include "TerrainStreamingSystem.h"
#include "Core/Terrain/Rendering/TerrainMeshletSystem.h"
#include "Core/Terrain/Streaming/TerrainVirtualTexturing.h"
#include "Core/Assets/AssetSystem.h"
#include "Core/Assets/HotReload/AssetHotReloadManager.h"
#include "Core/Collaboration/CollaborationManager.h"
#include "Core/Data/AttributeSystem.h"

namespace Hydragon {
namespace Terrain {

class TerrainStreamingCoordinator {
public:
    struct CoordinatorConfig {
        StreamingConfig streamingConfig;
        MeshletConfig meshletConfig;
        VirtualTextureConfig textureConfig;
        float meshletStreamingDistance = 1000.0f;
        float textureStreamingDistance = 2000.0f;
        uint32_t maxConcurrentOperations = 4;
    };

    TerrainStreamingCoordinator(IRenderDevice* device, const CoordinatorConfig& config);

    // Main update function
    void update(const Camera& camera);

    // Streaming control
    Result<void> streamRegion(const StreamingRegion& region);
    void setStreamingPriority(const Vector3& position, float priority);
    void pauseStreaming();
    void resumeStreaming();

    // Status and debug
    bool isRegionFullyLoaded(uint32_t regionId) const;
    float getLoadProgress(uint32_t regionId) const;
    void drawDebugInfo(CommandBuffer* cmd);

    // Asset integration
    void handleAssetReload(const AssetReloadEvent& event);
    void registerStreamingAssets(AssetSystem& assetSystem);
    
    // Collaboration features
    void handleCollaboratorUpdate(const CollaboratorUpdate& update);
    void broadcastStreamingState();
    
    // Attribute system integration
    void registerAttributes(AttributeSystem& attributes);
    
    // Enhanced streaming control
    void setStreamingBudget(size_t budgetMB);
    void setPrioritizedRegion(const BoundingBox& region, float priority);
    void setQualityLevel(StreamingQuality quality);
    
    // Statistics and monitoring
    struct StreamingStats {
        float averageLoadTime;
        size_t peakMemoryUsage;
        uint32_t activeStreamingTasks;
        uint32_t pendingRequests;
        float bandwidthUtilization;
        std::vector<RegionStreamingInfo> regionInfo;
    };
    
    const StreamingStats& getStats() const { return m_Stats; }

private:
    struct StreamingTask {
        uint32_t regionId;
        bool needsMeshlets;
        bool needsTextures;
        float priority;
    };

    TerrainStreamingSystem m_StreamingSystem;
    TerrainMeshletSystem m_MeshletSystem;
    TerrainVirtualTexturing m_TextureSystem;
    
    std::vector<StreamingTask> m_PendingTasks;
    bool m_IsPaused = false;

    AssetSystem* m_AssetSystem = nullptr;
    CollaborationManager* m_CollabManager = nullptr;
    AttributeSystem* m_AttributeSystem = nullptr;
    
    StreamingStats m_Stats;
    StreamingQuality m_QualityLevel = StreamingQuality::High;
    BoundingBox m_PrioritizedRegion;
    float m_RegionPriority = 1.0f;

    void updateStreamingTasks(const Camera& camera);
    void processStreamingTasks();
    void synchronizeSystems();
    void handleStreamingFailure(const StreamingError& error);
    void updateStreamingStats();
    void notifyCollaborators(const StreamingEvent& event);
    void handleAssetDependencies(uint32_t regionId);
    float calculateRegionPriority(const BoundingBox& bounds, float distance);
};

}} // namespace Hydragon::Terrain 