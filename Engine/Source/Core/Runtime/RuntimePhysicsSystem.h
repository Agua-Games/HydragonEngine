/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime physics simulation and collision management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimePhysicsConfig {
    bool enableContinuousDetection = true;
    bool enableMultithreading = true;
    bool enableSleeping = true;
    bool enableDebugDraw = true;
    uint32_t solverIterations = 8;
    uint32_t maxSubsteps = 3;
    float fixedTimestep = 1.0f/60.0f;
    float defaultFriction = 0.5f;
    float defaultRestitution = 0.0f;
    Vector3 gravity = {0.0f, -9.81f, 0.0f};
};

class RuntimePhysicsSystem {
public:
    static RuntimePhysicsSystem& Get();
    
    void Initialize(const RuntimePhysicsConfig& config = {});
    void Shutdown();
    
    void Update();
    void FixedUpdate();
    
    RigidBodyHandle CreateRigidBody(const RigidBodyDesc& desc);
    void DestroyRigidBody(RigidBodyHandle handle);
    
    ColliderHandle CreateCollider(const ColliderDesc& desc);
    void DestroyCollider(ColliderHandle handle);
    
    JointHandle CreateJoint(const JointDesc& desc);
    void DestroyJoint(JointHandle handle);
    
    void SetBodyTransform(RigidBodyHandle handle, const Transform& transform);
    void SetBodyVelocity(RigidBodyHandle handle, const Vector3& linear, const Vector3& angular = {});
    void ApplyForce(RigidBodyHandle handle, const Vector3& force, const Vector3& point = {});
    void ApplyImpulse(RigidBodyHandle handle, const Vector3& impulse, const Vector3& point = {});
    
    void SetCollisionFilter(ColliderHandle handle, uint32_t category, uint32_t mask);
    void SetCollisionGroup(ColliderHandle handle, int32_t group);
    
    void AddCollisionHandler(const std::string& name, const CollisionCallback& callback);
    void RemoveCollisionHandler(const std::string& name);
    
    void SetGravity(const Vector3& gravity);
    void SetTimestep(float timestep);
    
    RaycastResult Raycast(const Vector3& origin, const Vector3& direction, float maxDistance = 1000.0f);
    std::vector<OverlapResult> OverlapSphere(const Vector3& center, float radius);
    
    const RuntimePhysicsStats& GetStats() const { return m_Stats; }

private:
    RuntimePhysicsSystem() = default;
    
    RuntimePhysicsConfig m_Config;
    RuntimePhysicsStats m_Stats;
    std::unordered_map<RigidBodyHandle, RigidBodyInstance> m_Bodies;
    std::unordered_map<ColliderHandle, ColliderInstance> m_Colliders;
    std::unordered_map<JointHandle, JointInstance> m_Joints;
    std::unordered_map<std::string, CollisionCallback> m_CollisionHandlers;
    std::unique_ptr<IPhysicsSimulator> m_Simulator;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 