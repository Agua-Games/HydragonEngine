/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ValueGradient.h
 * @brief ValueGradient represents a value gradient node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Value gradient nodes are used to create smooth transitions between different values, such as colors, textures, and other visual elements.
 * - They can be used to create gradients for various purposes, such as color gradients, texture gradients, and other visual effects.
 * - It supports different interpolation modes, such as linear, cubic, and spline interpolation.
 * - It supports different easing functions, such as ease-in, ease-out, and ease-in-out.
 * - It offers built-in support for procedural modification of imported nodes, and for procedural node generation.
 */
#pragma once
#include "Node.h"
#include "Animation.h"

namespace hd {

struct ValueGradientInfo : public NodeInfo {
    ValueGradientInfo() {
        NodeType = "Logic/ValueGradient";
        
        inputs = {
            "Value",           // Value to interpolate
            "Interpolation",   // Interpolation mode
            "Easing",          // Easing function
            "Looping"          // Whether to loop the gradient
        };
        
        outputs = {
            "InterpolatedValue" // Interpolated value
        };
    }
};

class ValueGradient : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ValueGradient(const ValueGradientInfo& info = ValueGradientInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // Set default values
    float floatValue = 0.0f;
    glm::vec2 vector2Value = glm::vec2(0.0f);
    glm::vec3 vectorValue = glm::vec3(0.0f);
    glm::vec4 vector4Value = glm::vec4(0.0f);
    glm::vec4 colorValue = glm::vec4(0.0f);         // Default color is black

    // === Processing ===
    void processNode() override {
    }
    void interpolateValue();
    void applyEasing();
    void loopGradient();
    void addKey(float time, const glm::vec4& value);
    void removeKey(float time);
    void setKeyValue(float time, const glm::vec4& value);
    void setKeyTime(float oldTime, float newTime);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ValueGradient() = default;
};

} // namespace hd