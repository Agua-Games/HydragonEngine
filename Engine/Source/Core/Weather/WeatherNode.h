/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_WeatherNode.h
 * @brief WeatherNode represents a weather node in the engine's node graph.
 */
#pragma once

#include "Node.h"
#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"

namespace hd {

struct HD_WeatherInfo : public NodeInfo {
    HD_WeatherInfo() {
        NodeType = "Weather/WeatherSystem";
        
        Inputs = {
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
        
        Outputs = {
            "WeatherState",     // Current weather state
            "AtmosphereParams", // Atmospheric parameters
            "CloudData",        // Cloud system data
            "PrecipitationData", // Rain/snow data
            "WeatherEvents",    // Lightning, thunder, etc.
            "PerformanceMetrics"
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class HD_WeatherNode : public Node<WeatherState, AtmosphereParams, CloudData, PrecipitationData> {
public:
    explicit HD_WeatherNode(const HD_WeatherInfo& info = HD_WeatherInfo())
        : Node(info) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        weatherPatternId = orchestrator.RegisterPattern(CreateDefaultWeatherPattern());
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
        // Process inputs
        auto time = GetInputValue<float>("Time");
        auto location = GetInputValue<glm::vec3>("Location");
        auto temperature = GetInputValue<float>("Temperature");
        auto humidity = GetInputValue<float>("Humidity");
        auto windDir = GetInputValue<glm::vec3>("WindDirection");
        auto windSpeed = GetInputValue<float>("WindSpeed");
        auto intent = GetInputValue<OctaveParams>("ProceduralIntent");
        auto fogParams = GetInputValue<FogParams>("FogParams");
        auto precipParams = GetInputValue<PrecipitationParams>("PrecipParams");
        
        // Create weather pattern parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Weather;
        params.timeScale = time;
        params.position = location;
        params.environmentParams = {temperature, humidity};
        params.vectorParams = {windDir, windSpeed};
        
        // Update weather pattern
        weatherPatternId = orchestrator.CreateWeatherPattern(params);
        auto weatherData = orchestrator.GetProceduralPattern(weatherPatternId);
        
        // Update outputs
        SetOutputValue("WeatherState", ComputeWeatherState(weatherData));
        SetOutputValue("AtmosphereParams", ComputeAtmosphereParams(weatherData));
        SetOutputValue("CloudData", GenerateCloudData(weatherData));
        SetOutputValue("PrecipitationData", GeneratePrecipitationData(weatherData));
        SetOutputValue("WeatherEvents", GenerateWeatherEvents(weatherData));
        SetOutputValue("PerformanceMetrics", ComputePerformanceMetrics());
    }

    std::vector<std::string> GetInputPorts() const override {
        return GetNodeInfo().Inputs;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return GetNodeInfo().Outputs;
    }

private:
    std::string weatherPatternId;
    
    WeatherState ComputeWeatherState(const ProceduralPattern& pattern);
    AtmosphereParams ComputeAtmosphereParams(const ProceduralPattern& pattern);
    CloudData GenerateCloudData(const ProceduralPattern& pattern);
    PrecipitationData GeneratePrecipitationData(const ProceduralPattern& pattern);
    WeatherEvents GenerateWeatherEvents(const ProceduralPattern& pattern);
    PerformanceMetrics ComputePerformanceMetrics();
};

} // namespace hd
