/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Geometry system type definitions
 */

#pragma once
#include "../MathTypes.h"
#include <vector>

namespace Hydragon::Math {

struct Ray {
    Vector3 origin;
    Vector3 direction;
};

struct Plane {
    Vector3 normal;
    float distance;
};

struct Line {
    Vector3 start;
    Vector3 end;
};

struct Triangle {
    Vector3 v0, v1, v2;
    Vector3 normal;
};

struct Sphere {
    Vector3 center;
    float radius;
};

struct AABB {
    Vector3 min;
    Vector3 max;
};

struct Mesh {
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
    std::vector<Vector4> tangents;
    std::vector<uint32_t> indices;
};

struct RayHit {
    float distance;
    Vector3 position;
    Vector3 normal;
    uint32_t triangleIndex;
};

struct MeshOptimizationParams {
    bool optimizeVertexCache = true;
    bool optimizeVertexFetch = true;
    bool simplifyMesh = false;
    float simplificationRatio = 1.0f;
};

struct GeometryStats {
    uint64_t intersectionTests;
    uint64_t distanceChecks;
    uint32_t meshOperations;
    float processingTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Math 