/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 *      - The idea is to have sensible defaults so that the user can focus on the unique aspects of their game.
 */
#if 0
#include "Engine.h"
#include "Scene.h"
#include "PostProcess.h"
#include "AudioFile.h"
#include "ParticleSystem.h"
#include "WavePhysics.h"
#include "Trigger.h"
#include "Node.h"

namespace hd {

class CombatArenaSetup_Example_01 : public Node {
public:
    void setup() {                                  // Going to be replace with Initialize()
        auto& engine = Engine::getInstance();
        auto& graph = engine.getNodeGraph();

        // Main scene setup
        auto scene = std::make_shared<Scene>("CombatArena");
        scene->loadGeometry("Levels/Arena/MainGeometry.usd");
        scene->setLightingSetup("DynamicBattleField");

        // Combat area trigger zones
        auto combatTrigger = std::make_shared<Trigger>("CombatZone");
        combatTrigger->setVolume(Box(Vector3(-50, 0, -50), Vector3(50, 20, 50)));
        
        // Dynamic particle effects for combat
        auto combatEffects = std::make_shared<ParticleSystem>("CombatEffects");
        combatEffects->addEmitter<SphereEmitter>()
                     ->setProperty("radius", 5.0f)
                     ->setProperty("particleLife", 2.0f);

        // Combat music system
        auto musicSystem = std::make_shared<AudioFile>("CombatMusic");
        musicSystem->loadBank("Music/Combat/Intensity.bank");
        musicSystem->setProperty("crossfadeTime", 2.0f);

        // Physics setup for destructible elements
        auto physics = std::make_shared<WavePhysics>("ArenaPhysics");
        physics->setGravity(Vector3(0, -9.81f, 0));
        physics->enableDebris(true);

        // Post-process chain
        auto postProcess = std::make_shared<PostProcess>("CombatPostProcess");
        postProcess->addEffect<Bloom>()
                   ->addEffect<MotionBlur>()
                   ->addEffect<ColorGrading>("LUTs/Combat.cube");

        // Connect the nodes
        graph.connect(combatTrigger, "OnEnter", musicSystem, "IncreaseCombatIntensity");
        graph.connect(combatTrigger, "OnEnter", combatEffects, "Activate");
        graph.connect(physics, "DebrisEmitted", combatEffects, "EmitLocation");
        graph.connect(combatEffects, "ParticleData", postProcess, "ParticleBloom");

        // Setup feedback loops
        graph.connect(musicSystem, "IntensityLevel", postProcess, "BloomIntensity");
        graph.connect(physics, "DestructionAmount", musicSystem, "CombatIntensity");
    }
};

} // namespace hd

#endif