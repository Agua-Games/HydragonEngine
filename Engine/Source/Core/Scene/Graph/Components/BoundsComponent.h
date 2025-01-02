/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene graph bounds component
 */

#pragma once
#include "../SceneGraphTypes.h"
#include "Core/Math/MathTypes.h"
#include <memory>

namespace Hydragon::Scene {

class BoundsComponent {
public:
    void SetLocalBounds(const BoundingVolume& bounds);
    const BoundingVolume& GetLocalBounds() const { return m_LocalBounds; }
    const BoundingVolume& GetWorldBounds() const { return m_WorldBounds; }
    
    void UpdateWorldBounds(const Transform& worldTransform);
    bool Intersects(const BoundingVolume& other) const;
    bool Contains(const Vector3& point) const;
    
    float GetRadius() const { return m_WorldBounds.radius; }
    Vector3 GetCenter() const { return m_WorldBounds.center; }
    
    bool IsDirty() const { return m_Dirty; }

private:
    BoundingVolume m_LocalBounds;
    BoundingVolume m_WorldBounds;
    bool m_Dirty = true;
};

} // namespace Hydragon::Scene 