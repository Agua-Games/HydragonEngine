/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file FuelTypes.h
 * @brief Header file for the FuelTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - FuelTypes is a namespace that contains various types and enums related to fuel.
 * - It includes types for fuel types, fuel properties, and more.
 * 
 */
#pragma once

namespace hd {

enum class FuelType {
    Solid,
    Liquid,
    Gas,
    Plasma,
    Isotope,
    Fusion,
    Antimatter,
    SolarWind,     // Solar wind propulsion system (e.g., solar sails)
    Custom
};

enum SpacePropulsionType {
    None,
    SolidGas,
    LiquidGas,           // Liquid gas propulsion system (e.g., ion engines)
    SolidPropellant,     // Solid propellant propulsion system (e.g., rocket engines)
    LiquidPropellant,    // Liquid propellant propulsion system (e.g., jet engines)
    Plasma,              // Plasma propulsion system (e.g., fusion engines)
    Isotope,             // Isotope propulsion system (e.g., nuclear engines)
    Fusion,              // Fusion propulsion system (e.g., fusion engines)
    Antimatter,          // Antimatter propulsion system (e.g., antimatter engines)
    SolarWind,           // Solar wind propulsion system (e.g., solar sails)
    WarpDrive,
    Hyperdrive,
    JumpDrive,
    Custom
};

} // namespace hd