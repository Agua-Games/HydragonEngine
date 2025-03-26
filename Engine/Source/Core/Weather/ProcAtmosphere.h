/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcAtmosphere.h
 * @brief ProcAtmosphere represents a procedural atmosphere node in the engine's node graph.
 */
#pragma once

#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"
#include "Node.h"

namespace hd {

// Info struct inherits from NodeInfo - used for metadata/configuration
struct AtmosphereInfo : public NodeInfo {
    AtmosphereInfo() {
        NodeType = "Weather/Atmosphere";
        
        inputs = {
            "Temperature",       // Base temperature
            "Humidity",         // Base humidity
            "Pressure",         // Air pressure
            "TimeOfDay",        // Day/night cycle
            "Season",           // Seasonal influence
            "Location"          // Geographic position
        };
        
        outputs = {
            "AtmosphereState",  // Current state
            "ScatteringParams", // Light scattering
            "DensityProfile",   // Atmospheric density
            "OpticalDepth"      // Light transmission
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

// Actual node class inherits from Node - provides functionality
class ProcAtmosphere : public Node<AtmosphereState, ScatteringParams, DensityProfile> {
public:
    explicit ProcAtmosphere(const AtmosphereInfo& info = AtmosphereInfo())
        : Node(info) {}

    void () override {
        // Process inputs
        auto temperature = getInputValue<float>("Temperature");
        auto humidity = getInputValue<float>("Humidity");
        auto pressure = getInputValue<float>("Pressure");
        auto timeOfDay = getInputValue<float>("TimeOfDay");
        auto season = getInputValue<SeasonData>("Season");
        auto location = getInputValue<glm::vec3>("Location");

        // Compute atmospheric state
        AtmosphereState state = computeAtmosphereState(
            temperature, humidity, pressure, timeOfDay, season, location
        );

        // Calculate derived parameters
        ScatteringParams scattering = computeScatteringParams(state);
        DensityProfile density = computeDensityProfile(state);
        float opticalDepth = computeOpticalDepth(density);

        // Set outputs
        setOutputValue("AtmosphereState", state);
        setOutputValue("ScatteringParams", scattering);
        setOutputValue("DensityProfile", density);
        setOutputValue("OpticalDepth", opticalDepth);
    }

private:
    // Implementation methods...
};

} // namespace hd
