/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Geometry processing and computation system
 */

#pragma once
#include "../MathTypes.h"
#include "GeometryTypes.h"
#include <memory>
#include <vector>

namespace Hydragon::Math {

class GeometrySystem {
public:
    struct GeometryConfig {
        bool enableCaching = true;
        bool enableGPUAcceleration = true;
        uint32_t maxVertices = 1000000;
        uint32_t maxIndices = 3000000;
        float collisionPrecision = 0.001f;
    };

    static GeometrySystem& Get();
    
    void Initialize(const GeometryConfig& config = {});
    void Shutdown();
    
    // Intersection tests
    bool RayIntersectsTriangle(const Ray& ray, const Triangle& triangle, float* t = nullptr);
    bool RayIntersectsMesh(const Ray& ray, const Mesh& mesh, RayHit* hit = nullptr);
    bool SpheresIntersect(const Sphere& a, const Sphere& b);
    
    // Distance computations
    float PointToLineDistance(const Vector3& point, const Line& line);
    float PointToPlaneDistance(const Vector3& point, const Plane& plane);
    
    // Mesh operations
    void GenerateNormals(Mesh* mesh);
    void GenerateTangents(Mesh* mesh);
    void OptimizeMesh(Mesh* mesh, const MeshOptimizationParams& params);
    
    const GeometryStats& GetStats() const { return m_Stats; }

private:
    GeometrySystem() = default;
    
    GeometryConfig m_Config;
    GeometryStats m_Stats;
    std::unique_ptr<IGeometryProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Math 