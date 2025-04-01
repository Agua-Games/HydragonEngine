***Gameplay Submodule, Nodes, and Architecture***

INTRODUCTION

- Hydragon is a game engine made initially by a creator to fellow creators, artists, programmers, and other lovers of games. It is designed to be easy to use, and to be able to create high-quality games with minimal effort. We strive to simplify things, and to make things easier, whenever possible, leading to productivity gains and quality-of-life improvements. As a small game dev studio ourselves we know very well the challenges and limitations of small studios, and we want to help you. As the engine was developed
initially for our own projects and pipeline this is a real statement, not just a slogan.

It was also created to be an in-house pipeline tool for assets, so expect some functionality directed to this purpose also.
And it was also designed to be possible to use it in headless mode, so we incentivize you to explore the node graphs created in code, and the objectivity they offer, specially if
you feel comfortable with code.

- This submodule contains the gameplay nodes for the Hydragon engine. They are used to represent gameplay elements in the game world, both with a visual representation, such as characters, items, and environments, and without a visual representation, such as gameplay events, gameplay data, and gameplay resources.

- Despite being grouped in directories (only for the sake of organization), the gameplay nodes are meant to be used in creative ways, combined and connected to create unique and interesting gameplay experiences. Think of them more as building blocks, "lego blocks" (pieces of watertight functionality), than components grouped in rigid categories.

- A good minset and workflow for creating gameplay in Hydragon would be: have your ideas, conceive your gameplay, then leverage the existing nodes to achieve it. If there's
missing functionality, you create a new node (inheriting from Node or the closest relative node) and extend it. If it's a whole new mechanics/subsystem, try to follow our
architecture standards of having: a manager, a mechanics, an entity, and an ability/extension.

- You can use these nodes both conventionally, as they are standard classes, and procedurally, as they are also nodes in the node graph (and most of them implemented with built-in procedural features), both in "code-node graphs" (code based node graphs in concise fluent style) and in UI based node graphs.

GAMEPLAY SUBSYSTEM 'PACKED' HEADERS
- We offer "packed" headers for each gameplay category, which include all the headers for that category. These headers are useful for sparing users the trouble of including all the headers for a specific gameplay set of functionality. However, we recommend using these headers sparingly, generally only if you need most or all of the functionality, as they can cause a lot of bloat when compiled. For stricter memory economy, prefer explicitly including the headers you need.

USING THE GAMEPLAY NODES
- You can use the gameplay nodes both in the editor and in code. To use them in the editor, simply drag and drop them onto the scene graph. To use them in code, you can create instances of the nodes and add them to the scene graph.

GAMEPLAY SUBSYSTEM ARCHITECTURE
- The Gameplay subsystem architecture follows a modular approach, normally setup hierarchically like this:
```
    Engine --> GameplayManager --> [GameplaySpecificManager] --> [GameplaySpecificMechanics] --> [GameplaySpecificEntity] --> [Ability/Extension]
```
- We also offer some helper header categories, for coding, such as the suffix "Type" headers, for collecting types in a single header, and the suffix "Data" headers, for data (sometimes including types), for the same purpose.

- For a comprehensive set of schemes illustrating the Gameplay subsystem architecture, please refer to the documentation.

- So, users should extend the gameplay by creating new gameplay-specific managers, mechanics, entities, abilities, and extensions. Or other categories, as needed, but sticking to, and understanding, this overall structure.

- An example node graph-eing workflow, for creating a combat system, could be:
```
    - Create a CombatManager node
    - Create a CombatMechanics node
    - Create a Character node
    - Create some CombatAbility nodes

    - Connect the CombatManager to the CombatMechanics
    - Connect the CombatMechanics to the Character
    - Connect those CombatAbility nodes to the CombatMechanics
    
    - Set maximum combat health
    - Set character health
    - Set character attack
    - Set damage for each CombatAbility
```

Or, more concisely we could write it like this (in fluent style):

```
    auto combatSystem = Scene::current()
        .add<CombatManager>("combatSystem")
            .connect<CombatMechanics>("combatMechanics")
                .maxHealth(100.0f)
                .connect<Character>("playerCharacter")
                    .health(100.0f)
                    .attack(10.0f)
                    .connect<CombatAbility>("swordAttack")
                        .damage(10.0f)
                    .connect<CombatAbility>("shieldBlock")
                        .blockChance(0.5f)
                    .connect<CombatAbility>("magicSpell")
```

But you can also use the nodes conventionally, as they are standard classes, like:
```
    auto combatSystem = std::make_shared<CombatManager>();
    auto combatMechanics = std::make_shared<CombatMechanics>();
    auto playerCharacter = std::make_shared<Character>();
    auto swordAttack = std::make_shared<CombatAbility>();
    auto shieldBlock = std::make_shared<CombatAbility>();
    auto magicSpell = std::make_shared<CombatAbility>();

    combatSystem->connect(combatMechanics);
    combatMechanics->connect(playerCharacter);
    playerCharacter->connect(swordAttack);
    playerCharacter->connect(shieldBlock);
    playerCharacter->connect(magicSpell);

    combatMechanics->setMaxHealth(100.0f);
    playerCharacter->setHealth(100.0f);
    playerCharacter->setAttack(10.0f);
    swordAttack->setDamage(10.0f);
    shieldBlock->setBlockChance(0.5f);
```

EXTENDING THE NODES: USAGE
- Users who clone the repository should not modify the gameplay nodes in this submodule. Instead, they should create their own gameplay nodes, and place them in a separate submodule. Otherwise they run into the risk of losing compatibility with the engine.

- If you really think a built-in node should be extended, please open an issue or a pull request, and we'll discuss it. And if you already have written the suggested extension, please include it in the pull request, explaining why and how.

ABILITIES AND EXTENSIONS
- Some abilities and extensions may end up integrated into the entity itself, after extensive testing and user demand prove it's the case, if deemed essential, etc.

- Some nodes may still be in design sketch phase, and may not be fully functional yet. But we'll make clear in the node header itself this state, and in the documentation.

ESSENTIAL MANAGERS
- The most essential managers are created by default at engine initialization, so there's no need to create them manually. Check the engine source code, or the documentation, 
for more details. Most of those managers are designed to be singletons.

INIT, UPDATE, CLEANUP
- Each node should implement the init, update, and cleanup methods. And these methods are called by the engine, and should not be called manually, only overridden.
