/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */
#if 0
#include "Engine.h"
#include "Node.h"

using namespace hd;

// Example 1: Creating a texture node with a raw resource
// Simplified usage - raw resource automatically created and pooled
auto albedoNode1 = scene.create<TextureNode>("textures/metal.png")
    .filter(VK_FILTER_LINEAR)
    .addressMode(VK_SAMPLER_ADDRESS_MODE_REPEAT);

// Second node using same (raw) resource automatically
auto albedoNode2 = scene.create<TextureNode>("textures/metal.png")  // same path = same resource
    .filter(VK_FILTER_NEAREST)
    .addressMode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    
#endif