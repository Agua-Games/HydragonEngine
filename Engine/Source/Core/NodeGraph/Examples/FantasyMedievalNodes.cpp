/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * NOTE: The hierarchy of responsibilities in this and other examples is not final. It's a work in progress.
 * 
This example includes:

1. Snowy Mountain Environment:
    Snowy peaks with blizzard effects
    Ice formation system
    Cave network generation
2. Legendary Creatures:
    Dragon AI with flight mechanics
    Combat abilities and treasure system
    Territory and behavior patterns
3. Mystical Forest:
    Magical flora with effects
    Nymph spirits with behaviors
    Ambient magic system
4. Castle Fortress:
    Architectural elements
    Defense systems
    Guard patrol system
5. Warrior Guild:
    Rank progression
    Quest and reward system
    Combat training
6. Combat Systems:
    Medieval weapons and techniques
    Mounted combat and cavalry
    Magic and enchantment system

 *  - Here's the proper hierarchy and nesting of nodes:
 * 
 *      Engine --> GameplayManager --> [GameplaySpecificManager] --> [GameplaySpecificMechanics] --> [Entity] --> [Ability/Extension]
*/

#include "Engine.h"
#include "Node.h"
#include "Fortress.h"
#include "MagicSpell.h"
#include "Character.h"
#include "Creature.h"
#include "EnvironmentManager.h"
#include "FlightMechanics.h"
#include "AcousticZone.h"

using namespace hd;

// Snowy Mountain Environment
auto mountainRealm = Scene::current()
    .add<Scene>("frostPeaks")
        .connect<DynamicEnvLighting>("frostEnvLighting")
            .clouds("snowy")
        .connect<WavePhysicsSolver>("globalSolver")
            .gravity({0.0f, -9.8f, 0.0f})
        .connect<Atmosphere>("frostAtmosphere")
            .windDirection({1.0f, 0.0f, 0.0f})
            .windSpeed(0.5f)
            .density(0.5f)
        .connect<AudioManager>("audioManager")
            .addAudioBank("windGust")
            .addAudioBank("snowCrunch")
        .connect<WeatherManager>("weatherManager")
        .connect<WeatherCondition>("blizzard")
            .temperature(-20.0f)
            .windSpeed(15.0f)
            .snowDensity(0.8f)
            .connect<VisibilityManager>("snowVision")
                .viewDistance(50.0f)
                .connect<ParticleSystem>("snowFx")
                    .emitter("snowFlakes")
                    .emitter("windTrails")
        .connect<TerrainManager>("terrainManager")
        .connect<Terrain>("peak")
            .resolution(1024)
            .connect<Heightmap>("peakHeightmap")
                .scale(100.0f)
                .connect<Noise>("peakNoise")
                    .octaves(4)
                    .lacunarity(2.0f)
            .connect<IceFormation>("frozenSurface")
                .thickness(2.0f)
                .slipperiness(0.9f)
            .connect<CaveNetwork>("mountainCaves")
                .generatePaths(true)
                .connect<AcousticZone>("caveAcoustics")
                    .echoes(true)
                    .darknessBias(0.8f);

// Legendary Creature System
auto ancientDragon = Scene::current()
    .add<LegendaryBeast>("frostDragon")
        .connect<CreatureAI>("dragonBehavior")
            .aggression(0.8f)
            .territory(1000.0f)
            .connect<FlightMechanics>("dragonFlight")
                .wingSpan(30.0f)
                .altitude(500.0f)
            .connect<CombatAbilities>("dragonPowers")
                .addAbility("frostBreath")
                    .damage(200.0f)
                    .freezeEffect(true)
                .addAbility("wingGust")
                    .force(50.0f)
                    .radius(20.0f)
        .connect<LootTable>("dragonTreasure")
            .addItem("ancientScroll", 1.0f)
            .addItem("dragonScales", 0.8f)
            .addItem("frostCrystal", 0.5f);

