/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 *      - The idea is to have sensible defaults so that the user can focus on the unique aspects of their game.
 *      - The commands here may be in sketch phase. We'll try to consolidate them as we go.
 */
#if 0
auto physics = graph.create<Solid>("solidBody")
    .nucleusPotential(1.0f)              // Simple property
    .friction(0.5f)                      // Simple property
    .shape({1,1,1})                      // Complex setup needs setter
    .material("metal")                   // Needs resource loading
    .addForce({0, -9.81f, 0});           // Action method (for compatibility with traditional physics engines)

#endif