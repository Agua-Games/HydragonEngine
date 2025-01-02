/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Physics system type definitions
 */

#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/Quaternion.h"
#include <cstdint>
#include <functional>

namespace Hydragon::Physics {

using RigidBodyHandle = uint32_t;
using ColliderHandle = uint32_t;
using ConstraintHandle = uint32_t;

enum class BodyType {
    Static,
    Dynamic,
    Kinematic
};

enum class ColliderType {
    Box,
    Sphere,
    Capsule,
    Mesh,
    Compound
};

struct RigidBodyDesc {
    BodyType type = BodyType::Dynamic;
    float mass = 1.0f;
    Vector3 position;
    Quaternion rotation;
    bool enableCCD = false;
    float friction = 0.5f;
    float restitution = 0.0f;
    uint32_t collisionGroup = 1;
    uint32_t collisionMask = 0xFFFFFFFF;
};

struct ColliderDesc {
    ColliderType type;
    Vector3 size;  // For box, or radius for sphere
    float density = 1.0f;
    bool isTrigger = false;
    bool isExclusive = false;
};

struct ConstraintDesc {
    RigidBodyHandle bodyA;
    RigidBodyHandle bodyB;
    Vector3 pivotA;
    Vector3 pivotB;
    bool enableCollision = true;
    float breakingForce = FLT_MAX;
};

struct Ray {
    Vector3 origin;
    Vector3 direction;
    float maxDistance = FLT_MAX;
};

using RayCastCallback = std::function<bool(const RayCastHit&)>;

struct PhysicsStats {
    uint32_t activeBodies;
    uint32_t activeColliders;
    uint32_t activeConstraints;
    uint32_t activeContacts;
    float simulationTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Physics 