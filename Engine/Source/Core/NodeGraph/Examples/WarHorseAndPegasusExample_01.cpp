/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * This example showcases proper responsibilities hierarchy between the different nesting levels, and how it all fits together.
 * Note the similarity with concise hierarchical code like html or xml, in the pattern, but in our case most times we use a command (semantic action)
 * rather than an object (semantic substantive). Pattern: command<DataType>("name").
 * And also some similarity with other concise descriptive languages, like css in the dot notation where we even skip the scope: .connect.
 * This way we achieve the best of three or more worlds:
 *      - We stay in C++, which is the most performant, full-featured language for game development.
 *      - We use a node graph, which is the most flexible, extensible and encapsulated/modular way to describe gameplay.
 *      - We use a dot notation, which is the most concise and readable way to describe gameplay.
 *      - We use a command pattern, which is the most flexible and extensible way to describe gameplay. And neatly fits with Vulkan's pattern, so
 *      we can seamlessly integrate with it, paving the way to use Vulkan more broadly in the engine, if so we decide.
 *      - We use a language pattern which may be even more concise than Python in some sense. So much so that the code itself is almost the same length,
 *      in lines and tokens, as the architectural notes I'm writing down here at the top of these files.
 * 
 * So, a coder, just by reading an example like this, can understand the architecture of the game. And with the API in hand, can easily modify it. And by coder
 * I mean both human and AI.
 * 
 *  - Here's the proper hierarchy and nesting of nodes:
 * 
 *      Engine --> GameplayManager --> [GameplaySpecificManager] --> [GameplaySpecificMechanics] --> [Entity] --> [Ability/Extension]
 * 
 *  - Some abilities and extensions may end up integrated into the entity itself, after extensive testing and user demand prove it's the case, if deemed essential, etc.
 *  - Warhorse and Pegasus example with nested nodes
 */
// 1. Engine level
Engine::get()
    // 2. GameplayManager level
    .gameplay()
        // 3. Mechanics level
        .connect<MountMechanics>("mountMechanics") // Clearly a management node
            .enableHalterGuidance(true)
            // 4. Sub-mechanics level
            .connect<WarHorseMechanics>("warHorseMechanics") // Clearly a behavior rules node
                // 5. Entity level
                .connect<WarHorse>("knight_steed") // Clearly an entity node
                    .setBreed("destrier")
                    .setArmor("plate_barding")
                    // 6. Ability/Extension level
                    .connect<DodgeAttackAbility>("dodgeAttack") // Clearly an ability node. Candidate to be integrated into the entity itself.
                        .setStaminaCost(10.0f)
                        .setSpeedBonus(2.0f)
                    .connect<RescueRiderAbility>("rescueRider") // Clearly an extension node. Candidate to be integrated into the entity itself.
                        .setStability(0.8f)
                        .enableRearingAttack(true);

                        
Scene::current()
    .add<AerialManager>("sky_traffic")            // Clearly a management node
        .add<FlyingMountMechanics>("pegasi")      // Clearly a behavior rules node
            .add<Pegasus>("player_mount")         // Clearly an entity node
                .addAbility<WingAttackAbility>(); // Clearly an ability node