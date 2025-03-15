/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcAtmosphereNode.h
 * @brief ProcAtmosphereNode represents a procedural atmosphere node in the engine's node graph.
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
        
        Inputs = {
            "Temperature",       // Base temperature
            "Humidity",         // Base humidity
            "Pressure",         // Air pressure
            "TimeOfDay",        // Day/night cycle
            "Season",           // Seasonal influence
            "Location"          // Geographic position
        };
        
        Outputs = {
            "AtmosphereState",  // Current state
            "ScatteringParams", // Light scattering
            "DensityProfile",   // Atmospheric density
            "OpticalDepth"      // Light transmission
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

// Actual node class inherits from Node - provides functionality
class ProcAtmosphereNode : public Node<AtmosphereState, ScatteringParams, DensityProfile> {
public:
    explicit ProcAtmosphereNode(const AtmosphereInfo& info = AtmosphereInfo())
        : Node(info) {}

    void ProcessNodeGraph() override {
        // Process inputs
        auto temperature = GetInputValue<float>("Temperature");
        auto humidity = GetInputValue<float>("Humidity");
        auto pressure = GetInputValue<float>("Pressure");
        auto timeOfDay = GetInputValue<float>("TimeOfDay");
        auto season = GetInputValue<SeasonData>("Season");
        auto location = GetInputValue<glm::vec3>("Location");

        // Compute atmospheric state
        AtmosphereState state = ComputeAtmosphereState(
            temperature, humidity, pressure, timeOfDay, season, location
        );

        // Calculate derived parameters
        ScatteringParams scattering = ComputeScatteringParams(state);
        DensityProfile density = ComputeDensityProfile(state);
        float opticalDepth = ComputeOpticalDepth(density);

        // Set outputs
        SetOutputValue("AtmosphereState", state);
        SetOutputValue("ScatteringParams", scattering);
        SetOutputValue("DensityProfile", density);
        SetOutputValue("OpticalDepth", opticalDepth);
    }

private:
    // Implementation methods...
};

} // namespace hd
