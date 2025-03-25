/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 *      - The idea is to have sensible defaults so that the user can focus on the unique aspects of their game.
 */
#if 0
auto physics = graph.create<WavePhysics>("physics")
    .defaultNucleusPotential(1.0f)       // Simple property
    .defaultFriction(0.5f)               // Simple property
    .solidShape<SolidBoxShape>({1,1,1})  // Complex setup needs setter
    .solidShape.setMaterial("metal")     // Needs resource loading
    .addForce({0, -9.81f, 0});           // Action method

#endif