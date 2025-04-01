/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HorseTypes.h
 * @brief Header file for the HorseTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - HorseTypes is a namespace that contains various types and enums related to horses.
 * - It includes types for horse types, horse breeds, and more.
 * 
 */
#pragma once

namespace hd {

enum class HorseAbility {
    Charging,
    Galloping,
    Trotting,
    Neighing,
    Herding,
    Flocking,
    Other
};

enum class HorseState {
    Idle,
    Charging,
    Galloping,
    Trotting,
    Neighing,
    Herding,
    Flocking,
    Other
};

enum class HorseType {
    Stallion,
    Mare,
    Gelding,
    Other
};

enum class HorseCoatColor {
    Black,
    Brown,
    Chestnut,
    Bay,
    Gray,
    White,
    Other
};

enum class HorseBreed {
    Arabian,
    Thoroughbred,
    QuarterHorse,
    Mustang,
    Clydesdale,
    Shetland,
    Other
};

enum class HorseArmorType {
    None,
    Light,
    Medium,
    Heavy,
    Other
};

} // namespace hd