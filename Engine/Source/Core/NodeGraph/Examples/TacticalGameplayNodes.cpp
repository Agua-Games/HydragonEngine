/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Tactical gameplay nodes

1. Stealth Operative System:
    - Visibility and sound emission
    - Cover system with leaning
    - Advanced detection mechanics
2. Tactical Squad Coordination:
    - Formation management
    - Role-based squad structure
    - Tactical AI for engagement
3. Drone Control System:
    - Flight and navigation
    - Surveillance capabilities
    - Weapon systems
4. Tactical Gadget System:
    - Heartbeat sensors
    - Breach charges
    - Tactical cameras
5. Combat Engagement System:
    - Weapon handling and recoil
    - Cover and blind fire mechanics
    - Grenade system
6. Radio Communication System:
    - Encrypted channels
    - Voice communication
    - Tactical commands
*/

#include "Engine.h"
#include "Node.h"
#include "Combat.h"
#include "StealthMechanics.h"
#include "CoverMechanics.h"
#include "Device.h"
#include "Squad.h"

using namespace hd;

// Stealth Operative System
auto stealthSystem = Scene::current()
    .add<Character>("operative")    
    .crouchSpeed(2.0f)
    .proneSpeed(1.0f)
    .connect<StealthMechanics>("stealthMechanics")
        .visibility(0.2f)
        .soundEmission(0.1f)
        .connect<EnvironmentSensor>("stealthDetection")
            .alertnessLevel(0.0f)
            .suspicionThreshold(0.7f)
            .visionCone(60.0f)
            .visionRange(20.0f)
            .lightSensitivity(1.2f)
            .soundDetectionRadius(15.0f)
            .soundDetectionBinaural(true)
            .connect<NoiseFilter>("ambientFilter")
                .threshold(0.4f)
        .connect<CoverMechanics>("coverDetection")
            .coverHeight(1.5f)
            .leanAmount(0.5f)   
        .connect<AcousticSource>("footsteps")
            .addAudioBank("footsteps", "audio/footsteps.wav")
            .volumeMultiplier(0.5f)
        .connect<EnvironmentManager>("detectionSystem")
            .connect<EnvironmentSensor>("stealthDetection")
                .alertnessLevel(0.0f)
                .suspicionThreshold(0.7f)
                .visionCone(60.0f)
                .visionRange(20.0f)
                .lightSensitivity(1.2f)

// Tactical Squad Coordination
auto squadSystem = Scene::current()
    .add<SquadManager>("fireTeam")
        .connect<FormationSystem>("squadFormation")
            .type(FormationType::WEDGE)
            .spacing(2.0f)
            .connect<SquadMember>("pointMan")
                .role(SquadRole::LEADER)
                .connect<RadioSystem>("squadComms")
                    .channel("alpha")
                    .encrypt(true)
            .connect<SquadMember>("rifleman")
                .role(SquadRole::SUPPORT)
            .connect<SquadMember>("marksman")
                .role(SquadRole::OVERWATCH)
        .connect<TacticalAI>("squadTactics")
            .behavior(TacticalBehavior::AGGRESSIVE)
            .connect<CombatDirector>("engagementControl")
                .suppressiveFire(true)
                .flanking(true)
                .connect<ThreatAssessment>("threatEval")
                    .priority("heavyWeapons")
                    .priority("officers");

// Drone Control System
auto droneSystem = Scene::current()
    .add<DroneController>("reconDrone")
        .connect<FlightSystem>("droneFlight")
            .maxAltitude(100.0f)
            .hoverStability(0.9f)
            .connect<NavigationSystem>("droneNav")
                .pathfinding(true)
                .obstacleAvoidance(true)
        .connect<SurveillanceSystem>("droneSensors")
            .connect<ThermalCamera>("irVision")
                .resolution(1024)
                .range(200.0f)
            .connect<MotionDetector>("movementTrack")
                .sensitivity(0.8f)
        .connect<WeaponSystem>("droneWeapons")
            .connect<TargetingSystem>("weaponTargeting")
                .lockTime(1.0f)
                .accuracy(0.95f);

// Tactical Gadget System
auto gadgetSystem = Scene::current()
    .add<GadgetManager>("tacticalGear")
        .connect<Device>("heartbeatSensor")
            .range(15.0f)
            .batteryLife(300.0f)
            .connect<SignalProcessor>("vitalSigns")
                .filterNoise(true)
                .trackMultiple(true)
        .connect<Device>("breachCharge")
            .damage(500.0f)
            .radius(3.0f)
            .connect<DetonationSystem>("chargeTrigger")
                .timer(5.0f)
                .remote(true)
        .connect<Device>("tacticalCamera")
            .connect<CameraFeed>("camVision")
                .nightVision(true)
                .connect<MotionDetector>("camMotion")
                    .sensitivity(0.9f);

// Combat Engagement System
auto combatSystem = Scene::current()
    .add<CombatManager>("tacticalCombat")
        .connect<WeaponSystem>("primaryWeapon")
            .type("assaultRifle")
            .connect<RecoilSystem>("weaponRecoil")
                .pattern("verticalSpread")
                .intensity(0.3f)
            .connect<AimingSystem>("weaponAim")
                .stance(AimStance::STANDING)
                .connect<BreathControl>("steadyAim")
                    .stabilization(0.8f)
        .connect<CoverSystem>("combatCover")
            .coverHeight(1.5f)
            .connect<BlindFireSystem>("blindFire")
                .accuracy(0.4f)
                .exposure(0.3f)
        .connect<GrenadeSystem>("tacticalGrenades")
            .connect<ThrowTrajectory>("throwArc")
                .power(0.8f)
                .connect<ExplosionSystem>("grenadeEffect")
                    .damage(150.0f)
                    .radius(5.0f);

// Radio Communication System
auto radioSystem = Scene::current()
    .add<RadioManager>("teamComms")
        .connect<Channel>("squadNet")
            .frequency("encrypted")
            .connect<VoiceComm>("voiceChannel")
                .quality(VoiceQuality::HIGH)
                .connect<NoiseFilter>("radioFilter")
                    .static(0.1f)
            .connect<CommandSystem>("tacticalCommands")
                .addCommand("moveTo")
                .addCommand("holdPosition")
                .addCommand("engageTarget")
                .connect<ResponseSystem>("aiResponse")
                    .acknowledgment(true)
                    .executionTime(1.0f);