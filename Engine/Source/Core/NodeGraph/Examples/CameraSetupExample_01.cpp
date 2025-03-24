/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */
#if 0
// Example showing camera's typical usage context
auto& scene = renderer
    .add<Camera>()
        .position({0, 5, -10})
        .target({0, 0, 0})
    .connect<RenderPass>()
        .setViewport()
        .setClearColor();

#endif