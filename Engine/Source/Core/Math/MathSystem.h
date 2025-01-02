/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core mathematics and computation system
 */

#pragma once
#include "MathTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>

namespace Hydragon::Math {

class MathSystem {
public:
    struct MathConfig {
        bool enableSIMD = true;
        bool enableGPUAcceleration = true;
        uint32_t maxThreads = 8;
        size_t workGroupSize = 256;
        size_t memoryBudget = 64 * 1024 * 1024;  // 64MB
    };

    static MathSystem& Get();
    
    void Initialize(const MathConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Matrix operations
    void MultiplyMatrices(const Matrix4x4* matrices, uint32_t count, Matrix4x4* results);
    void InvertMatrices(const Matrix4x4* matrices, uint32_t count, Matrix4x4* results);
    
    // Vector operations
    void TransformVectors(const Vector4* vectors, const Matrix4x4& transform, 
                         uint32_t count, Vector4* results);
    void NormalizeVectors(const Vector4* vectors, uint32_t count, Vector4* results);
    
    // Quaternion operations
    void SlerpQuaternions(const Quaternion* start, const Quaternion* end, 
                         float* t, uint32_t count, Quaternion* results);
    
    // Batch computations
    void BatchProcess(const ComputeTask& task);
    void ScheduleTask(const ComputeTask& task);
    
    const MathStats& GetStats() const { return m_Stats; }

private:
    MathSystem() = default;
    
    MathConfig m_Config;
    MathStats m_Stats;
    std::unique_ptr<IMathProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Math 