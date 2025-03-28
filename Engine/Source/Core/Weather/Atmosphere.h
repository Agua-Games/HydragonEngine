/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Atmosphere.h
 * @brief Atmosphere represents a procedural atmosphere node in the engine's node graph.
 */
#pragma once

#include "ProceduralTypes.h"
#include "WeatherTypes.h"
#include "ProceduralManager.h"
#include "Node.h"
#include "WavePhysics.h"
#include "PhysicsFields.h"
#include "VolumetricTypes.h"

namespace hd {

// Info struct inherits from NodeInfo - used for metadata/configuration
struct AtmosphereInfo : public NodeInfo {
    AtmosphereInfo() {
        nodeType = "Weather/Atmosphere";
        
        inputs = {
            "temperature",       // Base temperature
            "humidity",         // Base humidity
            "pressure",         // Air pressure
            "timeOfDay",        // Day/night cycle
            "season",           // Seasonal influence
            "location"          // Geographic position
        };
        
        outputs = {
            "atmosphereState",  // Current state
            "scatteringParams", // Light scattering
            "densityProfile",   // Atmospheric density
            "opticalDepth"      // Light transmission
        };

        isSerializable = true;
        isEditableInEditor = true;
        isProcedural = true;
    }
};

// Actual node class inherits from Node - provides functionality
class Atmosphere : public Node {
public:
    // === Structure Definitions ===
    struct AtmosphereState {
        float temperature;
        float humidity;
        float pressure;
        float timeOfDay;
        SeasonData season;
        glm::vec3 location;
    };

    struct ScatteringParams {
        float rayleighScattering;
        float mieScattering;
        float mieDirectionalG;
    };
    struct DensityProfile {
        float troposphereDensity;
        float stratosphereDensity;
        float mesosphereDensity;
    };

    // === Allocation, Initialization, Loading ===
    explicit Atmosphere(const AtmosphereInfo& info = AtmosphereInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    AtmosphereState atmosphereState;
    PhysicsFields::AtmosphereField atmosphereField;
    ScatteringParams scatteringParams;
    DensityProfile densityProfile;
    float opticalDepth;

    // Those should be updated from the atmosphereField. If the atmosphereField is not present, we approximate them.
    vec3 windDirection;
    vec3 windSpeed;
    float density;

    // === Processing ===
    void processNode() override {
        // Process inputs
        auto temperature = getInputValue<float>("temperature");
        auto humidity = getInputValue<float>("humidity");
        auto pressure = getInputValue<float>("pressure");
        auto timeOfDay = getInputValue<float>("timeOfDay");
        auto season = getInputValue<SeasonData>("season");
        auto location = getInputValue<glm::vec3>("location");

        // Compute atmospheric state
        atmosphereState = computeAtmosphereState(
            temperature, humidity, pressure, timeOfDay, season, location
        );

        // Calculate derived parameters
        scatteringParams = computeScatteringParams(atmosphereState);
        densityProfile = computeDensityProfile(atmosphereState);
        opticalDepth = computeOpticalDepth(densityProfile);

        // Set outputs
        setOutputValue("atmosphereState", atmosphereState);
        setOutputValue("scatteringParams", scatteringParams);
        setOutputValue("densityProfile", densityProfile);
        setOutputValue("opticalDepth", opticalDepth);
    }

    void computeAtmosphereState(float temperature, float humidity, float pressure, float timeOfDay, const SeasonData& season, const glm::vec3& location) {
        // Implementation...
    }

    void computeWindApproximation() {
        // Implementation...
    }

    void updateWind() {
        // Implementation...
    }

    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Atmosphere() = default;     // Default destructor

private:
    // Implementation methods...
};

} // namespace hd
