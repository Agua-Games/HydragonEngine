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
#include "CombatVehicle.h"
#include "DamageZone.h"

using namespace hd;

// === Advanced Vehicle Damage System ===
auto armoredVehicle = Scene::current()
    .add<CombatVehicle>("mbtChallenger")
        .scene("vehicles/tanks/challenger2.usd")
        .addArmor("composite", 150.0f)
        .addArmor("reactive", 100.0f)
        .addWeapon("main_cannon")
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
    .connect<RepairMechanics>("field_repairs")
        .connect<RepairKit>("basicRepairKit")
            .healAmount(30.0f)
            .repairTime(10.0)
        .connect<RepairKit>("advancedRepairKit")
            .healAmount(70.0f)
            .repairTime(20.0f)
            .requiresCrew(true);

// === Space Exploration Mechanics ===
auto deepSpaceVessel = Scene::current()
    .add<Vehicle>("deep_space_explorer")
        .connect<SpaceshipMechanics>("ship_systems")
            .addSystem<PropulsionSystem>("warp_drive")
                .fuelType("antimatter")
                .maxWarpFactor(9.5f)
                .connect<WarpField>("subspace_bubble")
                    .stability(0.95f)
                    .connect<NavigationHazard>("subspace_eddies")
                        .detectionRange(50000.0f)
            .addSystem<ScanningSystem>("long_range_scanner")
                .range(100000.0f)
                .connect<AnomalyDetector>("space_phenomena")
                    .addSignature("black_holes")
                    .addSignature("wormholes")
                    .connect<AutoNavigator>("hazard_avoidance")
            .addSystem<ResourceManager>("ship_resources")
                .addResource("fuel", 10000.0f)
                .addResource("oxygen", 5000.0f)
                .connect<RecyclingSystem>("life_support")
                    .efficiency(0.98f)
                    .connect<EmergencyProtocol>("critical_resources")
                        .threshold(0.1f)  // 10% remaining
        .connect<SpaceStationDocking>("docking_system")
            .automaticDocking(true)
            .connect<TradingSystem>("cargo_management")
                .capacity(1000.0f)
                .autoTrade(true);

// === Battle Arena Game Modes ===
auto arenaManager = Scene::current()
    .add<BattleArea>("mega_arena")
        .dimensions({200.0f, 50.0f, 200.0f})
        .connect<GameModeManager>("arena_modes")
            .addMode("capture_points")
                .points({50.0f, 0.0f, 50.0f},
                    {-50.0f, 0.0f, -50.0f},
                    {0.0f, 20.0f, 0.0f})
                .captureTime(30.0f)
                .scoreLimit(1000)
            .addMode("king_of_hill")
                .hillRadius(20.0f)
                .moveInterval(60.0f)
                .connect<HazardRing>("closing_circle")
                    .shrinkRate(0.5f)
                    .damage(10.0f)
        .connect<EnvironmentManager>("dynamic_arena")
            .addHazard("lava_floor")
                .damage(50.0f)
                .spreadRate(0.1f)
                .connect<ParticleSystem>("lava_fx")
            .addHazard("lightning_storm")
                .frequency(0.2f)
                .damage(100.0f)
        .connect<SpectatorSystem>("arena_viewers")
            .enableReplay(true)
            .connect<BettingSystem>("arena_bets")
                .oddsCalculation(true);

// === Combined Arms Scenario ===
auto battleManager = Scene::current()
    .add<BattleManager>("combined_arms")
        .connect<TeamManager>("forces")
            .addTeam("blue_force", {
                .add<CombatVehicle>("main_tank")
                    .type("abrams")
                    .position({100.0f, 0.0f, 100.0f})
                    .connect<AICommander>("tank_ai")
                        .strategy("aggressive")
                        .supportRange(50.0f),
                .add<Vehicle>("transport_helo")
                    .type("blackhawk")
                    .altitude(100.0f)
                    .connect<AICommander>("helo_ai")
                        .strategy("support")
            })
            .addTeam("red_force", {
                .add<CombatVehicle>("artillery")
                    .type("mlrs")
                    .connect<IndirectFire>("artillery_ai")
                        .maxRange(2000.0f),
                .add<Vehicle>("drone")
                    .type("recon_uav")
                    .connect<ReconAI>("drone_ai")
                        .spotting(true)
            })
        .connect<ObjectiveManager>("mission_control")
            .addObjective("capture_bridge")
                .location(0.0f, 0.0f, 0.0f)
                .radius(50.0f)
                .reward(1000)
            .connect<ReinforcementSystem>("support")
                .addReinforcement("air_strike")
                    .cooldown(300.0f)
                    .damage(500.0f)
        .connect<WeatherSystem>("battlefield_conditions")
            .setWeather("rain")
            .visibility(0.7f)
            .connect<TerrainSystem>("ground_conditions")
                .mudFactor(0.5f)
                .traction(0.7f);