/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Advanced magic spell system
 *  - Intelligent NPC guard with combat and patrol
 *  - Dynamic weather system with effects
 *  - Advanced vehicle combat system
 */
#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"

using namespace hd;

// Advanced Magic Spell System
auto spellSystem = Scene::current()
    .add<Ability>("frost_nova")
        .type("spell")
        .damage(50.0f)
        .cooldown(8.0f)
        .connect<ParticleSystemNode>("frost_particles")
            .maxParticles(1000)
            .simulation(SimulationSpace::World)
            .connect<EmitterNode>("frost_emitter")
                .rate(100.0f)
                .shape(EmitterShape::Sphere)
                .speed({2.0f, 4.0f})
                .size({0.1f, 0.3f})
                .life({1.0f, 2.0f})
            .connect<ColorOverLifeNode>("frost_color")
                .addKey(0.0f, {0.7f, 0.9f, 1.0f, 1.0f})
                .addKey(1.0f, {0.5f, 0.8f, 1.0f, 0.0f})
        .connect<AudioProcessor>("frost_audio")
            .connect<AudioClip>("frost_cast")
                .file("audio/spells/frost_cast.ogg")
                .connect<Reverb>("frost_reverb")
                    .roomSize(0.8f)
                    .wetLevel(0.3f)
            .connect<EQ>("frost_eq")
                .addBand(100.0f, 1.0f, 0.7f);

// Intelligent NPC Guard with Combat and Patrol
auto guardAI = Scene::current()
    .add<BehaviorTreeNode>("guard_ai")
        .connect<SelectorNode>("root")
            .connect<SequenceNode>("combat")
                .connect<ConditionNode>("detect_threat")
                    .range(20.0f)
                    .fov(90.0f)
                .connect<ActionNode>("engage")
                    .connect<AnimationSystem>("combat_anims")
                        .addState("ready", "anims/combat_ready.fbx")
                        .addState("attack", "anims/combat_attack.fbx")
                        .addTransition("ready", "attack", 0.2f)
                    .connect<WeaponSystem>("guard_weapon")
                        .type("sword")
                        .damage(25.0f)
            .connect<SequenceNode>("patrol")
                .connect<ActionNode>("patrol_route")
                    .connect<AnimationSystem>("patrol_anims")
                        .addState("walk", "anims/walk.fbx")
                        .addState("idle", "anims/idle.fbx")
                    .connect<AudioEmitter>("footsteps")
                        .addSound("step", "sfx/footstep.wav");

// Dynamic Weather System with Effects
auto weatherSystem = Scene::current()
    .add<EnvironmentSystem>("dynamic_weather")
        .connect<ParticleSystemNode>("rain")
            .maxParticles(10000)
            .connect<EmitterNode>("rain_emitter")
                .rate(1000.0f)
                .shape(EmitterShape::Box)
                .speed({8.0f, 10.0f})
                .size({0.05f, 0.1f})
            .connect<ForceFieldNode>("wind")
                .direction({1.0f, -0.1f, 0.0f})
                .strength(2.0f)
        .connect<AudioProcessor>("weather_audio")
            .connect<AudioClip>("rain_ambient")
                .file("audio/ambient/rain_loop.ogg")
                .volume(0.8f)
                .looping(true)
            .connect<AudioClip>("thunder")
                .connect<SideChain>("thunder_duck")
                    .amount(0.5f)
                    .attack(0.1f)
                    .release(0.2f)
        .connect<PostProcess>("weather_post")
            .connect<BloomEffect>()
                .threshold(1.0f)
                .intensity(0.5f)
            .connect<ColorGrading>()
                .temperature(6500.0f)
                .contrast(1.1f);

// Advanced Vehicle Combat System
auto combatVehicle = Scene::current()
    .add<CombatVehicle>("assault_vehicle")
        .connect<WeaponSystem>("main_weapon")
            .connect<TargetingSystem>("targeting")
                .range(1000.0f)
                .lockTime(1.5f)
            .connect<ParticleSystemNode>("muzzle_flash")
                .connect<EmitterNode>("flash")
                    .rate(200.0f)
                    .life({0.1f, 0.2f})
                .connect<ColorOverLifeNode>("flash_color")
                    .addKey(0.0f, {1.0f, 0.7f, 0.0f, 1.0f})
                    .addKey(1.0f, {1.0f, 0.3f, 0.0f, 0.0f})
            .connect<AudioProcessor>("weapon_audio")
                .connect<AudioClip>("fire_sound")
                    .file("audio/weapons/cannon_fire.wav")
                    .connect<Compressor>("fire_comp")
                        .threshold(-12.0f)
                        .ratio(4.0f)
        .connect<BehaviorTreeNode>("vehicle_ai")
            .connect<SelectorNode>("combat_behavior")
                .connect<SequenceNode>("engage")
                    .connect<ConditionNode>("can_attack")
                    .connect<ActionNode>("fire_weapon")
                .connect<SequenceNode>("maneuver")
                    .connect<ActionNode>("find_cover")
                    .connect<ActionNode>("reposition");