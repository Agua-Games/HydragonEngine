/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PhysicsMaterial.h
 * @brief PhysicsMaterial represents the physics properties of a material, like friction, restitution, etc.
 * 
 * ARCHITECTURAL NOTES:
 * - PhysicsMaterial is a class that represents the physics properties of a material, like friction, restitution, etc.
 * - It is used to define the behavior of objects in the physics simulation.
 * - Some of the properties that would usually be present in PhysicsMaterials are delegated to the (solid or fluid) physics field representing the meshes, volumes.
 * So, we usually define here the properties that would be unecessarily expensive to calculate as fields - for instance, properties which remain the same value,
 * in general, throughout the whole mesh or volume (like friction).
 * 
 * @todo Integrate PhysicsMaterial in the Material node, so that users get used to setting up at least a physics material preset when creating a material.
 */
#pragma once
#include "PhysicsTypes.h"

namespace hd {

struct PhysicsMaterial {
    float friction;
    float restitution;
    float density;
};

} // namespace hd
