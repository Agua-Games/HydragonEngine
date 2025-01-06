# Classes
---

## RuntimeWeatherSystem
---




# Variables
---

### `enableVolumetricClouds`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDynamicWind`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePrecipitation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTimeOfDay`

- **Type:** `bool`

- **Default Value:** `true`



### `maxWeatherZones`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `particleSystemsPerZone`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `weatherTransitionTime`

- **Type:** `float`

- **Default Value:** `3.0f`



### `windUpdateInterval`

- **Type:** `float`

- **Default Value:** `0.5f`



### `weatherPresetPath`

- **Type:** `string`

- **Default Value:** `"Config/Weather"`



### `config`

- **Type:** `const RuntimeWeatherConfig&`

- **Default Value:** `{})`



### `transitionTime`

- **Type:** `float`

- **Default Value:** `-1.0f)`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeWeatherConfig`



### `m_Stats`

- **Type:** `RuntimeWeatherStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeWeatherConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static RuntimeWeatherSystem&`

---

## Initialize



- **Parameters:** const RuntimeWeatherConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## CreateZone



- **Parameters:** const WeatherZoneDesc& desc

- **Return:** `WeatherZoneHandle`

---

## DestroyZone



- **Parameters:** WeatherZoneHandle handle

- **Return:** `void`

---

## SetWeatherState



- **Parameters:** WeatherZoneHandle handle, const WeatherState& state, float transitionTime = -1.0f

- **Return:** `void`

---

## BlendWeatherStates



- **Parameters:** WeatherZoneHandle handle, const WeatherState& from, const WeatherState& to, float duration

- **Return:** `void`

---

## SetPrecipitationType



- **Parameters:** WeatherZoneHandle handle, PrecipitationType type

- **Return:** `void`

---

## SetPrecipitationIntensity



- **Parameters:** WeatherZoneHandle handle, float intensity

- **Return:** `void`

---

## SetWindDirection



- **Parameters:** WeatherZoneHandle handle, const Vector3& direction

- **Return:** `void`

---

## SetWindSpeed



- **Parameters:** WeatherZoneHandle handle, float speed

- **Return:** `void`

---

## AddWindGust



- **Parameters:** WeatherZoneHandle handle, const WindGustDesc& desc

- **Return:** `void`

---

## SetCloudCoverage



- **Parameters:** WeatherZoneHandle handle, float coverage

- **Return:** `void`

---

## SetCloudHeight



- **Parameters:** WeatherZoneHandle handle, float baseHeight, float topHeight

- **Return:** `void`

---

## LoadWeatherPreset



- **Parameters:** const std::string& name

- **Return:** `void`

---

## SaveWeatherPreset



- **Parameters:** const std::string& name, const WeatherPresetData& data

- **Return:** `void`

---

## RegisterWeatherEffect



- **Parameters:** const std::string& name, std::unique_ptr<IWeatherEffect> effect

- **Return:** `void`

---

## UnregisterWeatherEffect



- **Parameters:** const std::string& name

- **Return:** `void`

---
