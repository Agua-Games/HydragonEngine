/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */
#if 0
#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"

using namespace hd;

auto node = graph.create<TransformNode>("worldRoot")
    .position({0, 5, -10})
    .rotation({0, 45, 0})
    .scale(2.0f)
    .visible(true);

auto material = graph.create<Material>("metal")
    .baseColor({0.8f, 0.8f, 0.8f})
    .roughness(0.5f)
    .metallic(1.0f)
    .albedoMap("textures/metal_albedo.png")
    .normalMap("textures/metal_normal.png");

#endif