/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 *      - The idea is to have sensible defaults so that the user can focus on the unique aspects of their game.
 */

 #if 0
// Animation blend tree
auto anim = graph.create<AnimationSystem>("characterAnim")
    .connect<BlendSpace>("locomotion")
        .blendParameter("speed", 0.0f)
        .blendParameter("direction", 0.0f)
        .addState("idle", {0.0f, 0.0f}, "animations/idle.anim")
        .addState("walk", {1.0f, 0.0f}, "animations/walk.anim")
        .addState("run", {2.0f, 0.0f}, "animations/run.anim")
    .connect<Blend>("upperBody", BlendMode::Add)
        .connect<Animation>("animations/aim.anim")
        .connect<Animation>("animations/attack.anim")
        .factor(0.5f)
        .mask("upperBody")
    .connect<StateMachine>("actions")
        .addState("idle")
        .addState("attack", "animations/attack.anim")
        .addTransition("idle", "attack")
            .condition("attack_trigger")
            .crossfade(0.2f);

#endif