/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * These examples demonstrate:

    - Vehicle Damage System:
    - Layered armor with different materials
    - Critical hit zones with special effects
    - Subsystem damage affecting performance
    - Field repair mechanics
    - Space Exploration:
    - Advanced propulsion with hazard detection
    - Resource management and life support
    - Long-range scanning and anomaly detection
    - Automated docking and trading
    - Battle Arena Modes:
    - Multiple game modes (Capture Points, King of the Hill)
    - Dynamic hazards and environmental effects
    - Spectator and betting systems
    - Arena-wide effects
    - Combined Arms:
    - Multiple vehicle types working together
    - Team-based objectives
    - AI commanders with different strategies
    - Weather and terrain effects on combat
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "Vehicle.h"
#include "Spaceship.h"
#include "CombatVehicle.h"
#include "DamageZone.h"
#include "Propulsion.h"
#include "SpacePropulsion.h"
#include "SpaceNavigation.h"
#include "EnvironmentManager.h"
#include "PhysicsFields.h"
#include "ResourceManager.h"
#include "ScannerDevice.h"
#include "AnomalyDetector.h"
#include "LifeSupportMechanics.h"
#include "EmergencyMechanics.h"
#include "DockingMechanics.h"
#include "NavigatorAI.h"
#include "TradingMechanics.h"
#include "BattleZone.h"
#include "Waypoint.h"
#include "GameplayManager.h"
#include "BettingMechanics.h"
#include "SpectatorMechanics.h"
#include "BattleManager.h"
#include "TeamManager.h"

using namespace hd;

// === Advanced Vehicle Damage System ===
auto armoredVehicle = Scene::current()
    .add<CombatVehicle>("mbtChallenger")
        .scene("vehicles/tanks/challenger2.usd")
        .addArmor("composite", 150.0f)
        .addArmor("reactive", 100.0f)
        .addWeapon("mainCannon")
            .damage(1000.0f)
            .rateOfFire(1.0f)
        .connect<DamageZone>("exhaustDamageZone")
            .multiplier(3.0f)
            .damageChance(0.7f)
        .connect<DamageZone>("engineDamageZone")
            .multiplier(2.0f)
            .damageChance(0.4f)
    .connect<EquipmentManager>("mbtChallengerEquipments")
        .connect<Turret>("sideTurret")
            .health(50.0f)
            .accuracy(0.9f)
        .connect<Equipment>("EMJammingAntenna")
        .connect<Equipment>("droneDetector")
            .health(100.0f)
        .connect<Equipment>("transmission")
            .health(100.0f)
            .connect<PhysicsPhenomenon>("jamming");
    .connect<RepairMechanics>("fieldRepairs")
        .connect<RepairKit>("basicRepairKit")
            .healAmount(30.0f)
            .repairTime(10.0)
        .connect<RepairKit>("advancedRepairKit")
            .healAmount(70.0f)
            .repairTime(20.0f)
            .requiresCrew(true);

// === Space Exploration Mechanics ===
auto deepSpaceVessel = Scene::current()
    .add<Spaceship>("deepSpaceExplorer")
        .connect<SpaceshipMechanics>("shipSystems")
            .addSystem<SpacePropulsion>("warpDrive")
                .fuelType("antimatter")
                .maxWarpFactor(9.5f)
                .connect<WarpField>("subspaceBubble")
                    .stabilityPotential(Field<float, 3>(1.0f, 0.0f, 0.0f))
                    .warpPotential(Field<vec3, 3>(0.0f, 1.0f, 0.0f))
                    .connect<HazardZone>("subspaceEddies")
                        .detectionRange(50000.0f)
            .addSystem<ScannerDevice>("longRangeScanner")
                .range(100000.0f)
                .connect<AnomalyDetector>("spacePhenomena")
                    .addSignature("blackHoles")
                    .addSignature("wormholes")
                    .connect<NavigatorAI>("hazardAvoidance")
            .addSystem<ResourceManager>("shipResources")
                .addResource("fuel", 10000.0f)
                .addResource("oxygen", 5000.0f)
                .connect<LifeSupportMechanics>("lifeSupport")
                    .efficiency(0.98f)
                    .connect<EmergencyMechanics>("criticalResources")
                        .threshold(0.1f)  // 10% remaining
        .connect<DockingMechanics>("dockingMechanics")
            .automaticDocking(true)
            .connect<TradingMechanics>("cargoManagement")
                .capacity(1000.0f)
                .autoTrade(true);

// === Battle Arena Game Modes ===
auto arenaManager = Scene::current()
    .add<BattleZone>("megaArena")
        .dimensions({200.0f, 50.0f, 200.0f})
        .connect<GameplayManager>("arenaModes")
            .addMode("capturePoints")
                .connect<Waypoint>("capturePoints")
                    .addPoint({50.0f, 0.0f, 50.0f})
                    .addPoint({-50.0f, 0.0f, -50.0f})
                .captureTime(30.0f)
                .scoreLimit(1000)
            .addMode("kingofTheHill")
                .hillRadius(20.0f)
                .moveInterval(60.0f)
                .connect<HazardZone>("closingCircle")
                    .shrinkRate(0.5f)
                    .damage(10.0f)
        .connect<EnvironmentManager>("dynamicArena")
            .addHazard("lavaFloor")
                .damage(50.0f)
                .spreadRate(0.1f)
                .connect<ParticleSystem>("lavaFx")
            .addHazard("lightningStorm")
                .frequency(0.2f)
                .damage(100.0f)
        .connect<SpectatorMechanics>("arenaViewers")
            .enableReplay(true)
            .connect<BettingMechanics>("arenaBets")
                .oddsCalculation(true);

// === Combined Arms Scenario ===
auto battleManager = Scene::current()
    .add<BattleManager>("combinedArms")
        .connect<TeamManager>("forces")
            .addTeam("blueForce", {
                .add<CombatVehicle>("mainTank")
                    .type("abrams")
                    .position({100.0f, 0.0f, 100.0f})
                    .connect<AICommander>("tankAi")
                        .strategy("aggressive")
                        .supportRange(50.0f),
                .add<Vehicle>("transportHelo")
                    .type("blackhawk")
                    .altitude(100.0f)
                    .connect<AICommander>("heloAi")
                        .strategy("support")
            })
            .addTeam("redForce", {
                .add<CombatVehicle>("artillery")
                    .type("mlrs")
                    .connect<IndirectFire>("artilleryAi")
                        .maxRange(2000.0f),
                .add<Vehicle>("drone")
                    .type("reconUav")
                    .connect<ReconAI>("droneAi")
                        .spotting(true)
            })
        .connect<ObjectiveManager>("missionControl")
            .addObjective("captureBridge")
                .location(0.0f, 0.0f, 0.0f)
                .radius(50.0f)
                .reward(1000)
            .connect<ReinforcementSystem>("support")
                .addReinforcement("airStrike")
                    .cooldown(300.0f)
                    .damage(500.0f)
        .connect<WeatherSystem>("battlefieldConditions")
            .setWeather("rain")
            .visibility(0.7f)
            .connect<TerrainSystem>("groundConditions")
                .mudFactor(0.5f)
                .traction(0.7f);