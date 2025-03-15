/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_BlendNode.h
 * @brief BlendNode represents a blend node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Blend nodes are used to blend between two values, from simple integers to complex patterns.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "HD_ProceduralTypes.h"

namespace hd {

struct HD_BlendInfo : public NodeInfo {
    HD_BlendInfo() {
        NodeType = "Procedural/Blend";
        
        Inputs = {
            "SourceA",
            "SourceB",
            "BlendMode",     // Add, Multiply, Overlay, etc.
            "BlendFactor",
            "Mask",          // Optional blend mask
            "BlendCurve"     // Optional curve for non-linear blending
        };
        
        Outputs = {
            "BlendedResult",
            "BlendMetrics",
            "TransitionMap"
        };
    }
};

class HD_BlendNode : public Node {
public:
    enum class BlendMode {
        Linear,
        Add,
        Multiply,
        Screen,
        Overlay,
        SoftLight,
        HardLight,
        Custom
    };

    explicit HD_BlendNode(const HD_BlendInfo& info = HD_BlendInfo())
        : Node(info) {}

    std::vector<std::string> GetInputPorts() const override {
        return BlendInfo.Inputs;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return BlendInfo.Outputs;
    }

    void ProcessNodeGraph() override {
        auto sourceA = GetInputValue<ProceduralPatternData>("SourceA");
        auto sourceB = GetInputValue<ProceduralPatternData>("SourceB");
        auto mode = GetInputValue<BlendMode>("BlendMode");
        float factor = GetInputValue<float>("BlendFactor");
        
        auto mask = GetInputValue<ProceduralPatternData>("Mask");
        auto curve = GetInputValue<BlendCurve>("BlendCurve");
        
        BlendParams params{
            .mode = mode,
            .factor = factor,
            .mask = mask,
            .curve = curve
        };
        
        auto blendedResult = BlendPatterns(sourceA, sourceB, params);
        auto metrics = CalculateBlendMetrics(blendedResult);
        auto transitionMap = GenerateTransitionMap(sourceA, sourceB, params);
        
        SetOutputValue("BlendedResult", blendedResult);
        SetOutputValue("BlendMetrics", metrics);
        SetOutputValue("TransitionMap", transitionMap);
    }

protected:
    HD_BlendInfo BlendInfo;
};

} // namespace hd