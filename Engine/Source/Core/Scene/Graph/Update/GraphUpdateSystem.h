/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene graph update and synchronization
 */

#pragma once
#include "../SceneGraphTypes.h"
#include <memory>
#include <vector>

namespace Hydragon::Scene {

class IGraphUpdateSystem {
public:
    virtual ~IGraphUpdateSystem() = default;
    
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
    
    virtual void BeginUpdate() = 0;
    virtual void UpdateTransforms() = 0;
    virtual void UpdateBounds() = 0;
    virtual void UpdateVisibility() = 0;
    virtual void EndUpdate() = 0;
    
    virtual void MarkDirty(NodeHandle node) = 0;
    virtual bool IsDirty(NodeHandle node) const = 0;
    
    virtual const UpdateStats& GetStats() const = 0;
};

class GraphUpdateSystem : public IGraphUpdateSystem {
public:
    void Initialize() override;
    void Shutdown() override;
    
    void BeginUpdate() override;
    void UpdateTransforms() override;
    void UpdateBounds() override;
    void UpdateVisibility() override;
    void EndUpdate() override;
    
    void MarkDirty(NodeHandle node) override;
    bool IsDirty(NodeHandle node) const override;
    
    const UpdateStats& GetStats() const override;

private:
    struct DirtyState {
        bool transform : 1;
        bool bounds : 1;
        bool visibility : 1;
    };

    std::vector<NodeHandle> m_DirtyNodes;
    std::unordered_map<NodeHandle, DirtyState> m_DirtyStates;
    UpdateStats m_Stats;
};

} // namespace Hydragon::Scene 