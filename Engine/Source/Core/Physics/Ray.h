
/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Ray.h
 * @brief Header file for the Ray class.
 * 
 * ARCHITECTURAL NOTES:
 * - Ray is a class that represents a ray in Hydragon.
 * - Though being a much simpler data structure than Wave, and not physics-oriented, it can use for its primary function, raytracing/raycasting, and also 
 * for cheap light rays, sound rays, etc.
 * - A Ray node can cast many rays, in fact.
 * - It supports interactive features, such as collision detection and response.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct RayInfo : public NodeInfo {
    RayInfo() {
        NodeType = "Physics/Ray";
        
        inputs = {
            "Origin",                   // Origin of the ray
            "Direction",                // Direction of the ray
            "Length",                   // Length of the ray
            "SphereTraceRadius"         // Radius of the sphere trace
        };
        
        outputs = {
            "RayData"        // Ray data
        };
    }
};

class Ray : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Ray(const RayInfo& info = RayInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    RayInfo info;
    DataTable dataTable;
    vec3 origin = vec3(0.0f);
    vec3 direction = vec3(0.0f);
    float length = 0.0f;
    float sphereTraceRadius = 0.0f;
    
    // === Processing ===
    void processNode() override {    
    
    }
    // Simple, fast intersection tests
    bool intersect(const Collider& collider) const;
    bool intersect(const Scene& scene) const;

    // Different cast types
    RayHit cast(float maxDistance = MAX_FLOAT) const;
    RayHit sphereCast(float radius) const;
    RayHit boxCast(const vec3& halfExtents) const;

    // Batch operations for performance
    std::vector<RayHit> multiCast(const Scene& scene) const;
    
    // Optional: Wave-based version for special cases
    WaveHit castAsWave() const {
        // Convert ray to ultra-thin, high-speed wave
        // Use only when wave physics effects are needed
        return WavePhysics::createRayWave(*this);
    }

    void detectCollision();
    void respondToCollision();
    void processRay();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Ray() = default;     // Default destructor
};

} // namespace hd
