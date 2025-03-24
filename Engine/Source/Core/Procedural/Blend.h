/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Blend.h
 * @brief Blend represents a blend node in the engine's node graph.
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
#include "ProceduralTypes.h"

namespace hd {

struct BlendInfo : public NodeInfo {
    BlendInfo() {
        NodeType = "Procedural/Blend";
        
        inputs = {
            "SourceA",
            "SourceB",
            "BlendMode",     // Add, Multiply, Overlay, etc.
            "BlendFactor",
            "Mask",          // Optional blend mask
            "BlendCurve"     // Optional curve for non-linear blending
        };
        
        outputs = {
            "BlendedResult",
            "BlendMetrics",
            "TransitionMap"
        };
    }
};

class Blend : public Node {
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

    explicit Blend(const BlendInfo& info = BlendInfo())
        : Node(info) {}

    std::vector<std::string> getInputPorts() const override {
        return BlendInfo.inputs;
    }

    std::vector<std::string> getOutputPorts() const override {
        return BlendInfo.outputs;
    }

    void processNodeGraph() override {
        auto sourceA = getInputValue<ProceduralPatternData>("SourceA");
        auto sourceB = getInputValue<ProceduralPatternData>("SourceB");
        auto mode = getInputValue<BlendMode>("BlendMode");
        float factor = getInputValue<float>("BlendFactor");
        
        auto mask = getInputValue<ProceduralPatternData>("Mask");
        auto curve = getInputValue<BlendCurve>("BlendCurve");
        
        BlendParams params{
            .mode = mode,
            .factor = factor,
            .mask = mask,
            .curve = curve
        };
        
        auto blendedResult = blendPatterns(sourceA, sourceB, params);
        auto metrics = calculateBlendMetrics(blendedResult);
        auto transitionMap = generateTransitionMap(sourceA, sourceB, params);
        
        setOutputValue("BlendedResult", blendedResult);
        setOutputValue("BlendMetrics", metrics);
        setOutputValue("TransitionMap", transitionMap);
    }

protected:
    BlendInfo BlendInfo;
};

} // namespace hd