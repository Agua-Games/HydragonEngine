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
#include "PostProcessChain.h"
#include "Montage.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "LightingSetup.h"
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
        scene->loadFromFile("Levels/Arena/MainGeometry.usd");
        auto lightingSetup = std::make_shared<LightingSetup>("DynamicBattleField");
        lightingSetup->setAmbient({0.1f, 0.1f, 0.1f})
                     ->setDirectionalLight({0, -1, 0}, {0.5f, 0.5f, 0.5f})
                     ->enableDynamicShadows(true);

        // Combat area trigger zones
        auto combatTrigger = std::make_shared<Trigger>("CombatZone");
        combatTrigger->setVolume(Box(Vector3(-50, 0, -50), Vector3(50, 20, 50)));
        
        // Dynamic particle effects for combat
        auto combatFXEmitter = std::make_shared<ParticleEmitter>("CombatFXEmittter");
        combatFXEmitter->addEmitter<SphereEmitter>()
                        ->setProperty("Rate", 50.0f)
                        ->setProperty("initialVelocity", 2.0f)
                        ->setProperty("maxParticles", 1000)
        auto combatFXParticles = std::make_shared<ParticleSystem>("CombatFX");
        combatFXParticles->setProperty("color", Color(1.0f, 0.5f, 0.0f))
                         ->setProperty("size", 0.1f)
                         ->setProperty("gravity", Vector3(0, -9.81f, 0))->setProperty("radius", 5.0f)
                         ->setProperty("particleLife", 2.0f);

        // Combat music system
        auto musicSystem = std::make_shared<Montage>("CombatMusic");
        musicSystem->addTrack("Music/Intro/Ambient.bank")
                    ->addTrack("Music/Combat/Intensity.bank")
                    ->setProperty("Volume", 0.8f)
                    ->setProperty("Combat.Volume", 0.2f)
                    ->setProperty("Intro.Volume", 0.8f)
                    ->setProperty("CrossfadeTime", 2.0f);

        // Physics setup for destructible elements
        auto physics = std::make_shared<WavePhysics>("ArenaPhysics");
        physics->setConstantGravityForce(Vector3(0, -9.81f, 0))
                ->enableEnergyConservation(true);                // To spawn debris and other effects

        // Post-process chain
        auto postProcess = std::make_shared<PostProcessChain>("CombatPostProcess");
        postProcess->addPass<Bloom>()
                   ->addPass<MotionBlur>()
                   ->addPass<ColorGrading>("LUTs/Combat.cube");

        // Connect the nodes
        graph.connect(combatTrigger, "OnEnter", musicSystem, "Play.Combat");
        graph.connect(combatTrigger, "OnEnter", combatFXEmitter, "Activate");
        graph.connect(physics, "EnergyTransferEvent.Location", combatFXEmitter, "Position");
        graph.connect(combatFXParticles, "ParticleColor.R", postProcess, "BloomIntensity");

        // Setup feedback loops
        graph.connect(musicSystem, "volume", postProcess, "BloomIntensity");
        graph.connect(physics, "DestructionAmount", musicSystem, "Combat.Volume");
    }
};

} // namespace hd

#endif