/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief Base computational field (tensor)
 */
#pragma once
#include <vector>
#include <array>
#include <glm/gtx/gradient_paint.hpp>

namespace hd {

template<typename T, size_t Dims>
struct Field {
public:
    // Core tensor operations
    T sample(const std::array<size_t, Dims>& index) const;
    T interpolate(const std::array<float, Dims>& position) const;
    T computeGradient(const std::array<float, Dims>& position) const;
    T computeDivergence(const std::array<float, Dims>& position) const;
    T computeCurl(const std::array<float, Dims>& position) const;
    T computeAverage(const std::array<float, Dims>& position) const;
    
protected:
    // Multi-dimensional tensor storage
    std::vector<T> data;
    std::array<size_t, Dims> dimensions;
    
    // Common field operations, optimizations, etc.
};

} // namespace hd