/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Noise.h
 * @brief Noise represents a noise node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Noise nodes are used to generate various types of noise patterns.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once

#include "Core/NodeGraph/Node.h"
#include "Core/Procedural/ProceduralTypes.h"
#include "Core/Math/Noise.h"
#include "ProceduralManager.h"

namespace hd {

struct NoiseInfo : public NodeInfo {
    NoiseInfo() {
        nodeType = "Procedural/Noise";
        
        inputs = {
            "noiseType",      // Perlin, Simplex, Worley, etc.
            "seed",
            "scale",
            "octaves",
            "persistence",
            "lacunarity",
            "domain",         // 2D or 3D
            "warp"           // Optional domain warping
        };
        
        outputs = {
            "noiseValue",
            "derivatives",    // For normal map generation
            "noiseMetrics"    // Statistical properties
        };
    }
};

class Noise : public Node {
public:
    enum class NoiseType {
        Perlin,
        Simplex,
        Worley,
        Value,
        Gabor,
        Custom
    };

    struct NoiseParams{
        uint32_t seed = 0;
        float frequency = 1.0f;
        float scale = 1.0f;             // Maybe this is redundant, with frequency
        float intensity = 1.0f;
        int octaves = 1;
        float persistence = 0.5f;
        float lacunarity = 2.0f;
        float staticNoise = 0.0f;       // Sketch phase, need thought, testing. If not needed, remove, for memory economy
        float dynamicNoise = 0.0f;      // Same here
    };

    explicit Noise(const NoiseInfo& info = NoiseInfo())
        : Node(info) {}

    std::vector<std::string> getInputPorts() const override {
        return NoiseInfo.inputs;
    }

    std::vector<std::string> getOutputPorts() const override {
        return NoiseInfo.outputs;
    }

    void processNode() override {
        auto noiseType = getInputValue<NoiseType>("NoiseType");
        auto params = getInputValue<NoiseParams>("NoiseParams");
        
        auto noiseData = generateNoise(noiseType, params);
        auto derivatives = computeNoiseDerivatives(noiseData);
        auto metrics = analyzeNoiseProperties(noiseData);
        
        setOutputValue("NoiseValue", noiseData);
        setOutputValue("Derivatives", derivatives);
        setOutputValue("NoiseMetrics", metrics);
    }

protected:
    NoiseInfo NoiseInfo;
};

} // namespace hd