/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PhysicsTypes.h
 * @brief Header file for the PhysicsTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - PhysicsTypes is a namespace that contains various types and enums related to physics.
 * - It includes types for force fields, collision types, collision responses, and more. It also includes types for energy transfer events, collision events, etc.
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

enum class SimulationSpace {
    Local,
    World,
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

// Define collision responses
enum class CollisionResponse {
    Elastic,
    Inelastic,
    Bouncy,
    Sticky,
    Custom
};

} // namespace hd