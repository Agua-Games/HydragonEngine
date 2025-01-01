/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene description and serialization system
 */

#pragma once
#include "Core/Memory/Management/Strategies/SceneMemoryStrategy.h"

namespace Hydragon {
namespace Scene {

// Inspired by RSL/OSL but optimized for real-time
class SceneDescriptionSystem {
public:
    // Flexible parameter system similar to RenderMan's
    struct Parameter {
        enum class Type {
            Float, Vector, Color, Normal, Point,
            String, Shader, Connection, Array
        };
        
        // Support for both immediate and deferred evaluation
        template<typename T>
        T evaluate(const EvaluationContext& context) {
            if (isLazy()) {
                return evaluateProcedural<T>(context);
            }
            return m_Value.get<T>();
        }
    };

    // Shader network system inspired by RSL/OSL but real-time focused
    class ShaderNetwork {
    public:
        // Support for both AOT and JIT compilation
        void compile(const CompileOptions& options) {
            if (options.enableGPUAcceleration) {
                compileToGPU();
            } else {
                compileToCPU();
            }
        }
    };
};

}} // namespace Hydragon::Scene 