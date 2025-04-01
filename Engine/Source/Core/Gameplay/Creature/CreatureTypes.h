/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CreatureTypes.h
 * @brief Header file for the CreatureTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - CreatureTypes is a namespace that contains various types and enums related to creatures.
 * - It includes types for creature types, creature data, and more.
 */
#pragma once

namespace hd {

enum class CreatureType {
    Animal,
    Monster,
    Other
};

enum class CreatureSpecies {
    None,
    Dog,
    Cat,
    Horse,
    Bird,
    Fish,
    Reptile,
    Amphibian,
    Mammal,
    Other
};

} // namespace hd