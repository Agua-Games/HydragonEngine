/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Weather.h
 * @brief Weather represents a weather node in the engine's node graph.
 */
#pragma once

#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"

namespace hd {

struct WeatherInfo : public NodeInfo {
    WeatherInfo() {
        NodeType = "Weather/WeatherSystem";
        
        inputs = {
            "Time",              // Time of day/year
            "Location",          // World position
            "Temperature",       // Base temperature
            "Humidity",         // Base humidity
            "WindDirection",    // Wind vector
            "WindSpeed",        // Wind speed
            "ProceduralIntent", // For procedural variation
            "FogParams",        // Fog parameters
            "PrecipParams"      // Precipitation parameters
        };
        
        outputs = {
            "WeatherState",     // Current weather state
            "AtmosphereParams", // Atmospheric parameters
            "CloudData",        // Cloud system data
            "PrecipitationData", // Rain/snow data
            "WeatherEvents",    // Lightning, thunder, etc.
            "PerformanceMetrics"
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class Weather : public Node<WeatherState, AtmosphereParams, CloudData, PrecipitationData> {
public:
    explicit Weather(const WeatherInfo& info = WeatherInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        weatherPatternId = orchestrator.registerPattern(createDefaultWeatherPattern());
    }

    void () override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Process inputs
        auto time = getInputValue<float>("Time");
        auto location = getInputValue<glm::vec3>("Location");
        auto temperature = getInputValue<float>("Temperature");
        auto humidity = getInputValue<float>("Humidity");
        auto windDir = getInputValue<glm::vec3>("WindDirection");
        auto windSpeed = getInputValue<float>("WindSpeed");
        auto intent = getInputValue<OctaveParams>("ProceduralIntent");
        auto fogParams = getInputValue<FogParams>("FogParams");
        auto precipParams = getInputValue<PrecipitationParams>("PrecipParams");
        
        // Create weather pattern parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Weather;
        params.timeScale = time;
        params.position = location;
        params.environmentParams = {temperature, humidity};
        params.vectorParams = {windDir, windSpeed};
        
        // Update weather pattern
        weatherPatternId = orchestrator.createWeatherPattern(params);
        auto weatherData = orchestrator.getProceduralPattern(weatherPatternId);
        
        // Update outputs
        setOutputValue("WeatherState", computeWeatherState(weatherData));
        setOutputValue("AtmosphereParams", computeAtmosphereParams(weatherData));
        setOutputValue("CloudData", generateCloudData(weatherData));
        setOutputValue("PrecipitationData", generatePrecipitationData(weatherData));
        setOutputValue("WeatherEvents", generateWeatherEvents(weatherData));
        setOutputValue("PerformanceMetrics", computePerformanceMetrics());
    }

    std::vector<std::string> getInputPorts() const override {
        return getNodeInfo().inputs;
    }

    std::vector<std::string> getOutputPorts() const override {
        return getNodeInfo().outputs;
    }

private:
    std::string weatherPatternId;
    
    WeatherState computeWeatherState(const ProceduralPattern& pattern);
    AtmosphereParams computeAtmosphereParams(const ProceduralPattern& pattern);
    CloudData generateCloudData(const ProceduralPattern& pattern);
    PrecipitationData generatePrecipitationData(const ProceduralPattern& pattern);
    WeatherEvents generateWeatherEvents(const ProceduralPattern& pattern);
    PerformanceMetrics computePerformanceMetrics();
};

} // namespace hd
