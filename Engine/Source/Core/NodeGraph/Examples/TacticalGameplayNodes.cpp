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
    .add<Drone>("reconDrone")
        .connect<FlightMechanics>("droneFlight")
            .maxAltitude(100.0f)
            .hoverStability(0.9f)
            .connect<NavigationMechanics>("droneNav")
                .pathfinding(true)
                .obstacleAvoidance(true)
        .connect<SurveillanceMechanics>("droneSensors")
            .connect<EnvironmentSensor>("irVision")
                .EMDetection(true)
                .EMDetectionSensitivity(0.8f)
                .EMDeviceResolution(1024)
                .EMRangeLowEnd(EMRangeLowEnd::Infrared)
                .EMRangeHighEnd(EMRangeHighEnd::Infrared)
                .range(200.0f)
            .connect<EnvironmentSensor>("movementTrack")
                .sensitivity(0.8f)
        .connect<WeaponMechanics>("droneWeapons")
            .connect<TargetingSystem>("weaponTargeting")
                .lockTime(1.0f)
                .accuracy(0.95f);

// Tactical Gadget System
auto gadgetSystem = Scene::current()
    .add<EquipmentManager>("tacticalGear")
        .connect<Device>("heartbeatSensor")
            .range(15.0f)
            .batteryLife(300.0f)
            .signalQuality(SignalQuality::High)
            .staticNoise(0.0f)
        .connect<Device>("breachCharge")
            .inflictedDamage(500.0f)
            .damageRadius(3.0f)
            .connect<DetonationMechanics>("chargeTrigger")
                .timer(5.0f)
                .remote(true)                   // Can be detonated remotely
        .connect<Device>("tacticalCamera")
            .connect<EnvironmentSensor>("camVision")
                .nightVision(true)
            .connect<EnvironmentSensor>("camMotion")
                .EMDetection(true)
                .EMDetectionSensitivity(0.8f);

// Combat Engagement System
auto combatSystem = Scene::current()
    .add<CombatMechanics>("tacticalCombat")
        .connect<Weapon>("primaryWeapon")
            .type("assaultRifle")
            .recoilIntensity(0.2f)
            .recoilRecovery(0.5f)
            .recoilDirection(Vector3(0.0f, 1.0f, 0.0f))
            .aimType(AimType::Scoped)
            .aimStance(AimStance::Standing)
            .breathingNoise(true)
            .breathingIntensity(0.1f)
        .connect<CoverMechanics>("combatCover")
            .coverHeight(1.5f)
            .blindFireAccuracy(0.4f)
            .blindFireExposure(0.3f)
        .connect<Grenade>("tacticalGrenades")
            .connect<ThrowTrajectory>("throwArc")
                .power(0.8f)
                .damage(150.0f)
                .radius(5.0f);

// Radio Communication System
auto radioSystem = Scene::current()
    .add<RadioManager>("teamComms")
        .connect<RadioManager>("encryptedRadio")
            .encryption(true)
            .connect<RadioChannel>("secureChannel")
                .frequencyName("encrypted")
                .connect<Device>("secureVoice")
                    .signalQuality(SignalQuality::High)
                    .staticNoise(0.0f)
        .connect<RadioChannel>("squadNet")
            .frequencyName("encrypted")
            .connect<Device>("voiceChannel")
                .signalQuality(SignalQuality::High)
                .staticNoise(0.0f)
            .connect<RadioMechanics>("tacticalCommands")
                .addCommand("moveTo")
                .addCommand("holdPosition")
                .addCommand("engageTarget")
                .addResponse("acknowledged");