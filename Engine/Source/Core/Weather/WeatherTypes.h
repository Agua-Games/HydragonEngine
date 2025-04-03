/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WeatherTypes.h
 * @brief Header file for the WeatherTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - WeatherTypes is a namespace that contains various types and enums related to weather.
 * - It includes types for weather conditions, weather events, and more.
 * 
 */
#pragma once

namespace hd {

// Define weather types
enum class WeatherType {
    Clear,
    Cloudy,
    Rainy,
    Snowy,
    Thunderstorm,
    Foggy,
    Custom
};

// Define weather events
enum class WeatherEvent {
    Lightning,
    Thunder,
    Wind,
    Snowfall,
    Rainfall,
    Fog,
    Custom
};

enum class CloudType {
    Cumulus,
    Stratus,
    Cirrus,
    Cumulonimbus,
    Other
};

} // namespace hd