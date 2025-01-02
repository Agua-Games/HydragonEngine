/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Resource system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <functional>

namespace Hydragon::Resource {

using ResourceHandle = uint32_t;

enum class ResourceType {
    Texture,
    Mesh,
    Material,
    Shader,
    Sound,
    Animation,
    Script,
    Data
};

enum class ResourceState {
    Unloaded,
    Loading,
    Ready,
    Failed
};

struct ResourceDesc {
    std::string path;
    ResourceType type;
    bool async = true;
    bool keepInMemory = false;
    uint32_t priority = 0;
    std::string fallbackPath;
};

using ResourceCallback = std::function<void(ResourceHandle, ResourceState)>;

struct ResourceStats {
    uint32_t totalResources;
    uint32_t loadedResources;
    uint32_t pendingLoads;
    uint32_t failedLoads;
    float averageLoadTimeMs;
    size_t memoryUsed;
    size_t peakMemoryUsed;
};

} // namespace Hydragon::Resource 