/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Blend.h
 * @brief Blend represents a blend node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Blend nodes are used to blend between an arbitrary number of values (using arrays/vectors of ProceduralPatternData), from simple integers to complex patterns.
 * - They can be used to create smooth transitions between different values, such as colors, textures, and other visual elements.
 * - It supports various blend modes, such as linear, multiply, screen, and overlay.
 * - It supports custom blend curves for non-linear blending.
 * - Also supports built-in procedural generation of blend masks, transition maps and simple blending logic.
 * - Also supports multiple layers of blending, in fact working as a Layered Blend node.
 */
#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "ProceduralTypes.h"

namespace hd {

struct BlendInfo : public NodeInfo {
    BlendInfo() {
        NodeType = "Logic/Blend";
        
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