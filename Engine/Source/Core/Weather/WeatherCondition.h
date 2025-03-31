/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WeatherCondition.h
 * @brief WeatherCondition represents a weather node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - WeatherCondition is a class that represents a specific wholesome weather condition in Hydragon, such as Windy Storm, Blizzard, Rainy Fog, etc.
 * - It uses the Vulkan API (Compute) for weather processing.
 * - Not to be confused with WeatherManager, which is a class that manages the weather in the game. Also not to be confused with WeatherTypes, 
 * which is a class that represents the types of weather conditions in Hydragon.
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 */

#pragma once

#include "Node.h"
#include "WeatherTypes.h"

namespace hd {

struct WeatherConditionInfo : public NodeInfo {
    WeatherConditionInfo() {
        nodeType = "Weather/WeatherCondition";
        
        inputs = {
            "weatherType",      // Type of weather (rain, snow, etc.)
            "weatherData",      // Weather data
            "environment",      // Environment data
            "characterData",    // Character data
            "weatherState"      // Weather state
        };
        
        outputs = {
            "weatherStatus",    // Weather status
            "weatherMetrics"    // Weather performance metrics
        };
    }
};

class WeatherCondition : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit WeatherCondition(const WeatherConditionInfo& info = WeatherConditionInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    WeatherTypes weatherType;
    float temperature;
    float humidity;
    float windSpeed;
    float visibility;
    float snowDensity;
    float fogDensity;
    float precipitationIntensity;
    float lightningFrequency;
    float thunderFrequency;
    float cloudCoverage;
    float cloudHeight;
    float cloudSpeed;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~WeatherCondition() = default;     // Default destructor
};

} // namespace hd
