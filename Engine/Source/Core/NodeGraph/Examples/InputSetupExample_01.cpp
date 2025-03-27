/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Input setup with contexts and actions
 */
#if 0

// Menu context
auto& menuInput = UI::current()
    .add<InputListener>()
        .setContext("menu")
        .mapAction("select", GLFW_KEY_ENTER)
        .mapAction("back", GLFW_KEY_ESCAPE);

// Game context
auto& playerInput = player
    .add<InputListener>()
        .setContext("gameplay")
        .mapAction("jump", GLFW_KEY_SPACE);

#endif