/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Collision detection and response system
 */

#pragma once
#include "CollisionTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Physics {

class CollisionSystem {
public:
    struct CollisionConfig {
        uint32_t maxCollisionPairs = 10000;
        uint32_t maxContactPoints = 100000;
        bool enableBroadphase = true;
        bool enableContinuous = true;
        float contactThreshold = 0.01f;
        size_t memoryBudget = 64 * 1024 * 1024;  // 64MB
    };

    static CollisionSystem& Get();
    
    void Initialize(const CollisionConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void AddCollider(ColliderHandle handle, const ColliderDesc& desc);
    void RemoveCollider(ColliderHandle handle);
    
    void UpdateCollider(ColliderHandle handle, const Transform& transform);
    void SetCollisionFilter(ColliderHandle handle, uint32_t group, uint32_t mask);
    
    void QueryAABB(const AABB& aabb, QueryCallback callback);
    void QuerySphere(const Sphere& sphere, QueryCallback callback);
    
    const CollisionStats& GetStats() const { return m_Stats; }

private:
    CollisionSystem() = default;
    
    CollisionConfig m_Config;
    CollisionStats m_Stats;
    std::unique_ptr<IBroadphase> m_Broadphase;
    std::unique_ptr<INarrowphase> m_Narrowphase;
    std::vector<CollisionPair> m_CollisionPairs;
    bool m_Initialized = false;
};

} // namespace Hydragon::Physics 