/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */

auto physics = graph.create<PhysicsNode>("rigidBody")
    .mass(1.0f)                          // Simple property
    .friction(0.5f)                      // Simple property
    .setCollider<BoxShape>({1,1,1})      // Complex setup needs setter
    .setMaterial("metal")                // Needs resource loading
    .addForce({0, -9.81f, 0});          // Action method