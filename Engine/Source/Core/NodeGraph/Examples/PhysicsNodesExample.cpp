/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Physics setup with rigid bodies and colliders
 *  - Vehicle physics with suspension and wheels
 */
#if 0
#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"

using namespace hd;

// Physics setup
auto physics = graph.create<PhysicsNode>("vehiclePhysics")
    .mass(1500.0f)
    .connect<RigidBodyNode>("chassis")
        .position({0.0f, 0.5f, 0.0f})
        .connect<BoxColliderNode>("body")
            .size({4.0f, 1.5f, 2.0f})
            .offset({0.0f, 0.0f, 0.0f})
        .connect<WheelNode>("wheelFL")
            .radius(0.4f)
            .width(0.2f)
            .suspension(0.2f)
            .damping(0.3f)
            .position({-0.8f, 0.0f, 1.0f})
        .connect<WheelNode>("wheelFR")
            .radius(0.4f)
            .width(0.2f)
            .suspension(0.2f)
            .damping(0.3f)
            .position({0.8f, 0.0f, 1.0f});

#endif