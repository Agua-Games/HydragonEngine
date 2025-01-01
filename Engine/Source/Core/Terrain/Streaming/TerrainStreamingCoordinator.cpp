/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#include "TerrainStreamingCoordinator.h"
#include "Core/Profiling/Profiler.h"

namespace Hydragon {
namespace Terrain {

TerrainStreamingCoordinator::TerrainStreamingCoordinator(
    IRenderDevice* device, const CoordinatorConfig& config)
    : m_StreamingSystem(config.streamingConfig)
    , m_MeshletSystem(device, config.meshletConfig)
    , m_TextureSystem(device, config.textureConfig) {
    
    HD_PROFILE_SCOPE("TerrainStreamingCoordinator::Initialize");
}

void TerrainStreamingCoordinator::update(const Camera& camera) {
    HD_PROFILE_SCOPE("TerrainStreamingCoordinator::Update");

    if (m_IsPaused) return;

    // Update view-dependent information
    updateStreamingTasks(camera);

    // Process pending streaming operations
    processStreamingTasks();

    // Keep systems in sync
    synchronizeSystems();
}

void TerrainStreamingCoordinator::updateStreamingTasks(const Camera& camera) {
    const auto& frustum = camera.getFrustum();
    const auto& position = camera.getPosition();

    // Clear completed tasks
    m_PendingTasks.erase(
        std::remove_if(m_PendingTasks.begin(), m_PendingTasks.end(),
            [this](const StreamingTask& task) {
                return isRegionFullyLoaded(task.regionId);
            }), 
        m_PendingTasks.end());

    // Update priorities based on distance
    for (auto& task : m_PendingTasks) {
        const auto& region = m_StreamingSystem.getRegion(task.regionId);
        float distance = region.bounds.getDistance(position);
        
        // Determine streaming requirements
        task.needsMeshlets = distance <= m_Config.meshletStreamingDistance;
        task.needsTextures = distance <= m_Config.textureStreamingDistance;
        
        // Calculate priority
        task.priority = calculateStreamingPriority(distance, region.bounds);
    }

    // Sort by priority
    std::sort(m_PendingTasks.begin(), m_PendingTasks.end(),
        [](const StreamingTask& a, const StreamingTask& b) {
            return a.priority > b.priority;
        });
}

void TerrainStreamingCoordinator::processStreamingTasks() {
    HD_PROFILE_SCOPE("TerrainStreamingCoordinator::ProcessTasks");

    uint32_t processedTasks = 0;
    for (const auto& task : m_PendingTasks) {
        if (processedTasks >= m_Config.maxConcurrentOperations) break;

        try {
            if (task.needsMeshlets) {
                auto result = m_MeshletSystem.streamMeshlets(
                    m_StreamingSystem.getRegion(task.regionId));
                if (!result) {
                    handleStreamingFailure(result.error());
                    continue;
                }
            }

            if (task.needsTextures) {
                auto result = m_TextureSystem.streamRegion(
                    m_StreamingSystem.getRegion(task.regionId));
                if (!result) {
                    handleStreamingFailure(result.error());
                    continue;
                }
            }

            processedTasks++;
        } catch (const std::exception& e) {
            HD_LOG_ERROR("Streaming task failed: {}", e.what());
        }
    }
}

void TerrainStreamingCoordinator::synchronizeSystems() {
    // Ensure memory budgets are balanced
    size_t totalMemoryUsed = 
        m_MeshletSystem.getResidentMemory() +
        m_TextureSystem.getResidentMemory();

    if (totalMemoryUsed > m_Config.streamingConfig.streamingBudgetMB * 1024 * 1024) {
        // Trigger emergency cleanup
        m_MeshletSystem.evictDistantMeshlets();
        m_TextureSystem.evictPages(16);
    }

    // Synchronize streaming states
    for (const auto& region : m_StreamingSystem.getActiveRegions()) {
        if (m_MeshletSystem.isRegionLoaded(region.id) && 
            m_TextureSystem.isRegionLoaded(region.id)) {
            m_StreamingSystem.markRegionReady(region.id);
        }
    }
}

void TerrainStreamingCoordinator::handleStreamingFailure(const StreamingError& error) {
    HD_LOG_ERROR("Streaming error: {} (Code: {})", 
                 error.what(), static_cast<int>(error.code()));

    switch (error.code()) {
        case StreamingErrorCode::OutOfMemory:
            // Trigger emergency cleanup
            m_MeshletSystem.evictDistantMeshlets();
            m_TextureSystem.evictPages(32);
            break;

        case StreamingErrorCode::DeviceNotSupported:
            // Fall back to lower quality mode
            m_Config.meshletConfig.enableCulling = false;
            m_Config.textureConfig.enableCompression = false;
            break;

        default:
            // Log and continue
            break;
    }
}

void TerrainStreamingCoordinator::handleAssetReload(const AssetReloadEvent& event) {
    HD_PROFILE_SCOPE("TerrainStreamingCoordinator::HandleAssetReload");

    // Check if this is a terrain asset
    if (event.assetType != AssetType::Terrain) return;

    // Find affected regions
    std::vector<uint32_t> affectedRegions;
    for (const auto& region : m_StreamingSystem.getActiveRegions()) {
        if (region.assetId == event.assetId) {
            affectedRegions.push_back(region.id);
        }
    }

    // Reload affected regions
    for (auto regionId : affectedRegions) {
        // Unload current data
        m_MeshletSystem.unloadMeshlets(regionId);
        m_TextureSystem.unloadRegion(regionId);

        // Queue for restreaming
        StreamingTask task;
        task.regionId = regionId;
        task.needsMeshlets = true;
        task.needsTextures = true;
        task.priority = 1.0f;  // High priority for reloads
        m_PendingTasks.push_back(task);
    }
}

void TerrainStreamingCoordinator::handleCollaboratorUpdate(const CollaboratorUpdate& update) {
    if (update.type != UpdateType::TerrainModification) return;

    // Parse terrain modification data
    TerrainModification mod = parseModificationData(update.data);
    
    // Find affected regions
    auto affectedRegions = findAffectedRegions(mod.bounds);
    
    for (auto& region : affectedRegions) {
        // Mark region as modified
        region.version++;
        region.lastModified = Time::now();
        
        // Notify other systems
        m_MeshletSystem.invalidateRegion(region.id);
        m_TextureSystem.invalidateRegion(region.id);
        
        // Queue for restreaming if in view
        if (isRegionInView(region.id)) {
            queueRegionRestream(region.id);
        }
    }

    // Broadcast update to other collaborators
    broadcastStreamingState();
}

void TerrainStreamingCoordinator::setQualityLevel(StreamingQuality quality) {
    if (m_QualityLevel == quality) return;
    
    m_QualityLevel = quality;
    
    // Update configuration based on quality level
    switch (quality) {
        case StreamingQuality::Low:
            m_Config.meshletConfig.maxMeshlets /= 2;
            m_Config.textureConfig.enableCompression = true;
            m_Config.meshletStreamingDistance *= 0.5f;
            break;
            
        case StreamingQuality::High:
            m_Config.meshletConfig.maxMeshlets *= 2;
            m_Config.textureConfig.enableCompression = false;
            m_Config.meshletStreamingDistance *= 1.5f;
            break;
    }

    // Notify systems of quality change
    m_MeshletSystem.updateQualitySettings(m_Config.meshletConfig);
    m_TextureSystem.updateQualitySettings(m_Config.textureConfig);
}

void TerrainStreamingCoordinator::updateStreamingStats() {
    HD_PROFILE_SCOPE("TerrainStreamingCoordinator::UpdateStats");

    StreamingStats stats;
    
    // Gather system-specific stats
    stats.activeStreamingTasks = m_PendingTasks.size();
    stats.pendingRequests = m_StreamingSystem.getPendingRequestCount();
    
    // Calculate memory usage
    stats.peakMemoryUsage = std::max(stats.peakMemoryUsage,
        m_MeshletSystem.getResidentMemory() + 
        m_TextureSystem.getResidentMemory());
        
    // Calculate average load times
    stats.averageLoadTime = calculateAverageLoadTime();
    
    // Update bandwidth utilization
    stats.bandwidthUtilization = calculateBandwidthUtilization();
    
    // Gather per-region info
    for (const auto& region : m_StreamingSystem.getActiveRegions()) {
        RegionStreamingInfo info;
        info.regionId = region.id;
        info.loadProgress = getLoadProgress(region.id);
        info.memoryUsage = calculateRegionMemoryUsage(region);
        stats.regionInfo.push_back(info);
    }
    
    m_Stats = stats;
}

}} // namespace Hydragon::Terrain 