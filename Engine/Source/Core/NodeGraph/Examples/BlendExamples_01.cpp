/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Simple blend between two textures
 *  - Complex blend with blend parameters and weights
 */
#if 0
// Simple blend - uses optimized path
auto simpleBlend = graph.create<Blend>("simpleBlend")
    .connect("sourceA", texture1)
    .connect("sourceB", texture2)
    .setFactor(0.5f);  // Fast path, no parameter overhead

// Complex blend - full feature set
auto complexBlend = graph.create<Blend>("complexBlend")
    .connect("walk", walkAnim)
    .connect("run", runAnim)
    .addBlendParameter("speed")     // Switches to complex mode
    .addBlendParameter("direction")
    .setBlendWeight(0, 0.7f);
#endif