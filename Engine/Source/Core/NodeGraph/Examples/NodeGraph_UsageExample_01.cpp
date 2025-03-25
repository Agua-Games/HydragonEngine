/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NodeGraph_UsageExample_01.cpp
 * @brief This file contains examples of how to setup a node graph in code.
 */

 #if 0
using namespace hd;

// Example 1: Creating a simple render pipeline
auto& renderer = Engine::get().renderer()
    .add<Camera>()
        .position({0, 5, -10})
        .target({0, 0, 0})
    .add<Mesh>("player")
        .load("models/player.fbx")
        .material("materials/pbr.mat");

// Example 2: Setting up a character with physics
auto& character = Scene::current()
    .add<Transform>()
        .position({0, 0, 0})
        .connect<RigidBody>()
            .mass(70.0f)
            .connect<CapsuleCollider>()
                .radius(0.5f)
                .height(2.0f);

// Example 3: Procedural building generation
auto& building = Scene::current()
    .add<ProceduralBuilding>()
        .floors(5)
        .style("modern")
        .connect<MeshGenerator>()
            .connect<CollisionGenerator>();

// Example 4: Audio system setup
auto& audio = Engine::get().audio()
    .add<AcousticSource>("background")
        .file("music/ambient.ogg")
        .loop(true)
        .volume(0.5f)
    .add<AcousticListener>()
        .connect<Transform>();

// Example 5: Processing chain
auto& processor = ImageProcessor::create()
    .add<ColorCorrection>()
        .contrast(1.2f)
        .saturation(1.1f)
    .connect<Blur>(5.0f)
    .connect<Sharpen>(0.3f);

    #endif
