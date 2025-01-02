/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Physics system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Physics/PhysicsSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct PhysicsBindingsConfig {
    bool enableCollisionCallbacks = true;
    bool enableTriggerCallbacks = true;
    bool enableConstraintBreakage = true;
    bool enableDebugDrawing = true;
    uint32_t maxCollisionPairs = 1000;
    uint32_t maxConstraints = 500;
    uint32_t maxRaycasts = 100;
    std::string physicsDataPath = "Data/Physics";
};

class PhysicsBindings {
public:
    static PhysicsBindings& Get();
    
    void Initialize(const PhysicsBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Rigid body management
    RigidBodyHandle CreateRigidBody(const RigidBodyDesc& desc);
    void DestroyRigidBody(RigidBodyHandle handle);
    void SetRigidBodyProperties(RigidBodyHandle handle, const RigidBodyProperties& props);
    
    // Collider management
    ColliderHandle CreateCollider(const ColliderDesc& desc);
    void DestroyCollider(ColliderHandle handle);
    void SetColliderProperties(ColliderHandle handle, const ColliderProperties& props);
    
    // Constraint management
    ConstraintHandle CreateConstraint(const ConstraintDesc& desc);
    void DestroyConstraint(ConstraintHandle handle);
    void SetConstraintProperties(ConstraintHandle handle, const ConstraintProperties& props);
    
    // Physics queries
    void RayCast(const Vector3& start, const Vector3& end, const RaycastOptions& options);
    void ShapeCast(const ColliderDesc& shape, const Transform& start, const Transform& end);
    void OverlapTest(const ColliderDesc& shape, const Transform& pose);
    
    // Physics callbacks
    void RegisterCollisionCallback(const std::string& name, const CollisionCallback& callback);
    void UnregisterCollisionCallback(const std::string& name);
    void RegisterTriggerCallback(const std::string& name, const TriggerCallback& callback);
    void UnregisterTriggerCallback(const std::string& name);
    
    // Debug visualization
    void EnableDebugDrawing(bool enable);
    void SetDebugDrawMode(DebugDrawMode mode);
    
    const PhysicsBindingsStats& GetStats() const { return m_Stats; }

private:
    PhysicsBindings() = default;
    
    PhysicsBindingsConfig m_Config;
    PhysicsBindingsStats m_Stats;
    std::unordered_map<RigidBodyHandle, RigidBodyInstance> m_RigidBodies;
    std::unordered_map<ColliderHandle, ColliderInstance> m_Colliders;
    std::unordered_map<ConstraintHandle, ConstraintInstance> m_Constraints;
    std::unordered_map<std::string, CollisionCallback> m_CollisionCallbacks;
    std::unordered_map<std::string, TriggerCallback> m_TriggerCallbacks;
    std::unique_ptr<Physics::IPhysicsProcessor> m_Processor;
    std::unique_ptr<Physics::IDebugDrawer> m_DebugDrawer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 