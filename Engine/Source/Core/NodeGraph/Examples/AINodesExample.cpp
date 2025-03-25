/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */

 #if  0
// AI behavior tree
auto ai = graph.create<BehaviorTreeNode>("enemyAI")
    .connect<SelectorNode>("root")
        .connect<SequenceNode>("combat")
            .connect<ConditionNode>("canSeePlayer")
                .range(20.0f)
                .fov(90.0f)
            .connect<ActionNode>("moveToRange")
                .preferredRange(10.0f)
                .minRange(5.0f)
            .connect<ActionNode>("attack")
                .cooldown(2.0f)
        .connect<SequenceNode>("patrol")
            .connect<ActionNode>("setPatrolPoints")
                .setWaypointsFromGroup("patrolPoints")
            .connect<ActionNode>("moveToPoint")
                .speed(2.0f)
                .tolerance(0.5f)
            .connect<ActionNode>("wait")
                .duration({1.0f, 3.0f});

#endif