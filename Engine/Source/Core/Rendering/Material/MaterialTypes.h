/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MaterialTypes.h
 * @brief Header file for the MaterialTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - MaterialTypes is a namespace that contains various types and enums related to materials.
 * - It includes types for material properties, material types, and more.
 * 
 */
#pragma once

namespace hd {

// Define material types
enum class MaterialType {
    Surface,
    PostProcess,
    Volumetric,
    Compute,
    RayTracing
};

// Define material properties
enum class SurfaceMaterialProperty {
    Albedo,
    Metallic,
    Roughness,
    Normal,
    Emissive,
    Opacity,
    Displacement,
    AmbientOcclusion
};

enum class PostProcessMaterialProperty {
    Color,
    Intensity,
    Blur,
    Contrast,
    Saturation
};

enum class VolumetricMaterialProperty {
    Density,
    Scattering,
    Absorption,
    Emission
};

enum class ComputeMaterialProperty {
    Input,
    Output,
    Parameter
};


enum class RayTracingMaterialProperty {
    rayTracedShadows,
    rayTracedReflections,
    rayTracedRefractions
};

} // namespace hd