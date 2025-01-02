/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Implementation of attribute interpolation methods
 */

#pragma once
#include "Core/Data/AttributeInterpolation.h"
#include "Core/Data/AttributeTypeMapping.h"
#include <cmath>

namespace Hydragon::Data {

template<typename T>
bool AttributeInterpolator::LinearInterpolate(
    const IAttributeStorage* source,
    IAttributeStorage* target,
    size_t startIndex,
    size_t endIndex,
    float t) {
    
    if constexpr (std::is_arithmetic_v<T>) {
        T start = source->GetValue<T>(startIndex);
        T end = source->GetValue<T>(endIndex);
        T result = static_cast<T>(start + (end - start) * t);
        target->SetValue<T>(startIndex, result);
        return true;
    }
    return false;
}

template<typename T>
bool AttributeInterpolator::SmoothInterpolate(
    const IAttributeStorage* source,
    IAttributeStorage* target,
    size_t startIndex,
    size_t endIndex,
    float t) {
    
    if constexpr (std::is_arithmetic_v<T>) {
        float smoothT = SmoothStep(t);
        T start = source->GetValue<T>(startIndex);
        T end = source->GetValue<T>(endIndex);
        T result = static_cast<T>(start + (end - start) * smoothT);
        target->SetValue<T>(startIndex, result);
        return true;
    }
    return false;
}

template<typename T>
bool AttributeInterpolator::CubicInterpolate(
    const IAttributeStorage* source,
    IAttributeStorage* target,
    size_t startIndex,
    size_t endIndex,
    float t) {
    
    if constexpr (std::is_arithmetic_v<T>) {
        float cubicT = CubicStep(t);
        T start = source->GetValue<T>(startIndex);
        T end = source->GetValue<T>(endIndex);
        T result = static_cast<T>(start + (end - start) * cubicT);
        target->SetValue<T>(startIndex, result);
        return true;
    }
    return false;
}

template<typename T>
bool AttributeInterpolator::NearestInterpolate(
    const IAttributeStorage* source,
    IAttributeStorage* target,
    size_t startIndex,
    size_t endIndex,
    float t) {
    
    T result = t < 0.5f ? 
        source->GetValue<T>(startIndex) : 
        source->GetValue<T>(endIndex);
    target->SetValue<T>(startIndex, result);
    return true;
}

template<typename T>
bool AttributeInterpolator::CustomInterpolate(
    const IAttributeStorage* source,
    IAttributeStorage* target,
    size_t startIndex,
    size_t endIndex,
    float t) {
    
    if (!m_Config.customCurve) {
        return LinearInterpolate<T>(source, target, startIndex, endIndex, t);
    }

    if constexpr (std::is_arithmetic_v<T>) {
        float customT = m_Config.customCurve(t);
        T start = source->GetValue<T>(startIndex);
        T end = source->GetValue<T>(endIndex);
        T result = static_cast<T>(start + (end - start) * customT);
        target->SetValue<T>(startIndex, result);
        return true;
    }
    return false;
}

bool AttributeInterpolator::ValidateStorages(
    const IAttributeStorage* source,
    const IAttributeStorage* target) const {
    
    if (!source || !target) {
        return false;
    }

    return source->GetValueType() == target->GetValueType();
}

float AttributeInterpolator::SmoothStep(float t) const {
    // Smootherstep interpolation
    t = std::clamp(t, 0.0f, 1.0f);
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float AttributeInterpolator::CubicStep(float t) const {
    // Cubic Bezier interpolation with configurable smoothness
    t = std::clamp(t, 0.0f, 1.0f);
    float s = m_Config.smoothness;
    float u = 1.0f - t;
    return 3.0f * u * u * t * s + 3.0f * u * t * t * (1.0f - s) + t * t * t;
}

} // namespace Hydragon::Data 