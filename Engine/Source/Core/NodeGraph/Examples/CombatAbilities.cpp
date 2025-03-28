/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * These examples demonstrate:

    - Complex ability chains with multiple effects
    - Resource generation and management
    - Status effect application
    - Particle and sound effect integration
    - Combat mechanics like combo systems and chain reactions
    - Area effects and healing mechanics
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Projectile.h"
#include "Ability.h"
#include "DamageZone.h"

using namespace hd;

// Ice Mage's frost bolt with chain reaction
auto frostBolt = Scene::current()
    .add<Ability>("frost_bolt")
        .type("projectile")
        .damage(35.0f)
        .cooldown(2.0f)
        .connect<Projectile>("ice_projectile")
            .velocity({0.0f, 0.0f, 40.0f})
            .connect<ParticleEmitter>("frost_trail")
                .rate(30)
                .lifetime(0.8f)
            .connect<AcousticSource>("frost_sounds")
                .addAudioBank("launch", "sfx/frost_launch.wav")
                .addAudioBank("impact", "sfx/frost_impact.wav")
        .connect<ChainReaction>("frost_chain")
            .maxTargets(3)
            .chainRange(5.0f)
            .damageReduction(0.2f)  // Each subsequent target takes 20% less damage
            .connect<StatusEffect>("frozen")
                .duration(2.0f)
                .movementPenalty(0.5f);

// Berserker's whirlwind attack with rage buildup
auto whirlwind = Scene::current()
    .add<Ability>("whirlwind")
        .type("melee_aoe")
        .damage(15.0f)
        .tickRate(0.2f)
        .duration(3.0f)
        .connect<DamageZone>("spin_damage")
            .radius(3.0f)
            .connect<StatusEffect>("bleeding")
                .duration(4.0f)
                .tickDamage(5.0f)
        .connect<ResourceGenerator>("rage")
            .perHit(5.0f)
            .connect<StatusEffect>("berserk")
                .threshold(50.0f)
                .damageBonus(0.3f)
        .connect<ParticleSystem>("spin_fx")
            .addEmitter("sword_trail")
            .addEmitter("blood_splatter")
        .connect<CameraShake>("impact_shake")
            .intensity(0.2f)
            .frequency(10.0f);

// Rogue's shadow strike with stealth mechanics
auto shadowStrike = Scene::current()
    .add<Ability>("shadow_strike")
        .type("dash_attack")
        .damage(75.0f)
        .range(10.0f)
        .connect<MovementController>("dash")
            .speed(50.0f)
            .connect<StatusEffect>("invulnerable")
                .duration(0.5f)
        .connect<StealthSystem>("shadow_form")
            .fadeTime(0.3f)
            .connect<ParticleEmitter>("shadow_trail")
                .rate(50)
                .lifetime(0.5f)
        .connect<ComboSystem>("backstab")
            .multiplier(2.0f)
            .condition("behind_target")
            .connect<StatusEffect>("exposed")
                .duration(3.0f)
                .defenseReduction(0.2f);

// Paladin's holy shield with reactive healing
auto holyShield = Scene::current()
    .add<Ability>("holy_shield")
        .type("defensive")
        .duration(6.0f)
        .cooldown(12.0f)
        .connect<DamageAbsorber>("divine_shield")
            .capacity(100.0f)
            .connect<HealingEffect>("divine_response")
                .conversion(0.5f)  // 50% of absorbed damage converted to healing
                .radius(8.0f)
                .connect<ParticleEmitter>("holy_pulse")
                    .burstCount(20)
                    .scale({2.0f, 2.0f, 2.0f})
        .connect<AuraEffect>("protection")
            .radius(5.0f)
            .connect<StatusEffect>("blessed")
                .duration(3.0f)
                .damageReduction(0.15f);