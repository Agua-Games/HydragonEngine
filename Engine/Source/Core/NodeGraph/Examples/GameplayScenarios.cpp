/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * These examples showcase:

    - Complex vehicle systems with physics, weapons, and targeting
    - Space exploration with life support and navigation
    - Mount/rider dynamics with AI and emotion systems
    - Battle arena environment with hazards and spectators
    - Pet/companion systems with AI and bonding mechanics
    - Advanced military vehicles with guidance and targeting
 */
#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"
#include "Core/Gameplay/Vehicle.h"
#include "Core/Gameplay/CombatVehicle.h"
#include "Core/Gameplay/Automobile.h"

using namespace hd;

// Modern tank with advanced targeting
auto mainBattleTank = Scene::current()
    .add<CombatVehicle>("abrams_tank")
        .model("vehicles/tanks/abrams_m1a2.fbx")
        .mass(62000.0f)
        .connect<WavePhysics>("tank_physics")
            .solidShape("tank_hull")
            .connect<TrackSystem>("tracks")
                .gripFactor(0.85f)
                .terrainResponse(true)
        .connect<WeaponSystem>("main_gun")
            .type("cannon_120mm")
            .damage(1000.0f)
            .reloadTime(8.0f)
            .connect<BallisticProjectile>("sabot_round")
                .velocity({0.0f, 0.0f, 1800.0f})
                .penetration(800.0f)
        .connect<TargetingSystem>("thermal_sight")
            .range(4000.0f)
            .lockTime(2.0f)
            .thermalVision(true);

// Space exploration vessel
auto spaceShip = Scene::current()
    .add<Vehicle>("explorer_ship")
        .model("vehicles/space/explorer_mk3.fbx")
        .connect<SpacePhysics>("ship_physics")
            .mass(250000.0f)
            .connect<PropulsionSystem>("engines")
                .mainThrust(100000.0f)
                .maneuvering(true)
            .connect<GravityField>("artificial_gravity")
                .strength(9.81f)
                .radius(50.0f)
        .connect<LifeSupportSystem>("life_support")
            .oxygenCapacity(10000.0f)
            .recyclingEfficiency(0.95f)
        .connect<NavigationSystem>("nav_computer")
            .stellarMapping(true)
            .warpCapability(true);

// Cavalry unit with horse AI
auto mountedKnight = Scene::current()
    .add<Mount>("warhorse")
        .breed("destrier")
        .stamina(100.0f)
        .connect<AnimalAI>("horse_behavior")
            .courage(0.8f)
            .loyalty(0.9f)
            .connect<EmotionSystem>("horse_mood")
                .stress(0.0f)
                .calmness(1.0f)
        .connect<RiderSystem>("knight")
            .mountedCombat(true)
            .connect<WeaponSystem>("lance")
                .damage(150.0f)
                .chargeBonus(2.0f);

// Battle arena with dynamic environment
auto arena = Scene::current()
    .add<BattleArea>("colosseum")
        .dimensions({100.0f, 30.0f, 100.0f})
        .connect<EnvironmentSystem>("arena_environment")
            .weather("dynamic")
            .timeOfDay(14.0f)
            .connect<HazardSystem>("arena_hazards")
                .addTrap("flame_jets")
                .addTrap("spike_pits")
        .connect<SpectatorSystem>("crowd")
            .density(0.8f)
            .enthusiasm(0.7f)
            .connect<AudioSystem>("crowd_noise")
                .volume(0.6f)
                .dynamicResponse(true);

// Combat pet system
auto battlePet = Scene::current()
    .add<CompanionSystem>("war_wolf")
        .level(10)
        .loyalty(1.0f)
        .connect<PetAI>("wolf_behavior")
            .aggression(0.7f)
            .protection(0.8f)
            .connect<CombatAbilities>("wolf_skills")
                .addAbility("fierce_bite")
                .addAbility("pack_howl")
        .connect<BondingSystem>("pet_bond")
            .ownerLink("player_1")
            .syncLevel(true)
            .connect<StatusEffect>("pack_strength")
                .duration(-1.0f)  // Permanent while bonded
                .damageBonus(0.15f);

// Ballistic missile system
auto missileLauncher = Scene::current()
    .add<CombatVehicle>("missile_platform")
        .type("ballistic_launcher")
        .connect<TargetingSystem>("guidance")
            .range(25000.0f)
            .accuracy(0.98f)
            .connect<RadarSystem>("tracking")
                .scanRange(30000.0f)
                .jamResistance(0.7f)
        .connect<WeaponSystem>("missile_battery")
            .capacity(4)
            .connect<BallisticProjectile>("cruise_missile")
                .range(20000.0f)
                .speed(800.0f)
                .payload("high_explosive")
                .connect<GuidanceSystem>("missile_ai")
                    .trackingMode("terrain_following")
                    .evasionCapability(true);