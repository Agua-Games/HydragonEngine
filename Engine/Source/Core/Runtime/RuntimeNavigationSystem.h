/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime navigation and pathfinding management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeNavigationConfig {
    bool enableDynamicObstacles = true;
    bool enableAreaModifiers = true;
    bool enableCrowdSimulation = true;
    bool enableAsyncPathfinding = true;
    uint32_t maxPathRequests = 1000;
    uint32_t maxCrowdAgents = 500;
    uint32_t maxAreaModifiers = 100;
    float tileSize = 1.0f;
    std::string navMeshCachePath = "Cache/Navigation";
};

class RuntimeNavigationSystem {
public:
    static RuntimeNavigationSystem& Get();
    
    void Initialize(const RuntimeNavigationConfig& config = {});
    void Shutdown();
    
    void Update();
    
    NavMeshHandle LoadNavMesh(const std::string& path);
    void UnloadNavMesh(NavMeshHandle handle);
    void BuildNavMesh(const NavMeshBuildSettings& settings);
    
    PathRequestHandle RequestPath(const Vector3& start, const Vector3& end, const PathfindingParams& params = {});
    void CancelPathRequest(PathRequestHandle handle);
    
    CrowdAgentHandle CreateCrowdAgent(const CrowdAgentDesc& desc);
    void DestroyCrowdAgent(CrowdAgentHandle handle);
    void SetAgentDestination(CrowdAgentHandle handle, const Vector3& destination);
    
    void AddDynamicObstacle(const Vector3& position, float radius);
    void RemoveDynamicObstacle(ObstacleHandle handle);
    
    void AddAreaModifier(const AreaModifierDesc& desc);
    void RemoveAreaModifier(AreaModifierHandle handle);
    void UpdateAreaModifier(AreaModifierHandle handle, float cost);
    
    void RegisterNavigationFilter(const std::string& name, std::unique_ptr<INavigationFilter> filter);
    void UnregisterNavigationFilter(const std::string& name);
    
    PathStatus GetPathStatus(PathRequestHandle handle) const;
    std::vector<Vector3> GetPathPoints(PathRequestHandle handle) const;
    float GetPathLength(PathRequestHandle handle) const;
    const RuntimeNavigationStats& GetStats() const { return m_Stats; }

private:
    RuntimeNavigationSystem() = default;
    
    RuntimeNavigationConfig m_Config;
    RuntimeNavigationStats m_Stats;
    std::unordered_map<NavMeshHandle, NavMeshInstance> m_NavMeshes;
    std::unordered_map<PathRequestHandle, PathRequest> m_PathRequests;
    std::unordered_map<CrowdAgentHandle, CrowdAgent> m_CrowdAgents;
    std::unordered_map<std::string, std::unique_ptr<INavigationFilter>> m_Filters;
    std::unique_ptr<IPathfinder> m_Pathfinder;
    std::unique_ptr<ICrowdManager> m_CrowdManager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 