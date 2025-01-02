/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Collision system type definitions
 */

#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/AABB.h"
#include <cstdint>
#include <functional>

namespace Hydragon::Physics {

struct CollisionPair {
    ColliderHandle colliderA;
    ColliderHandle colliderB;
    uint32_t numContacts;
    float penetration;
    Vector3 normal;
};

struct ContactPoint {
    Vector3 position;
    Vector3 normal;
    float penetration;
    float friction;
    float restitution;
};

struct Sphere {
    Vector3 center;
    float radius;
};

using QueryCallback = std::function<bool(ColliderHandle)>;

struct CollisionStats {
    uint32_t broadphaseTests;
    uint32_t narrowphaseTests;
    uint32_t activeCollisionPairs;
    uint32_t totalContactPoints;
    float collisionTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Physics 