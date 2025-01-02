/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene graph transform component
 */

#pragma once
#include "../SceneGraphTypes.h"
#include "Core/Math/MathTypes.h"
#include <memory>

namespace Hydragon::Scene {

class TransformComponent {
public:
    void SetPosition(const Vector3& position);
    void SetRotation(const Quaternion& rotation);
    void SetScale(const Vector3& scale);
    
    void Translate(const Vector3& translation);
    void Rotate(const Quaternion& rotation);
    void Scale(const Vector3& scale);
    
    Vector3 GetPosition() const { return m_LocalTransform.position; }
    Quaternion GetRotation() const { return m_LocalTransform.rotation; }
    Vector3 GetScale() const { return m_LocalTransform.scale; }
    
    const Transform& GetLocalTransform() const { return m_LocalTransform; }
    const Transform& GetWorldTransform() const { return m_WorldTransform; }
    
    void UpdateWorldTransform(const Transform* parentTransform = nullptr);
    bool IsDirty() const { return m_Dirty; }

private:
    Transform m_LocalTransform;
    Transform m_WorldTransform;
    bool m_Dirty = true;
};

} // namespace Hydragon::Scene 