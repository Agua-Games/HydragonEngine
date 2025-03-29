/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code, for gameplay scenarios.
 * 
 * These examples demonstrate how to setup a node graph in code, for gameplay scenarios:
 * 
 *  - Advanced magic spell system
 *  - Intelligent NPC guard with combat and patrol
 *  - Dynamic weather system with effects
 *  - Advanced vehicle combat system
 */
#include "Engine.h"
#include "Node.h"
#include "Ability.h"
#include "PhysicsTypes.h"
#include "PhysicsFields.h"
#include "AcousticSource.h"
#include "AnimationBlend.h"
#include "BehaviorTree.h"
#include "EnvironmentManager.h"
#include "Vehicle.h"
#include "ValueGradient.h"

using namespace hd;

// Advanced Magic Spell System
auto spellSystem = Scene::current()
    .add<Ability>("frost_nova")
        .type("spell")
        .damage(50.0f)
        .cooldown(8.0f)
        .connect<ParticleSystem>("frost_particles")
            .maxParticles(1000)
            .simulationSpace(SimulationSpace::World)
            .connect<ParticleEmitter>("frost_emitter")
                .rate(100.0f)
                .shape(EmitterShape::Sphere)
                .speed({2.0f, 4.0f})
                .size({0.1f, 0.3f})
                .life({1.0f, 2.0f})
            .connect<ValueGradient>("frost_color")
                .addKey(0.0f, {0.7f, 0.9f, 1.0f, 1.0f})
                .addKey(1.0f, {0.5f, 0.8f, 1.0f, 0.0f})
        .connect<AudioProcessor>("frost_audio")
            .connect<AudioClip>("frost_cast")
                .audioFile("audio/spells/frost_cast.ogg")
                .connect<Reverb>("frost_reverb")
                    .roomSize(0.8f)
                    .wetLevel(0.3f)
            .connect<EQ>("frost_eq")
                .addBand(100.0f, 1.0f, 0.7f);

// Intelligent NPC Guard with Combat and Patrol
auto guardAI = Scene::current()
    .add<BehaviorTree>("guard_ai")
        .connect<SelectorAI>("root")
            .connect<SequenceAI>("combat")
                .connect<ConditionAI>("detect_threat")
                    .range(20.0f)
                    .fov(90.0f)
                .connect<ActionAI>("engage")
                    .connect<AnimationBlend>("combat_anims")
                        .addAnimation("ready", "anims/combat_ready.fbx")
                        .addAnimation("attack", "anims/combat_attack.fbx")
                        .addTransition("ready", "attack", 0.2f)
                    .connect<Weapon>("guard_weapon")
                        .type("sword")
                        .damage(25.0f)
            .connect<SequenceAI>("patrol")
                .connect<ActionAI>("patrol_route")
                    .connect<AnimationBlend>("patrol_anims")
                        .addAnimation("walk", "anims/walk.fbx")
                        .addAnimation("idle", "anims/idle.fbx")
                    .connect<AcousticSource>("footsteps")
                        .addAudioBank("step", "sfx/footstep.wav");

// Dynamic Weather System with Effects
auto weatherSystem = Scene::current()
    .add<EnvironmentManager>("dynamic_weather")
        .connect<ParticleSystem>("rain")
            .maxParticles(10000)
            .connect<ParticleEmitter>("rain_emitter")
                .rate(1000.0f)
                .shape(EmitterShape::Box)
                .speed({8.0f, 10.0f})
                .size({0.05f, 0.1f})
            .connect<AtmosphereField>("wind")
                .kineticEnergy({1.0f, 0.0f, 0.0f})
        .connect<AudioProcessor>("weather_audio")
            .connect<AudioClip>("rain_ambient")
                .audioFile("audio/ambient/rain_loop.ogg")
                .volume(0.8f)
                .looping(true)
            .connect<AudioClip>("thunder")
                .connect<SideChain>("thunder_duck")
                    .amount(0.5f)
                    .attack(0.1f)
                    .release(0.2f)
        .connect<PostProcess>("weather_post")
            .connect<Bloom>()
                .threshold(1.0f)
                .intensity(0.5f)
            .connect<ColorGrading>()
                .temperature(6500.0f)
                .contrast(1.1f);

// Advanced Vehicle Combat System
auto combatVehicle = Scene::current()
    .add<CombatVehicle>("assault_vehicle")
        .connect<WeaponMechanics>("main_weapon")
            .connect<Targeting>("targeting")
                .range(1000.0f)
                .lockTime(1.5f)
            .connect<ParticleSystem>("muzzle_flash")
                .connect<ParticleEmitter>("flash")
                    .rate(200.0f)
                    .life({0.1f, 0.2f})
                .connect<ValueGradient>("flash_color")
                    .addKey(0.0f, {1.0f, 0.7f, 0.0f, 1.0f})
                    .addKey(1.0f, {1.0f, 0.3f, 0.0f, 0.0f})
            .connect<AudioProcessor>("weapon_audio")
                .connect<AudioClip>("fire_sound")
                    .audioFile("audio/weapons/cannon_fire.wav")
                    .connect<Compressor>("fire_comp")
                        .threshold(-12.0f)
                        .ratio(4.0f)
        .connect<BehaviorTree>("vehicle_ai")
            .connect<SelectorAI>("combat_behavior")
                .connect<SequenceAI>("engage")
                    .connect<ConditionAI>("can_attack")
                    .connect<ActionAI>("fire_weapon")
                .connect<SequenceAI>("maneuver")
                    .connect<ActionAI>("find_cover")
                    .connect<ActionAI>("reposition");