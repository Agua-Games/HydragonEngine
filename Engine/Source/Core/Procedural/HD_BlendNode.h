/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Procedural/HD_ProceduralTypes.h"

namespace hd {

struct HD_BlendInfo : public HD_NodeInfo {
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

class HD_BlendNode : public HD_Node {
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
        : HD_Node(info) {}

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