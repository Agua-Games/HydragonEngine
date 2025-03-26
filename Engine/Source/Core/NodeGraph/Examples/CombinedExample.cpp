/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 *      - The idea is to have sensible defaults so that the user can focus on the unique aspects of their game.
 *      - The commands here may be in sketch phase. We'll try to consolidate them as we go.
 */

// Clean, fluent style for node creation
auto scene = graph.create<DynamicEnvLighting>("envLighting")
    .skybox("sky/hdri.exr")
    .ambient({0.1f, 0.1f, 0.1f})
    .fog(true)
    .fogDensity(0.01f);

// Add child with transform
auto camera = scene.add<Camera>("mainCam")
    .position({0, 2, -10})
    .target({0, 0, 0})
    .fov(75.0f)
    .near(0.1f)
    .far(1000.0f);

// Complex setup using setters where needed
auto physics = scene.add<WavePhysics>("dynamic")
    .position({0, 5, 0})
    .mass(1.0f)
    .solidShape<SolidShape>(0.5f)      // Complex setup
    .solidShape.setMaterial("bouncy")               // Resource loading
    .onCollision([](auto& other) {       // Event handler
        // Handle collision
    });

// The same node can be accessed traditionally elsewhere
auto collider = scene.add<Solid>("collider");
if (collider.solidField.nuclearPotential > 10.0f) {
    collider.Shape[0].setMaterialArchetype("metal");
    collider.solidField.solidShape.setDynamic(false);
}

#endif