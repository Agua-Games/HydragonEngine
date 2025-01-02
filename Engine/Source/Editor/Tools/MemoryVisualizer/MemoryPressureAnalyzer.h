/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory pressure analyzer for Hydragon
 */

#pragma once
#include "Core/Memory/MemoryBlock.h"

namespace Hydragon {
namespace Tools {

class MemoryPressureAnalyzer {
public:
    enum class PressureLevel {
        Low,
        Medium,
        High,
        Critical
    };

    struct PressureMetrics {
        float fragmentationRatio;
        float pageUtilization;
        float allocationRate;
        float failureRate;
        size_t largestContiguousBlock;
        bool hasPageThrashing;
    };

    PressureLevel analyzePressure(const Memory::MemoryState& state);
    PressureMetrics calculateMetrics(const Memory::MemoryState& state);
    
    // Pressure thresholds
    void setPressureThresholds(float medium, float high, float critical);
};

}} // namespace Hydragon::Tools 