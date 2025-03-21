/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief Base computational field (tensor)
 */
#pragma once
#include <vector>
#include <array>

namespace hd {
    
// Forward declarations for dependencies
class vec3;

template<typename T, size_t Dims>
class Field {
public:
    // Core tensor operations
    T sample(const std::array<size_t, Dims>& index) const;
    T interpolate(const std::array<float, Dims>& position) const;
    
protected:
    // Multi-dimensional tensor storage
    std::vector<T> data;
    std::array<size_t, Dims> dimensions;
    
    // Common field operations, optimizations, etc.
};

} // namespace hd