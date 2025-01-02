/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Asset system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <chrono>

namespace Hydragon::Asset {

using AssetHandle = uint64_t;
using AssetVersion = uint32_t;

enum class AssetType {
    Model,
    Texture,
    Material,
    Shader,
    Audio,
    Animation,
    Scene,
    Prefab,
    Data
};

struct AssetImportDesc {
    std::string sourcePath;
    AssetType type;
    std::string targetPath;
    bool generateMipmaps = true;
    bool compressTextures = true;
    bool optimizeMeshes = true;
};

struct AssetMetadata {
    AssetType type;
    AssetVersion version;
    std::string guid;
    std::chrono::system_clock::time_point timestamp;
    std::vector<std::string> tags;
    std::vector<AssetHandle> dependencies;
};

struct AssetStats {
    uint32_t totalAssets;
    uint32_t processedAssets;
    uint32_t failedImports;
    uint32_t cachedAssets;
    float averageProcessTimeMs;
    size_t memoryUsed;
    size_t cacheSize;
};

} // namespace Hydragon::Asset 