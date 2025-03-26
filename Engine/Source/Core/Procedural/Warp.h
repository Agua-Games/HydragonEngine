/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief Header file for the Warp class.
 * 
 * ARCHITECTURAL NOTES:
 * - Warp is a class that represents a warp effect in Hydragon.
 * - It is used to apply warp effects to various elements in the game world.
 * - It supports different types of warp effects, such as displacement, distortion, and morphing.
 * - It supports different domains, such as 2D and 3D.
 * - It supports different noise types, such as Perlin, Simplex, and Worley.
 * - It supports different noise parameters, such as seed, scale, octaves, persistence, and lacunarity.
 * - It supports different warp parameters, such as warp type, warp strength, and warp direction.
 * - It supports different output formats, such as noise value, derivatives, and noise metrics.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "ProceduralTypes.h"
#include "Noise.h"
#include "ProcOrchestrator.h"

namespace hd {

struct WarpInfo : public NodeInfo {
    WarpInfo() {
        NodeType = "Procedural/Warp";
        
        inputs = {
            "NoiseType",      // Perlin, Simplex, Worley, etc.
            "Seed",
            "Scale",
            "Octaves",
            "Persistence",
            "Lacunarity",
            "Domain",         // 2D or 3D
            "WarpType",       // Displacement, distortion, morphing, etc.
            "WarpStrength",
            "WarpDirection"
        };
        
        outputs = {
            "WarpedValue",
            "Derivatives",    // For normal map generation
            "WarpMetrics"     // Statistical properties
        };
    }
};

class Warp : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Warp(const WarpInfo& info = WarpInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void () override {
        auto noiseType = getInputValue<NoiseType>("NoiseType");
        uint32_t seed = getInputValue<uint32_t>("Seed");
        float scale = getInputValue<float>("Scale");
        int octaves = getInputValue<int>("Octaves");
        float persistence = getInputValue<float>("Persistence");
        float lacunarity = getInputValue<float>("Lacunarity");
        
        NoiseParams params{
            .seed = seed,
            .scale = scale,
            .octaves = octaves,
            .persistence = persistence,
            .lacunarity = lacunarity
        };
        
        auto noiseData = generateNoise(noiseType, params);
        auto warpedData = applyWarp(noiseData);
        auto derivatives = computeNoiseDerivatives(warpedData);
        auto metrics = analyzeNoiseProperties(warpedData);
        
        setOutputValue("WarpedValue", warpedData);
        setOutputValue("Derivatives", derivatives);
        setOutputValue("WarpMetrics", metrics);
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Warp() = default;     // Default destructor
};

} // namespace hd