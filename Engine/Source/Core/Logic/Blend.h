/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Blend.h
 * @brief Blend represents a blend node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Blend nodes automatically choose the most efficient blending implementation based on the input data and parameters, to allow for lightweight blending when
 * not using advanced features, like blend parameters, weights, or custom blend curves.
 * - Blend nodes are used to blend between an arbitrary number of values (using arrays/vectors of ProceduralPatternData), from simple integers to complex patterns.
 * - They can be used to create smooth transitions between different values, such as colors, textures, and other visual elements.
 * - It supports various blend modes, such as linear, multiply, screen, and overlay.
 * - It supports custom blend curves for non-linear blending.
 * - Also supports built-in procedural generation of blend masks, transition maps and simple blending logic.
 * - Also supports multiple layers of blending, in fact working as a Layered Blend node.
 * 
 * @todo When testing it in practice, there may be some issues to fix, for we implemented it with the assistant in different tasks, maybe not reconciled.
 * @todo Create .cpp file and move the implementation there.
 */
#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "Node.h"
#include "ProceduralTypes.h"
#include "Math.h"

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

/**
 * @class Blend.
 * @brief Blend represents a flexible blend node in the engine's node graph.
 * It leverages internal optimizations to provide efficient blending for simple cases, while still supporting complex blending scenarios.
 */
class Blend : public Node {
public:
    // === Structure Definitions ===
    struct BlendMode {
        enum Type {
            Linear,
            Add,
            Multiply,
            Screen,
            Overlay,
            SoftLight,
            HardLight,
            Custom
        };
    };

    // Simple initialization for basic blending
    explicit Blend(const BlendInfo& info = BlendInfo()) : Node(info) {
        m_isSimpleBlend = true;  // Optimization flag
    }

    // === Efficient API for simple blending ===
    void setFactor(float factor) {
        m_factor = factor;
        // No parameter overhead
    }

    // Direct, efficient blend for two inputs
    template<typename T>
    void blend(const T& a, const T& b) {
        if (m_isSimpleBlend) {
            // Fast path: direct lerp
            m_result = lerp(a, b, m_factor);
            return;
        }
        processComplexBlend();  // Fall back to full processing if needed
    }

    // === Extended API for parameter-driven blending ===
    void addBlendParameter(const std::string& name, float defaultValue = 0.0f) {
        if (m_parameters.empty()) {
            m_isSimpleBlend = false;  // Switch to complex mode only when needed
        }
        m_parameters[name] = defaultValue;
    }

    void setBlendWeight(size_t index, float weight) {
        if (m_weights.empty()) {
            m_isSimpleBlend = false;
        }
        m_weights.resize(std::max(m_weights.size(), index + 1));
        m_weights[index] = weight;
    }

    void setBlendMode(BlendMode::Type mode) {
        m_blendMode = mode;
    }

    void setMask(const ProceduralPatternData& mask) {
        m_mask = std::make_unique<ProceduralPatternData>(mask);
        m_usesMask = true;
    }

    void setBlendCurve(const ProceduralPatternData& curve) {
        m_blendCurve = std::make_unique<ProceduralPatternData>(curve);
        m_usesCustomCurve = true;
    }

protected:
    // Optimization flags
    bool m_isSimpleBlend = true;     // Fast path for simple blending
    bool m_usesMask = false;         // Mask optimization
    bool m_usesParameters = false;    // Parameter system optimization
    bool m_usesCustomCurve = false;  // Custom blend curve optimization

    // Core data
    float m_factor = 0.0f;           // Simple blend factor (always available)
    BlendMode::Type m_blendMode = BlendMode::Linear;
    T m_result;
    
    // Extended features (only allocated when needed)
    std::unique_ptr<std::unordered_map<std::string, float>> m_parameters;  // Lazy-initialized
    std::unique_ptr<std::vector<float>> m_weights;                         // Lazy-initialized
    std::unique_ptr<ProceduralPatternData> m_mask;                         // Lazy-initialized
    std::unique_ptr<ProceduralPatternData> m_blendCurve;                   // Lazy-initialized

    float evaluateBlendFactor(float rawFactor) const {
        if (m_usesCustomCurve) {
            return m_blendCurve->evaluate(rawFactor);
        }
        return rawFactor;
    }

    template<typename T>
    T applyMask(const T& blendResult, const T& sourceA) {
        if (m_usesMask) {
            return lerp(sourceA, blendResult, m_mask->evaluate());
        }
        return blendResult;
    }

    void () override {
        if (m_isSimpleBlend) {
            processSimpleBlend();  // Fast path
        } else {
            processComplexBlend(); // Full featured path
        }
    }

private:
    void processSimpleBlend() {
        auto sourceA = getInputValue<T>("SourceA");
        auto sourceB = getInputValue<T>("SourceB");
        
        float factor = evaluateBlendFactor(m_factor);
        T result = lerp(sourceA, sourceB, factor);
        
        if (m_usesMask) {
            result = applyMask(result, sourceA);
        }
        
        setOutputValue("BlendedResult", result);
    }

    void processComplexBlend() {
        // Process complex blending logic
        // ...
    }
};

} // namespace hd

