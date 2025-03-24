/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */

 #if 0
// Animation blend tree
auto anim = graph.create<AnimatorNode>("characterAnim")
    .connect<BlendSpaceNode>("locomotion")
        .parameter("speed", 0.0f)
        .parameter("direction", 0.0f)
        .addState({0.0f, 0.0f}, "animations/idle.anim")
        .addState({1.0f, 0.0f}, "animations/walk.anim")
        .addState({2.0f, 0.0f}, "animations/run.anim")
    .connect<Blend>("upperBody")
        .addInput("animations/aim.anim", 0.0f)
        .addMask("upperBody")
    .connect<StateMachineNode>("actions")
        .addState("idle")
        .addState("attack", "animations/attack.anim")
        .addTransition("idle", "attack")
            .condition("attack_trigger")
            .crossfade(0.2f);

#endif