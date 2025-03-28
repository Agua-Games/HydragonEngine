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

#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"
#include "Core/Gameplay/Medieval.h"
#include "Core/Gameplay/Magic.h"
#include "Core/Gameplay/Creature.h"
#include "Core/Gameplay/Environment.h"

using namespace hd;

// Snowy Mountain Environment
auto mountainRealm = Scene::current()
    .add<Scene>("frostPeaks")
        .connect<DynamicEnvLighting>("frostEnvLighting")
            .clouds("snowy")
        .connect<WavePhysicsSolver>("GlobalSolver")
            .gravity({0.0f, -9.8f, 0.0f})
        .connect<Atmosphere>("frostAtmosphere")
            .windDirection({1.0f, 0.0f, 0.0f})
            .windSpeed(0.5f)
            .density(0.5f)
        .connect<AudioManager>("AudioManager")
            .addAudioBank("wind_gust")
            .addAudioBank("snow_crunch")
        .connect<WeatherManager>("Weather")
        .connect<Weather>("blizzard")
            .temperature(-20.0f)
            .windSpeed(15.0f)
            .snowDensity(0.8f)
            .connect<VisibilitySystem>("snow_vision")
                .viewDistance(50.0f)
                .connect<ParticleSystem>("snow_fx")
                    .emitter("snowflakes")
                    .emitter("wind_trails")
        .connect<TerrainSystem>("mountainTerrain")
            .heightmap("terrain/peaks.height")
            .connect<IceFormation>("frozen_surface")
                .thickness(2.0f)
                .slipperiness(0.9f)
        .connect<CaveNetwork>("mountain_caves")
            .generatePaths(true)
            .connect<AmbientSystem>("cave_atmosphere")
                .echoes(true)
                .darkness(0.8f);

// Legendary Creature System
auto ancientDragon = Scene::current()
    .add<LegendaryBeast>("frost_dragon")
        .connect<CreatureAI>("dragon_behavior")
            .aggression(0.8f)
            .territory(1000.0f)
            .connect<FlightSystem>("dragon_flight")
                .wingSpan(30.0f)
                .altitude(500.0f)
            .connect<CombatAbilities>("dragon_powers")
                .addAbility("frost_breath")
                    .damage(200.0f)
                    .freezeEffect(true)
                .addAbility("wing_gust")
                    .force(50.0f)
                    .radius(20.0f)
        .connect<LootTable>("dragon_treasure")
            .addItem("ancient_scroll", 1.0f)
            .addItem("dragon_scales", 0.8f)
            .addItem("frost_crystal", 0.5f);

// Mystical Forest
auto enchantedForest = Scene::current()
    .add<MysticForest>("elven_woods")
        .connect<VegetationSystem>("magical_flora")
            .addPlant("glowshrooms")
                .luminosity(0.5f)
            .addPlant("spirit_trees")
                .connect<AuraEffect>("tree_blessing")
                    .radius(10.0f)
                    .healing(0.1f)
        .connect<NymphSystem>("forest_spirits")
            .density(0.01f)
            .connect<BehaviorTree>("spirit_ai")
                .addBehavior("dance")
                .addBehavior("hide")
                .addBehavior("bless")
        .connect<AmbientMagic>("forest_magic")
            .intensity(0.7f)
            .connect<ParticleSystem>("magic_fx")
                .emitter("fireflies")
                .emitter("leaf_spirits");

// Castle Fortress
auto castleKeep = Scene::current()
    .add<Castle>("northern_fortress")
        .connect<ArchitectureSystem>("castle_structure")
            .addSection("outer_walls")
                .height(15.0f)
                .connect<DefenseSystem>("wall_defenses")
                    .addTurret("corner_tower")
                    .addElement("battlements")
            .addSection("great_hall")
                .connect<InteriorSystem>("hall_interior")
                    .addFeature("throne")
                    .addFeature("hearth")
        .connect<GarrisonSystem>("castle_garrison")
            .connect<GuardPatrol>("guard_routes")
                .addPath("ramparts")
                .addPath("courtyard")
            .connect<AlertSystem>("castle_alerts")
                .connect<SignalSystem>("warning_bells");

// Warrior Guild
auto warriorGuild = Scene::current()
    .add<GuildSystem>("warriors_guild")
        .connect<RankSystem>("guild_ranks")
            .addRank("initiate", 1)
            .addRank("warrior", 5)
            .addRank("veteran", 10)
            .addRank("master", 20)
        .connect<QuestBoard>("guild_missions")
            .addQuest("slay_dragon")
            .addQuest("protect_caravan")
            .connect<RewardSystem>("quest_rewards")
                .gold(true)
                .reputation(true)
        .connect<TrainingSystem>("combat_training")
            .addStyle("sword_shield")
            .addStyle("two_handed")
            .addStyle("archery");

// Combat System
auto combatSystem = Scene::current()
    .add<MedievalCombat>("knight_combat")
        .connect<WeaponSystem>("melee_weapons")
            .addWeapon("longsword")
                .damage(50.0f)
                .connect<CombatMoves>("sword_techniques")
                    .addMove("overhead_strike")
                    .addMove("thrust")
                    .addMove("parry")
            .addWeapon("battle_axe")
                .damage(70.0f)
                .armorPiercing(0.3f)
        .connect<ArmorSystem>("plate_armor")
            .protection(0.7f)
            .weight(50.0f)
            .connect<EnchantmentSystem>("magic_armor")
                .addEffect("frost_resist")
                .addEffect("magic_ward");

// Mounted Combat System
auto mountedCombat = Scene::current()
    .add<MountedCombat>("warhorse")
        .connect<HorseAI>("horse_behavior")
            .courage(0.8f)
            .stamina(100.0f)
            .connect<MovementSystem>("horse_movement")
                .gallop(true)
                .jump(true)
        .connect<CavalrySystem>("mounted_combat")
            .connect<ChargeMechanic>("lance_charge")
                .damage(150.0f)
                .momentum(true)
            .connect<MountedArchery>("horse_archery")
                .accuracy(0.6f)
                .stability(0.7f);

// Magic System
auto magicSystem = Scene::current()
    .add<MagicSystem>("elemental_magic")
        .connect<SpellBook>("frost_magic")
            .addSpell("ice_spike")
                .damage(80.0f)
                .freezeChance(0.3f)
            .addSpell("frost_shield")
                .protection(0.5f)
                .duration(30.0f)
        .connect<ManaSystem>("magic_power")
            .maxMana(100.0f)
            .regeneration(0.1f)
        .connect<EnchantingSystem>("weapon_enchanting")
            .addEnchantment("frost_blade")
            .addEnchantment("soul_trap")
            .connect<RuneSystem>("magic_runes")
                .addRune("protection")
                .addRune("power");