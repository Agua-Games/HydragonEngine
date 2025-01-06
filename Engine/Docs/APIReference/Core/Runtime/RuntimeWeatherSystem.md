
## Classes

### RuntimeWeatherSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeWeatherConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateZone



**Parameters:** const WeatherZoneDesc& desc

---

### DestroyZone



**Parameters:** WeatherZoneHandle handle

---

### SetWeatherState



**Parameters:** WeatherZoneHandle handle, const WeatherState& state, float transitionTime = -1.0f

---

### BlendWeatherStates



**Parameters:** WeatherZoneHandle handle, const WeatherState& from, const WeatherState& to, float duration

---

### SetPrecipitationType



**Parameters:** WeatherZoneHandle handle, PrecipitationType type

---

### SetPrecipitationIntensity



**Parameters:** WeatherZoneHandle handle, float intensity

---

### SetWindDirection



**Parameters:** WeatherZoneHandle handle, const Vector3& direction

---

### SetWindSpeed



**Parameters:** WeatherZoneHandle handle, float speed

---

### AddWindGust



**Parameters:** WeatherZoneHandle handle, const WindGustDesc& desc

---

### SetCloudCoverage



**Parameters:** WeatherZoneHandle handle, float coverage

---

### SetCloudHeight



**Parameters:** WeatherZoneHandle handle, float baseHeight, float topHeight

---

### LoadWeatherPreset



**Parameters:** const std::string& name

---

### SaveWeatherPreset



**Parameters:** const std::string& name, const WeatherPresetData& data

---

### RegisterWeatherEffect



**Parameters:** const std::string& name, std::unique_ptr<IWeatherEffect> effect

---

### UnregisterWeatherEffect



**Parameters:** const std::string& name

---
