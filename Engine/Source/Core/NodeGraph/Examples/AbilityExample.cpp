/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Basic attack ability with effects
 *  - Spell ability with targeting and effects
 *  - Passive ability with aura
 */
// Example 1: Basic attack ability with effects
#pragma once
#include "Engine.h"
#include "Node.h"
#include "SkeletonAnimation.h"
#include "Ability.h"
#include "AnimationBlend.h"
#include "AcousticSource.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Fluid.h"
#include "Projectile.h"
#include "NodePool.h"
#include "MagicSpell.h"

using namespace hd;

auto attack = Scene::current()
    .add<Ability>("slash")
        .type("melee")
        .damage(25.0f)
        .cooldown(1.5f)
        .range(2.0f)
        .connect<AnimationBlend>("attackMoves")
            .add<SkeletonAnimation>("wind_up")
                .setAnimationClip("anims/slash_start.fbx")
            .add<SkeletonAnimation>("attack")
                .setAnimationClip("anims/slash_mid.fbx")
            .add<SkeletonAnimation>("recovery")
                .setAnimationClip("anims/slash_end.fbx")
        .connect<ParticleEmitter>()
            .addEmitter("slash_trail")
                .rate(100)
                .lifetime(0.5f)
        .connect<AcousticSource>("sword")
            .addAudioBank("swing", "sfx/sword_swing.wav")
            .addAudioBank("hit", "sfx/sword_hit.wav");

// Example 2: Spell ability with targeting and effects
auto fireball = Scene::current()
    .add<NodePool>("fireballsPool")
            .connect<Projectile>("fireball")
    .add<Ability>("fireball")
        .type("projectile")
        .damage(50.0f)
        .cooldown(3.0f)
        .cost(25)
        .connect<Projectile>("fireball")
            .velocity({0.0f, 0.0f, 50.0f})      // Meters per second
            .energyMultiplier(1.5f)             // Intensify the projectile's energy, to deal more damage
            .connect<Fluid>("fire")
                .temperature(1000.0f)           // Kelvin
                .density(0.5f)                  // kg/m^3
                .viscosity(0.01f)               // Pa*s
                .conductivity(0.05f)            // W/(m*K)
                .specificHeat(1000.0f)          // J/(kg*K)
            .connect<ParticleEmitter>("fireball_trail")
                .addEmitter("fire_trail")
                    .rate(50)
                    .lifetime(1.0f)
            .connect<AcousticSource>("fireball")
                .addAudioBank("impact", "sfx/fireball_impact.wav")
            .connect<Light>("fireball_light")
                .color({1.0f, 0.5f, 0.0f})
                .intensity(2.0f)

// Example 3: Passive ability with aura
auto healingAura = Scene::current()
    .add<Ability>("healing_aura")
        .type("spell")
        .tickRate(1.0f)
        .connect<MagicSpell>()
            .range(8.0f)
            .affectMask("allies")
            .connect<Ability>("healing")
                .damage(-10.0f)
                .connect<ParticleSystem>()
                    .addEmitter("heal_sparkles")
                    .addEmitter("ground_runes")
            .connect<Ability>("Regeneration")
                .damage(-5.0f)
                .connect<ParticleSystem>()
                    .addEmitter("heal_sparkles")
                    .addEmitter("ground_runes");