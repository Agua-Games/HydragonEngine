/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PhysicsTypes.h
 * @brief Header file for the PhysicsTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - Still a design sketch. We must check how it relates to, integrates with WavePhysics current types, data structures.
 * - PhysicsTypes is a namespace that contains various types and enums related to physics.
 * - It includes types for force fields, collision types, and more.
 * 
 */
#pragma once

namespace hd {

// Define force field types
enum class ForceFieldType {
    Gravity,
    Wind,
    Turbulence,
    Attractor,
    Repeller,
    Custom
};

// Define collision types
enum class CollisionType {
    Box,
    Sphere,
    Capsule,
    Mesh,
    Custom
};

} // namespace hd