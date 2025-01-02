/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene graph and spatial organization system
 */

#pragma once
#include "SceneGraphTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::Scene {

class SceneGraphSystem {
public:
    struct GraphConfig {
        bool enableDynamicUpdates = true;
        bool enableSpatialHashing = true;
        bool enableFrustumCulling = true;
        bool enableOcclusionCulling = true;
        uint32_t maxNodes = 100000;
        uint32_t maxLayers = 32;
        size_t memoryBudget = 256 * 1024 * 1024;  // 256MB
    };

    static SceneGraphSystem& Get();
    
    void Initialize(const GraphConfig& config = {});
    void Shutdown();
    
    void Update();
    
    NodeHandle CreateNode(const NodeDesc& desc);
    void DestroyNode(NodeHandle handle);
    
    void SetParent(NodeHandle node, NodeHandle parent);
    void RemoveParent(NodeHandle node);
    
    void SetTransform(NodeHandle node, const Transform& transform);
    Transform GetWorldTransform(NodeHandle node) const;
    
    void SetBounds(NodeHandle node, const BoundingVolume& bounds);
    const BoundingVolume& GetBounds(NodeHandle node) const;
    
    void SetLayer(NodeHandle node, uint32_t layer);
    void SetVisibility(NodeHandle node, bool visible);
    
    std::vector<NodeHandle> GetVisibleNodes(const Frustum& frustum) const;
    std::vector<NodeHandle> GetNodesInVolume(const BoundingVolume& volume) const;
    
    const GraphStats& GetStats() const { return m_Stats; }

private:
    SceneGraphSystem() = default;
    
    GraphConfig m_Config;
    GraphStats m_Stats;
    std::unique_ptr<ISpatialStructure> m_SpatialStructure;
    std::vector<Node> m_Nodes;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scene 