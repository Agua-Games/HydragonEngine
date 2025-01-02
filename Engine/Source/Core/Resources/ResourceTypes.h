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
#include <vector>

namespace Hydragon::Resources {

using ResourceHandle = uint64_t;

enum class ResourceType {
    Texture,
    Mesh,
    Material,
    Shader,
    Sound,
    Animation,
    Script,
    Config,
    Custom
};

enum class LoadPriority {
    Critical,
    High,
    Normal,
    Low,
    Background
};

struct ResourceDesc {
    std::string path;
    ResourceType type;
    LoadPriority priority = LoadPriority::Normal;
    bool async = true;
    bool cached = true;
    bool compressed = true;
    size_t expectedSize = 0;
};

struct ResourceInfo {
    ResourceType type;
    std::string path;
    size_t size;
    bool loaded;
    bool cached;
    bool compressed;
    float loadProgress;
    uint64_t lastAccessTime;
};

using ResourceCallback = std::function<void(ResourceHandle, bool)>;

struct ResourceStats {
    uint32_t totalResources;
    uint32_t loadedResources;
    uint32_t pendingLoads;
    uint32_t failedLoads;
    size_t totalMemoryUsed;
    size_t cachedMemoryUsed;
    float averageLoadTimeMs;
};

} // namespace Hydragon::Resources 