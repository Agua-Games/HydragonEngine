/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VolumetricTypes.h
 * @brief Header file for the VolumetricTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - VolumetricTypes is a namespace that contains various types and enums related to volumetric rendering.
 * - It includes types for volumetric material properties, volumetric rendering techniques, and more.
 * 
 */
#pragma once

namespace hd {

// Define volumetric material properties
enum class VolumetricMaterialProperty {
    Density,
    Scattering,
    Absorption,
    Emission
};

// Define volumetric rendering techniques
enum class VolumetricRenderingTechnique {
    RayMarching,
    VolumeRendering,
    ScreenSpace,
    Custom
};

} // namespace hd
