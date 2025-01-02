/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Attribute interpolation system for smooth data transitions
 */

#pragma once
#include "Core/Data/AttributeTypes.h"
#include "Core/Data/AttributeStorage.h"
#include <memory>
#include <functional>

namespace Hydragon::Data {

// Interpolation methods
enum class InterpolationMethod {
    Linear,
    Smooth,
    Cubic,
    Nearest,
    Custom
};

// Interpolation configuration
struct InterpolationConfig {
    InterpolationMethod method = InterpolationMethod::Linear;
    float smoothness = 0.5f;
    bool extrapolate = false;
    uint32_t samples = 10;
    std::function<float(float)> customCurve;
};

class AttributeInterpolator {
public:
    explicit AttributeInterpolator(const InterpolationConfig& config = {})
        : m_Config(config) {}

    template<typename T>
    bool Interpolate(const IAttributeStorage* source, 
                    IAttributeStorage* target,
                    size_t startIndex,
                    size_t endIndex,
                    float t) {
        if (!ValidateStorages(source, target)) {
            return false;
        }

        try {
            switch (m_Config.method) {
                case InterpolationMethod::Linear:
                    return LinearInterpolate<T>(source, target, startIndex, endIndex, t);
                case InterpolationMethod::Smooth:
                    return SmoothInterpolate<T>(source, target, startIndex, endIndex, t);
                case InterpolationMethod::Cubic:
                    return CubicInterpolate<T>(source, target, startIndex, endIndex, t);
                case InterpolationMethod::Nearest:
                    return NearestInterpolate<T>(source, target, startIndex, endIndex, t);
                case InterpolationMethod::Custom:
                    return CustomInterpolate<T>(source, target, startIndex, endIndex, t);
                default:
                    return false;
            }
        } catch (const std::exception& e) {
            // Log error and return false
            return false;
        }
    }

private:
    InterpolationConfig m_Config;

    bool ValidateStorages(const IAttributeStorage* source, 
                         const IAttributeStorage* target) const;

    template<typename T>
    bool LinearInterpolate(const IAttributeStorage* source,
                          IAttributeStorage* target,
                          size_t startIndex,
                          size_t endIndex,
                          float t);

    template<typename T>
    bool SmoothInterpolate(const IAttributeStorage* source,
                          IAttributeStorage* target,
                          size_t startIndex,
                          size_t endIndex,
                          float t);

    template<typename T>
    bool CubicInterpolate(const IAttributeStorage* source,
                         IAttributeStorage* target,
                         size_t startIndex,
                         size_t endIndex,
                         float t);

    template<typename T>
    bool NearestInterpolate(const IAttributeStorage* source,
                           IAttributeStorage* target,
                           size_t startIndex,
                           size_t endIndex,
                           float t);

    template<typename T>
    bool CustomInterpolate(const IAttributeStorage* source,
                          IAttributeStorage* target,
                          size_t startIndex,
                          size_t endIndex,
                          float t);

    float SmoothStep(float t) const;
    float CubicStep(float t) const;
};

} // namespace Hydragon::Data 