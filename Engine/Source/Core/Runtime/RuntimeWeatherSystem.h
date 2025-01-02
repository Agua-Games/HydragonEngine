/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime weather simulation and environmental effects
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeWeatherConfig {
    bool enableVolumetricClouds = true;
    bool enableDynamicWind = true;
    bool enablePrecipitation = true;
    bool enableTimeOfDay = true;
    uint32_t maxWeatherZones = 16;
    uint32_t particleSystemsPerZone = 8;
    float weatherTransitionTime = 3.0f;
    float windUpdateInterval = 0.5f;
    std::string weatherPresetPath = "Config/Weather";
};

class RuntimeWeatherSystem {
public:
    static RuntimeWeatherSystem& Get();
    
    void Initialize(const RuntimeWeatherConfig& config = {});
    void Shutdown();
    
    void Update();
    
    WeatherZoneHandle CreateZone(const WeatherZoneDesc& desc);
    void DestroyZone(WeatherZoneHandle handle);
    
    void SetWeatherState(WeatherZoneHandle handle, const WeatherState& state, float transitionTime = -1.0f);
    void BlendWeatherStates(WeatherZoneHandle handle, const WeatherState& from, const WeatherState& to, float duration);
    
    void SetPrecipitationType(WeatherZoneHandle handle, PrecipitationType type);
    void SetPrecipitationIntensity(WeatherZoneHandle handle, float intensity);
    
    void SetWindDirection(WeatherZoneHandle handle, const Vector3& direction);
    void SetWindSpeed(WeatherZoneHandle handle, float speed);
    void AddWindGust(WeatherZoneHandle handle, const WindGustDesc& desc);
    
    void SetCloudCoverage(WeatherZoneHandle handle, float coverage);
    void SetCloudHeight(WeatherZoneHandle handle, float baseHeight, float topHeight);
    
    void LoadWeatherPreset(const std::string& name);
    void SaveWeatherPreset(const std::string& name, const WeatherPresetData& data);
    
    void RegisterWeatherEffect(const std::string& name, std::unique_ptr<IWeatherEffect> effect);
    void UnregisterWeatherEffect(const std::string& name);
    
    WeatherState GetCurrentWeather(WeatherZoneHandle handle) const;
    Vector3 GetWindVector(const Vector3& position) const;
    const RuntimeWeatherStats& GetStats() const { return m_Stats; }

private:
    RuntimeWeatherSystem() = default;
    
    RuntimeWeatherConfig m_Config;
    RuntimeWeatherStats m_Stats;
    std::unordered_map<WeatherZoneHandle, WeatherZoneInstance> m_Zones;
    std::unordered_map<std::string, std::unique_ptr<IWeatherEffect>> m_Effects;
    std::unique_ptr<IWeatherSimulator> m_Simulator;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 