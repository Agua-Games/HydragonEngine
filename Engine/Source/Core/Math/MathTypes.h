/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Math system type definitions
 */

#pragma once
#include <cstdint>
#include <functional>

namespace Hydragon::Math {

struct Vector2 {
    float x, y;
    
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vector4 {
    float x, y, z, w;
    
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

struct Quaternion {
    float x, y, z, w;
    
    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

struct Matrix3x3 {
    float m[9];
    
    Matrix3x3() { Identity(); }
    void Identity();
};

struct Matrix4x4 {
    float m[16];
    
    Matrix4x4() { Identity(); }
    void Identity();
};

struct Transform {
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
    
    Transform() : scale(1, 1, 1) {}
};

using ComputeCallback = std::function<void(void*, uint32_t)>;

struct ComputeTask {
    void* data;
    uint32_t elementCount;
    uint32_t elementSize;
    ComputeCallback callback;
    bool parallel = true;
};

struct MathStats {
    uint64_t matrixOps;
    uint64_t vectorOps;
    uint64_t quaternionOps;
    uint32_t batchTasks;
    uint32_t activeThreads;
    float computeTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Math 