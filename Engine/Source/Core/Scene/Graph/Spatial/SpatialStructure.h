/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Spatial acceleration structure interface
 */

#pragma once
#include "../SceneGraphTypes.h"
#include <memory>
#include <vector>

namespace Hydragon::Scene {

class ISpatialStructure {
public:
    virtual ~ISpatialStructure() = default;
    
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    
    virtual void Insert(NodeHandle handle, const BoundingVolume& bounds) = 0;
    virtual void Remove(NodeHandle handle) = 0;
    virtual void Update(NodeHandle handle, const BoundingVolume& bounds) = 0;
    
    virtual std::vector<NodeHandle> Query(const BoundingVolume& volume) const = 0;
    virtual std::vector<NodeHandle> QueryFrustum(const Frustum& frustum) const = 0;
    
    virtual void Optimize() = 0;
    virtual void Clear() = 0;
    
    virtual const SpatialStats& GetStats() const = 0;
};

class OctreeSpatialStructure : public ISpatialStructure {
public:
    void Initialize() override;
    void Shutdown() override;
    
    void Insert(NodeHandle handle, const BoundingVolume& bounds) override;
    void Remove(NodeHandle handle) override;
    void Update(NodeHandle handle, const BoundingVolume& bounds) override;
    
    std::vector<NodeHandle> Query(const BoundingVolume& volume) const override;
    std::vector<NodeHandle> QueryFrustum(const Frustum& frustum) const override;
    
    void Optimize() override;
    void Clear() override;
    
    const SpatialStats& GetStats() const override;

private:
    struct OctreeNode {
        BoundingBox bounds;
        std::vector<NodeHandle> objects;
        std::array<std::unique_ptr<OctreeNode>, 8> children;
        bool isLeaf = true;
    };

    std::unique_ptr<OctreeNode> m_Root;
    SpatialStats m_Stats;
};

} // namespace Hydragon::Scene 