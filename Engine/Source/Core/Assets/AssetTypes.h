/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <chrono>

namespace Hydragon::Assets {

using AssetHandle = uint64_t;

enum class AssetType {
    Mesh,
    Texture,
    Material,
    Shader,
    Animation,
    Sound,
    Script,
    Scene
};

enum class LoadPriority {
    Low,
    Normal,
    High,
    Critical
};

enum class UpdateStrategy {
    Immediate,
    Deferred,
    Manual,
    Cascade
};

struct LoadParams {
    LoadPriority priority = LoadPriority::Normal;
    bool asyncLoad = true;
    bool keepInMemory = false;
    bool generateMips = true;
};

struct AssetInfo {
    AssetType type;
    std::string path;
    size_t size;
    std::chrono::system_clock::time_point lastAccess;
    uint32_t referenceCount;
    bool isLoaded;
    bool isHotReloadable;
};

struct AssetStats {
    size_t totalLoaded;
    size_t totalMemoryUsed;
    size_t peakMemoryUsed;
    uint32_t loadingErrors;
    float averageLoadTime;
};

} // namespace Hydragon::Assets 