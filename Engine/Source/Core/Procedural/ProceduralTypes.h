/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Procedural generation type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Procedural {

using GeneratorHandle = uint32_t;

enum class GeneratorType {
    Terrain,
    Vegetation,
    City,
    Dungeon,
    Texture,
    Mesh,
    Sound,
    Custom
};

struct NoiseParams {
    float frequency = 1.0f;
    float amplitude = 1.0f;
    uint32_t octaves = 4;
    float persistence = 0.5f;
    float lacunarity = 2.0f;
    Vector3 offset = {0.0f, 0.0f, 0.0f};
};

struct GeneratorParams {
    NoiseParams noise;
    float density = 1.0f;
    float variation = 0.5f;
    float detail = 1.0f;
    std::vector<float> customParams;
};

struct GenerationContext {
    Vector3 center;
    Vector3 extent;
    uint32_t resolution;
    uint32_t lod;
    bool async = true;
};

struct GeneratorDesc {
    GeneratorType type;
    std::string name;
    GeneratorParams params;
    bool persistent = false;
    bool incremental = false;
};

struct ProceduralStats {
    uint32_t activeGenerators;
    uint32_t pendingJobs;
    uint32_t completedJobs;
    uint32_t failedJobs;
    float averageGenerationTimeMs;
    size_t memoryUsed;
};

using GenerationCallback = std::function<void(GeneratorHandle, bool success)>;

} // namespace Hydragon::Procedural 