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
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Vehicle.h"
#include "CombatVehicle.h"
#include "Weapon.h"
#include "Solid.h"
#include "Pathfinder.h"
#include "Wheel.h"
#include "Automobile.h"
#include "Drone.h"
#include "Mount.h"
#include "MountMechanics.h"
#include "MountedCombat.h"
#include "BallisticVehicle.h"
#include "TargetMechanics.h"
#include "Solid.h"
#include "SpacePropulsion.h"
#include "PhysicsFields.h"
#include "GravitationalField.h"
#include "LifeSupportMechanics.h"
#include "NavigationMechanics.h"
#include "WarHorse.h"
#include "CreatureAI.h"
#include "EmotionMechanics.h"
#include "Weapon.h"
#include "BattleZone.h"
#include "EnvironmentManager.h"
#include "WeatherCondition.h"
#include "HazardZone.h"
#include "CrowdMechanics.h"
#include "CompanionMechanics.h"
#include "EmotionMechanics.h"
#include "ScannerDevice.h"
#include "NavigatorAI.h"

using namespace hd;

// Modern tank with advanced targeting
auto mainBattleTank = Scene::current()
    .add<CombatVehicle>("abramsTank")
        .model("vehicles/tanks/abramsM1a2.fbx")
        .mass(62000.0f)
        .connect<Solid>("tankPhysics")
            .shape("tankHull")
            .connect<Pathfinder>("tracks")
                .traction(0.8f)
                .connect<Wheel>("frontLeft")
                    .radius(0.5f)
                    .connect<Wheel>("frontRight")
                        .radius(0.5f)
                    .connect<Wheel>("rearLeft")
                        .radius(0.5f)
                    .connect<Wheel>("rearRight")
                        .radius(0.5f)
        .connect<Weapon>("mainGun")
            .type("cannon120mm")
            .damage(1000.0f)
            .reloadTime(8.0f)
            .connect<BallisticVehicle>("sabotRound")
                .velocity({0.0f, 0.0f, 1800.0f})
                .penetration(800.0f)
        .connect<TargetMechanics("thermalSight")
            .range(4000.0f)
            .lockTime(2.0f)
            .thermalVision(true);

// Space exploration vessel
auto spaceShip = Scene::current()
    .add<Vehicle>("explorerShip")
        .model("vehicles/space/explorer_mk3.fbx")
        .connect<Solid>("shipPhysics")
            .mass(250000.0f)
            .connect<SpacePropulsion>("engines")
                .mainThrust(100000.0f)
                .maneuvering(true)
            .connect<GravitationalField>("artificialGravity")
                .gravitationalPotential(9.81f)
        .connect<LifeSupportMechanics>("lifeSupport")
            .oxygenCapacity(10000.0f)
            .recyclingEfficiency(0.95f)
        .connect<NavigationMechanics>("navComputer")
            .stellarMapping(true)
            .warpCapability(true);

// Cavalry unit with horse AI
auto mountedKnight = Scene::current()
    .add<WarHorse>("warhorse")
        .breed("destrier")
        .stamina(100.0f)
        .connect<CreatureAI>("horseBehavior")
            .courage(0.8f)
            .loyalty(0.9f)
            .connect<EmotionMechanics>("horseBood")
                .stress(0.0f)
                .socialization(0.5f)
        .connect<MountMechanics>("knight")
            .connect<MountedCombat>("combatSkills")
                .charge(true)
                .shield(true)
            .connect<Weapon>("lance")
                .damage(150.0f)
                .chargeBonus(2.0f);

// Battle arena with dynamic environment
auto arena = Scene::current()
    .add<BattleZone>("colosseum")
        .extents({100.0f, 30.0f, 100.0f})
        .connect<EnvironmentManager>("arenaEnvironment")
            .connect<WeatherCondition>("clearSky"))
                .temperature(25.0f)
                .windSpeed(5.0f)
            .timeOfDay(14.0f)
            .connect<HazardZone>("arenaHazards")
                .addTrap("flame_jets")
                .addTrap("spike_pits")
        .connect<CrowdMechanics>("crowd")
            .density(0.8f)
            .enthusiasm(0.7f)
            .connect<AcousticSource>("crowdNoise")
                .volume(0.6f)
                .dynamicResponse(true);

// Combat pet system
auto battlePet = Scene::current()
    .add<CompanionMechanics>("warWolf")
        .level(10)
        .loyalty(1.0f)
        .aggression(0.7f)
        .protection(0.8f)
        .connect<CombatAbilities>("wolfSkills")
            .addAbility("fierceBite")
            .addAbility("packHowl")
    .connect<EmotionMechanics>("petBond")
        .link("player1")
        .syncLevel(true)
        .bonding(0.9f)
        .damageBonus(0.15f);

// Ballistic missile system
auto missileLauncher = Scene::current()
    .add<CombatVehicle>("missilePlatform")
        .type("ballisticLauncher")
        .connect<TargetMechanics>("guidance")
            .range(25000.0f)
            .accuracy(0.98f)
            .connect<ScannerDevice>("tracking")
                .range(30000.0f)
                .jamResistance(0.7f)
        .connect<Weapon>("missileBattery")
            .capacity(4)
            .connect<BallisticVehicle>("cruiseMissile")
                .range(20000.0f)
                .speed(800.0f)
                .payload("highExplosive")
                .connect<NavigatorAI>("missileAI")
                    .trackingMode("terrainFollowing")
                    .evasionCapability(true);