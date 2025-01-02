/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core physics simulation system
 */

#pragma once
#include "PhysicsTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Physics {

class PhysicsSystem {
public:
    struct PhysicsConfig {
        float fixedTimeStep = 1.0f/60.0f;
        uint32_t maxSubSteps = 8;
        uint32_t maxBodies = 10000;
        uint32_t maxConstraints = 1000;
        bool enableCCD = true;
        bool enableMultithreading = true;
        bool enableDebugDraw = true;
        Vector3 gravity = {0.0f, -9.81f, 0.0f};
        size_t memoryBudget = 256 * 1024 * 1024;  // 256MB
    };

    static PhysicsSystem& Get();
    
    void Initialize(const PhysicsConfig& config = {});
    void Shutdown();
    
    void Update(float deltaTime);
    void StepSimulation(float timeStep);
    
    RigidBodyHandle CreateRigidBody(const RigidBodyDesc& desc);
    void DestroyRigidBody(RigidBodyHandle handle);
    
    ColliderHandle CreateCollider(const ColliderDesc& desc);
    void DestroyCollider(ColliderHandle handle);
    
    ConstraintHandle CreateConstraint(const ConstraintDesc& desc);
    void DestroyConstraint(ConstraintHandle handle);
    
    void SetGravity(const Vector3& gravity);
    void RayCast(const Ray& ray, RayCastCallback callback);
    
    const PhysicsStats& GetStats() const { return m_Stats; }

private:
    PhysicsSystem() = default;
    
    PhysicsConfig m_Config;
    PhysicsStats m_Stats;
    std::unique_ptr<IPhysicsEngine> m_Engine;
    std::vector<RigidBody> m_Bodies;
    std::vector<Collider> m_Colliders;
    bool m_Initialized = false;
};

} // namespace Hydragon::Physics 