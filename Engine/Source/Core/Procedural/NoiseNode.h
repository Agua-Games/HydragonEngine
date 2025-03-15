/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_NoiseNode.h
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
#include "Core/Procedural/HD_ProceduralTypes.h"
#include "Core/Math/HD_Noise.h"

namespace hd {

struct HD_NoiseInfo : public NodeInfo {
    HD_NoiseInfo() {
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

class HD_NoiseNode : public Node {
public:
    enum class NoiseType {
        Perlin,
        Simplex,
        Worley,
        Value,
        Gabor,
        Custom
    };

    explicit HD_NoiseNode(const HD_NoiseInfo& info = HD_NoiseInfo())
        : Node(info) {}

    std::vector<std::string> GetInputPorts() const override {
        return NoiseInfo.Inputs;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return NoiseInfo.Outputs;
    }

    void ProcessNodeGraph() override {
        auto noiseType = GetInputValue<NoiseType>("NoiseType");
        uint32_t seed = GetInputValue<uint32_t>("Seed");
        float scale = GetInputValue<float>("Scale");
        int octaves = GetInputValue<int>("Octaves");
        float persistence = GetInputValue<float>("Persistence");
        float lacunarity = GetInputValue<float>("Lacunarity");
        
        NoiseParams params{
            .seed = seed,
            .scale = scale,
            .octaves = octaves,
            .persistence = persistence,
            .lacunarity = lacunarity
        };
        
        auto noiseData = GenerateNoise(noiseType, params);
        auto derivatives = ComputeNoiseDerivatives(noiseData);
        auto metrics = AnalyzeNoiseProperties(noiseData);
        
        SetOutputValue("NoiseValue", noiseData);
        SetOutputValue("Derivatives", derivatives);
        SetOutputValue("NoiseMetrics", metrics);
    }

protected:
    HD_NoiseInfo NoiseInfo;
};

} // namespace hd