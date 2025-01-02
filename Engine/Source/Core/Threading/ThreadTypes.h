/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Threading system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <functional>

namespace Hydragon::Threading {

using ThreadHandle = uint32_t;
using FiberHandle = uint32_t;

enum class ThreadPriority {
    Low,
    Normal,
    High,
    RealTime
};

using ThreadFunction = std::function<void()>;
using FiberFunction = std::function<void()>;

struct ThreadDesc {
    std::string name;
    ThreadFunction function;
    ThreadPriority priority = ThreadPriority::Normal;
    uint32_t coreMask = 0;  // 0 = no affinity
    size_t stackSize = 0;   // 0 = use default
};

struct FiberDesc {
    std::string name;
    FiberFunction function;
    size_t stackSize = 0;   // 0 = use default
    bool pooled = true;
};

struct ThreadStats {
    uint32_t activeThreads;
    uint32_t activeFibers;
    uint32_t pooledFibers;
    uint32_t contextSwitches;
    float averageUtilization;
    size_t totalStackMemory;
};

} // namespace Hydragon::Threading 