/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NoiseNode.h
 * @brief NoiseNode represents a noise node in the engine's node graph.
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

namespace hd {

struct NoiseInfo : public NodeInfo {
    NoiseInfo() {
        NodeType = "Procedural/Noise";
        
        Inputs = {
            "NoiseType",      // Perlin, Simplex, Worley, etc.
            "Seed",
            "Scale",
            "Octaves",
            "Persistence",
            "Lacunarity",
            "Domain",         // 2D or 3D
            "Warp"           // Optional domain warping
        };
        
        Outputs = {
            "NoiseValue",
            "Derivatives",    // For normal map generation
            "NoiseMetrics"    // Statistical properties
        };
    }
};

class NoiseNode : public Node {
public:
    enum class NoiseType {
        Perlin,
        Simplex,
        Worley,
        Value,
        Gabor,
        Custom
    };

    explicit NoiseNode(const NoiseInfo& info = NoiseInfo())
        : Node(info) {}

    std::vector<std::string> getInputPorts() const override {
        return NoiseInfo.Inputs;
    }

    std::vector<std::string> getOutputPorts() const override {
        return NoiseInfo.Outputs;
    }

    void processNodeGraph() override {
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