// Mystical Forest
auto enchantedForest = Scene::current()
    .add<MysticForest>("elvenWoods")
        .connect<VegetationSystem>("magicalFlora")
            .addPlant("glowshrooms")
                .luminosity(0.5f)
            .addPlant("spiritTrees")
                .connect<AuraEffect>("treeBlessing")
                    .radius(10.0f)
                    .healing(0.1f)
        .connect<NymphSystem>("forestSpirits")
            .density(0.01f)
            .connect<BehaviorTree>("spiritAi")
                .addBehavior("dance")
                .addBehavior("hide")
                .addBehavior("bless")
        .connect<AmbientMagic>("forestMagic")
            .intensity(0.7f)
            .connect<ParticleSystem>("magicFx")
                .emitter("fireflies")
                .emitter("leafSpirits");

// Castle Fortress
auto castleKeep = Scene::current()
    .add<Castle>("northernFortress")
        .connect<ArchitectureSystem>("castleStructure")
            .addSection("outerWalls")
                .height(15.0f)
                .connect<DefenseSystem>("wallDefenses")
                    .addTurret("cornerTower")
                    .addElement("battlements")
            .addSection("greatHall")
                .connect<InteriorSystem>("hallInterior")
                    .addFeature("throne")
                    .addFeature("hearth")
        .connect<GarrisonSystem>("castleGarrison")
            .connect<GuardPatrol>("guardRoutes")
                .addPath("ramparts")
                .addPath("courtyard")
            .connect<AlertSystem>("castleAlerts")
                .connect<SignalSystem>("warningBells");

// Warrior Guild
auto warriorGuild = Scene::current()
    .add<GuildSystem>("warriorsGuild")
        .connect<RankSystem>("guildRanks")
            .addRank("initiate", 1)
            .addRank("warrior", 5)
            .addRank("veteran", 10)
            .addRank("master", 20)
        .connect<QuestBoard>("guildMissions")
            .addQuest("slayDragon")
            .addQuest("protectCaravan")
            .connect<RewardSystem>("questRewards")
                .gold(true)
                .reputation(true)
        .connect<TrainingSystem>("combatTraining")
            .addStyle("swordShield")
            .addStyle("twoHanded")
            .addStyle("archery");

// Combat System
auto combatSystem = Scene::current()
    .add<MedievalCombat>("knightCombat")
        .connect<WeaponSystem>("meleeWeapons")
            .addWeapon("longsword")
                .damage(50.0f)
                .connect<CombatMoves>("swordTechniques")
                    .addMove("overheadStrike")
                    .addMove("thrust")
                    .addMove("parry")
            .addWeapon("battleAxe")
                .damage(70.0f)
                .armorPiercing(0.3f)
        .connect<ArmorSystem>("plateArmor")
            .protection(0.7f)
            .weight(50.0f)
            .connect<EnchantmentSystem>("magicArmor")
                .addEffect("frostResist")
                .addEffect("magicWard");

// Mounted Combat System
auto mountedCombat = Scene::current()
    .add<MountedCombat>("warHorse")
        .connect<HorseAI>("horseBehavior")
            .courage(0.8f)
            .stamina(100.0f)
            .connect<MovementSystem>("horseMovement")
                .gallop(true)
                .jump(true)
        .connect<CavalrySystem>("mountedCombat")
            .connect<ChargeMechanic>("lanceCharge")
                .damage(150.0f)
                .momentum(true)
            .connect<MountedArchery>("horseArchery")
                .accuracy(0.6f)
                .stability(0.7f);

// Magic System
auto magicSystem = Scene::current()
    .add<MagicSystem>("elementalMagic")
        .connect<SpellBook>("frostMagic")
            .addSpell("iceSpike")
                .damage(80.0f)
                .freezeChance(0.3f)
            .addSpell("frostShield")
                .protection(0.5f)
                .duration(30.0f)
        .connect<ManaSystem>("magicPower")
            .maxMana(100.0f)
            .regeneration(0.1f)
        .connect<EnchantingSystem>("weaponEnchanting")
            .addEnchantment("frostBlade")
            .addEnchantment("soulTrap")
            .connect<RuneSystem>("magicRunes")
                .addRune("protection")
                .addRune("power");