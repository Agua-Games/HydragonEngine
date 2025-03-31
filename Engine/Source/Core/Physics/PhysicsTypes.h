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
#include "MathTypes.h"              // For vec3 type
#include "PhysicsMaterial.h"

namespace hd {

// === Fields ===
// Define force field types
enum class ForceFieldType {
    Gravity,
    Wind,
    Turbulence,
    Attractor,
    Repeller,
    Custom
};

// === Energy Transfer ===
enum class EnergyTransferType {
    Kinetic,
    Thermal,
    Deformation,
    Particle,
    Wave,
    Field,
    Custom
};

struct EnergyTransferEvent {
    EnergyTransferType type;
    float magnitude;
    vec3 direction;
};

// === Entity Data ===
struct PhysicsData {
    vec3 position;
    vec3 velocity;
    float nucleusPotential;
    float mass;                 // Derived from nucleusPotential, must always be synced to it
    float density;
    PhysicsMaterial material;
    //BodyState bodyState;
};

// === Simulation, Solvers ===
enum class SimulationSpace {
    Local,
    World,
    Custom
};

// === Collisions ===
// Define collision types
enum class CollisionType {
    Box,
    Sphere,
    Capsule,
    Mesh,
    Custom
};

struct CollisionEvent {
    CollisionType type;
    CollisionResponse response;
    vec3 normal;
    float depth;
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