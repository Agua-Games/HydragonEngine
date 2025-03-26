/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DynamicEnvLighting.h
 * @brief Header file for the DynamicEnvLighting class.
 * 
 * ARCHITECTURAL NOTES:
 * - DynamicEnvLighting is a node that represents dynamic environment lighting (Time-of-day, weather, etc.) in the scene.
 * - It can be used to create different types of dynamic environment lighting with various properties.
 * - It uses the Vulkan API for light management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include "Node.h"
#include "Light.h"

namespace hd {

struct DynamicEnvLightingInfo : public LightInfo {
    DynamicEnvLightingInfo() {
        NodeType = "Rendering/DynamicEnvLighting";
        inputs.insert(inputs.end(), {
            "TimeOfDay",                 // Time of day (day, night, etc.)
            "Weather",                   // Weather conditions (sunny, rainy, etc.)
            "Season",                    // Season (spring, summer, etc.)
            "Location",                  // Geographic location
            "SkylightStrength",          // Strength of skylight
        });
        outputs.insert(outputs.end(), {
            "DynamicLightData",          // Dynamic light data for rendering
            "DynamicEnvironmentData"     // Dynamic environment data for rendering (e.g. skylight)
        });
    }
};

class DynamicEnvLighting : public Light {
public:
    // === Structure Definitions ===
    struct DynamicLightData {
        
    };
    struct DynamicEnvironmentData {
        
    };

    struct Skylight {
        vec3 color;
        float intensity = 20000.0f;     // Based on real-world values, in lux (e.g. 20,000 lux for a clear sky) - this is normalized to 1.0f in the engine.
        float quality;
        std::string capturePath;
        float captureInterval;
    };

    struct Sun {
        vec3 color;
        // Based on real-world values, in lux (e.g. 100,000 lux for direct sunlight, usually 90,000-120,000) - this is normalized to 1.0f in the engine.
        float intensity = 100000.0f;
        float quality;
    };

    struct Atmosphere {
        float density;
        float MieScattering;            // Mie scattering coefficient, sampled from the AtmosphereField. If absent use default value.
        float MieAbsorption;            // Mie absorption coefficient, sampled from the AtmosphereField. If absent use default value.
        float RayleighScattering;       // Rayleigh scattering coefficient, sampled from the AtmosphereField. If absent use default value.
    };

    struct Fog {
        float density;
        vec3 color;
        float startDistance;
        float endDistance;
    };

    struct Clouds {
        float density;
        vec3 color;
        float startDistance;
        float endDistance;
    };

    struct TimeOfDay {
        float time;
        // ...
    };

    struct Weather {
        std::string type;
        // ...
    };

    struct Season {
        float 1.0f;         // Normalized season value (0.0f = winter, 0.25f = spring, 0.5f = summer, 0.75f = fall, 1.0f = winter)
        // ...
    };

    struct Location {
        float latitude;
        float longitude;
        // ...
    };

    // === Allocation, Initialization, Loading ===
    explicit DynamicEnvLighting(const DynamicEnvLightingInfo& info = DynamicEnvLightingInfo())
        : Light(info) {}   
    initialize() override {}
    load() override {}

    Skylight skylight;
    Sun sun;
    Fog fog;
    Clouds clouds;
    Atmosphere atmosphere;
    Weather weather;
    TimeOfDay timeOfDay;
    Season season;
    Location location;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~DynamicEnvLighting() = default;     // Default destructor
};

} // namespace hd