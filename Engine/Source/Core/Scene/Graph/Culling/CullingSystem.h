/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * View frustum and occlusion culling
 */

#pragma once
#include "../SceneGraphTypes.h"
#include <memory>
#include <vector>

namespace Hydragon::Scene {

class ICullingSystem {
public:
    virtual ~ICullingSystem() = default;
    
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    
    virtual void UpdateFrustum(const Frustum& frustum) = 0;
    virtual void UpdateOccluders(const std::vector<NodeHandle>& occluders) = 0;
    
    virtual std::vector<NodeHandle> CullNodes(const std::vector<NodeHandle>& nodes) = 0;
    virtual bool IsVisible(NodeHandle node) const = 0;
    
    virtual const CullingStats& GetStats() const = 0;
};

class CullingSystem : public ICullingSystem {
public:
    void Initialize() override;
    void Shutdown() override;
    
    void UpdateFrustum(const Frustum& frustum) override;
    void UpdateOccluders(const std::vector<NodeHandle>& occluders) override;
    
    std::vector<NodeHandle> CullNodes(const std::vector<NodeHandle>& nodes) override;
    bool IsVisible(NodeHandle node) const override;
    
    const CullingStats& GetStats() const override;

private:
    Frustum m_CurrentFrustum;
    std::vector<NodeHandle> m_Occluders;
    std::unique_ptr<IOcclusionQuery> m_OcclusionQuery;
    CullingStats m_Stats;
};

} // namespace Hydragon::Scene 