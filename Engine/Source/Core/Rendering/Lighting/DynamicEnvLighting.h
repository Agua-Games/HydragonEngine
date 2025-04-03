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
 * 
 * @todo We include Atmosphere.h here to sample its properties, but Atmosphere (class) takes care of more than just lighting - thermal radiation, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <string>
#include "Node.h"
#include "Light.h"           // Used for the sun light.
#include "Mesh.h"            // Used for the sky sphere. Maybe replace with AdaptiveMesh, if we decide to use it as the standard mesh type.
#include "Material.h"        // Used for the sky sphere.
#include "Atmosphere.h"      // We could include PhysicsFields.h instead, to use the AtmosphereField directly, but Atmosphere wraps it and adds more functionality.
#include "WeatherManager.h"
#include "WeatherTypes.h"

namespace hd {

struct DynamicEnvLightingInfo : public NodeInfo {
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

class DynamicEnvLighting : public Node {
public:
    // === Structure Definitions ===
    struct DynamicLightData {
        
    };
    struct DynamicEnvironmentData {
        
    };

    struct Sun {
        vec3 color;
        // Based on real-world values, in lux (e.g. 100,000 lux for direct sunlight, usually 90,000-120,000) - this is normalized to 1.0f in the engine.
        float intensity = 100000.0f;
        float quality;
    };

    struct Skylight {
        vec3 color;
        float intensity = 20000.0f;     // Based on real-world values, in lux (e.g. 20,000 lux for a clear sky) - this is normalized to 1.0f in the engine.
        float quality;
        std::string capturePath;
        float captureInterval;
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
        CloudType type;
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
        WeatherType type;
        // ...
    };

    struct Season {
        float value = 1.0f;         // Normalized season value (0.0f = winter, 0.25f = spring, 0.5f = summer, 0.75f = fall, 1.0f = winter)
        // ...
    };

    struct Location {
        float latitude;
        float longitude;
        // ...
    };

    enum class AmbientSource {
        Skylight,
        IBL,
        Sun,
        Other
    };

    struct Ambient {
        AmbientSource source;
        vec3 color;
        float intensity;
    };

    // === Allocation, Initialization, Loading ===
    explicit DynamicEnvLighting(const DynamicEnvLightingInfo& info = DynamicEnvLightingInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    Mesh skySphere;             // TODO: Decide if we use a Skybox instead. If the scene capture to render target is ok using a skysphere, we prefer it.
    Material skySphereMaterial;
    Skylight skylight;
    Sun sun;
    Fog fog;
    Clouds clouds;
    Atmosphere atmosphere;
    Weather weather;
    TimeOfDay timeOfDay;
    Season season;
    Location location;
    Ambient ambient;

    Sun::intensity = 100000.0f;
    Sun::quality = 1.0f;
    Sun::color = vec3(1.0f);

    Skylight::intensity = 20000.0f;
    Skylight::quality = 1.0f;
    Skylight::capturePath = "textures/sky_day.hdr";
    Skylight::captureInterval = 1.0f;

    Atmosphere::density = 1.0f;
    Atmosphere::MieScattering = 0.0f;
    Atmosphere::MieAbsorption = 0.0f;
    Atmosphere::RayleighScattering = 0.0f;

    Fog::density = 0.01f;
    Fog::color = vec3(0.5f);
    Fog::startDistance = 100.0f;
    Fog::endDistance = 1000.0f;

    Clouds::type = CloudType::Cumulus;
    Clouds::density = 0.01f;
    Clouds::color = vec3(0.8f);
    Clouds::startDistance = 100.0f;
    Clouds::endDistance = 1000.0f;

    Weather::type = WeatherType::Clear;

    ambient::source = AmbientSource::Skylight; // Default source is skylight.
    ambient::color = vec3(0.1f);               // Default color is gray. Should be sampled from the skybox.
    ambient::intensity = 0.1f;                 // TODO: Decide if we should use 1.0 as the default (= full Skylight contribution).

    bool dynamicTime = true;                   // Of course, time in the game is always dynamic, but here we mean the time of day, season (sun azimuth changes, etc.)
    int dayLength = 24;         // In hours
    int seasonLength = 3;       // In months
    int yearLength = 365;       // In days
    bool dynamicWeather = true;

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