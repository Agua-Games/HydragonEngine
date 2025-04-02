/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatTypes.h
 * @brief Header file for the CombatTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatTypes is a namespace that contains various types and enums related to combat.
 * - It includes types for combat vehicle types, combat vehicle data, and more.
 * 
 */
#pragma once

namespace hd {

enum WeaponRangeType {
    None,             // For when weapon is not equipped, or not equippable.
    Melee,
    Ranged,
    Thrown,           // Thrown weapons (e.g., grenades, knives, etc.)
    Custom,           // Custom range type (e.g., for modding)
};

enum WeaponType {
    None,             // For when weapon is not equipped, or not equippable.
    Club,
    Bola,
    Sword,
    Shield,
    Dagger,
    Battleaxe,
    Halberd,
    Spear,
    Mace,
    Staff,
    Bow,
    Crossbow,
    Pistol,
    Rifle,
    MachineGun,
    RocketLauncher,
    GrenadeLauncher,
    PlasmaRifle,
    PlasmaCannon,
    Mortar,
    Cannon,
    Howitzer,
    Turret,
    Launcher,
    Sentry,
    Other
};

enum CombatVehicleType {
    None,             // No combat vehicle type specified (e.g., for AI agents)
    Jeep,
    Buggy,
    MainBattleTank,
    AmphibiousAV,     // Amphibious armored vehicle (e.g., M1 Abrams, M109)
    InfantryFV,       // Infantry fighting vehicle (e.g., M113, BMP-2)
    LightTank,
    LightRecon,       // Light reconnaissance vehicle (e.g., M3 Bradley, BMP-3)
    Truck,
    ArmoredTruck,     // Armored truck (e.g., M1 Abrams)
    Helicopter,
    Bomber,           // Bomber (e.g., B-52, Tu-95, B-1B)
    ASFighter,        // Air superiority fighter (e.g., F-15, Su-35)
    Interceptor,      // Interceptor (e.g., F-22, Su-47)
    StrategicBomber,  // Strategic bomber (e.g., B-52, Tu-95, B-1B)
    TacticalBomber,   // Tactical bomber (e.g., F-111, Su-24)
    FighterBomber,    // Fighter bomber (e.g., A-10, Frogfoot)
    ArmoredPC,
    SPHowitzer,       // Self-propelled howitzer (e.g., M109, 2S19 Msta)
    Submarine,
    Corvette,
    Battleship,
    AircraftCarrier,
    Cruiser,
    Destroyer,
    Frigate,
    PatrolBoat,
    Spaceship,
    Other
};

enum AmmoType {
    None,
    Bullet,
    Shell,
    Missile,
    Rocket,
    Grenade,
    Plasma,
    Other
};

} // namespace hd